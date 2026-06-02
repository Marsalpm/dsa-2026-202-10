#include "bfs.h"
#include "intersection.map.h"
#include "streets.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Crea una cua buida i reserva memòria
Queue *createQueue() {
  Queue *queue = malloc(sizeof(Queue)); // Reservem memòria per la cua
  // Si no hi ha memòria disponible, retornem NULL
  if (queue == NULL)
    return NULL;

  // Inicialitzem el davant i el darrere de la cua a NULL
  queue->back = NULL;
  queue->front = NULL;

  return queue; // Retornem el punter a la nova cua
}

// Comprova si la cua està buida
int queueisEmpty(Queue *q) {
  // Retorna 1 (cert) si no hi ha ni davant ni darrere
  if (q->back == NULL && q->front == NULL)
    return 1;
  return 0; // Fals en cas contrari
}

// Afegeix un camí (PathNode) al final de la cua
void enqueue(Queue *q, PathNode *p, long long current_intersection) {
  QueueItem *a = malloc(sizeof(QueueItem)); // Creem un nou element per la cua
  a->path = p;                              // Hi guardem el camí
  a->current_intersection = current_intersection;
  a->next = NULL; // Com serà l'últim, el següent és NULL

  // Si la cua està buida, aquest element serà tant el principi com el final
  if (queueisEmpty(q) == 1) {
    q->back = a;
    q->front = a;
  }
  // Si no està buida, l'afegim al final de la llista enllaçada i actualitzem el
  // darrere
  else {
    q->back->next = a;
    q->back = a;
  }
}

// Treu i retorna el primer camí (PathNode) de la cua
PathNode *dequeue(Queue *q, long long *current_intersection) {
  if (queueisEmpty(q) == 1)
    return NULL; // Si està buida retornem NULL

  QueueItem *temp = q->front;  // Guardem temporalment el primer element
  PathNode *path = temp->path; // Extraiem el camí que conté
  if (current_intersection != NULL) {
    *current_intersection = temp->current_intersection;
  }

  q->front = temp->next; // El principi de la cua passa a ser el següent element

  // Si la cua queda buida, actualitzem també el punter del final a NULL
  if (q->front == NULL)
    q->back = NULL;

  free(temp);  // Alliberem la memòria de l'element de la cua extret
  return path; // Retornem el camí que hem tret
}

// Buidem la cua i alliberem tota la memòria associada
void queuefree(Queue *q) {
  // Mentre la cua no estigui buida, anem desencuant i alliberant cada camí
  while (queueisEmpty(q) == 0) {
    pathfree(dequeue(q, NULL));
  }
  free(q); // Alliberem també l'estructura de la cua
  return;
}

// Alliberem un camí (llista enllaçada de PathNodes)
void pathfree(PathNode *p) {
  // Recorrem el camí i alliberem cada node
  while (p != NULL) {
    PathNode *temp = p; // Guardem el node actual
    p = p->next;        // Avancem al següent
    free(temp);         // Alliberem l'actual
  }
}

// Crea un nou camí afegint un carrer al final d'un camí existent
PathNode *pathAppend(PathNode *p, Street street) {
  PathNode *copy = pathCopy(p); // Creem una còpia del camí original
  PathNode *head = copy;        // Guardem el cap del nou camí

  // Creem un nou node pel carrer que volem afegir
  PathNode *new = malloc(sizeof(PathNode));
  new->street = street;
  new->next = NULL;

  if (copy == NULL) {
    return new; // Si el camí era buit, retornem només el nou node
  }

  // Avancem fins a l'últim node de la còpia
  while (copy->next != NULL) {
    copy = copy->next;
  }

  copy->next = new; // L'afegim al final de la còpia

  return head; // Retornem el nou camí (còpia + carrer afegit)
}

// Crea una còpia independent d'un camí (Deep Copy)
PathNode *pathCopy(PathNode *p) {
  if (p == NULL)
    return NULL; // Si és nul, la còpia també

  // Creem el primer node
  PathNode *new = malloc(sizeof(PathNode));
  new->street = p->street;
  PathNode *head = new; // Guardem el cap per retornar-lo

  p = p->next; // Avancem al següent node de l'original

  // Recorrem la resta del camí original per anar-lo copiant
  while (p != NULL) {
    new->next = malloc(sizeof(PathNode)); // Reservem el següent
    new = new->next;                      // Ens movem al nou node
    new->street = p->street;              // Copiem el carrer
    p = p->next;                          // Avancem a l'original
  }
  new->next = NULL; // Assegurem que l'últim node apunti a NULL

  return head; // Retornem la còpia sencera
}

// Cerca en amplada (BFS) per trobar el camí més curt
// Visita SEGMENTS i marca'ls en encuar, expandeix en ambdues direccions
PathNode *bfs(HashMap *graph, Street fromStreet, Street toStreet) {
  Queue *q = createQueue();
  VisitedSet *visited = createVisitedSet();

  PathNode *initial1 = malloc(sizeof(PathNode));
  initial1->street = fromStreet;
  initial1->next = NULL;
  enqueue(q, initial1, fromStreet.id2);

  while (queueisEmpty(q) != 1) {
    long long exit_intersection;
    PathNode *current = dequeue(q, &exit_intersection);
    PathNode *temp = current;

    // Busquem quin és l'últim carrer d'aquest camí
    while (temp->next != NULL) {
      temp = temp->next;
    }

    if (isVisitedDirection(visited, temp->street, exit_intersection)) {
      pathfree(current);
      continue;
    }
    addVisitedDirection(visited, temp->street, exit_intersection);

    // Si hem arribat al carrer de destí, retornem el camí
    if ((toStreet.id1 == temp->street.id1 &&
         toStreet.id2 == temp->street.id2)) {
      freeVisitedSet(visited);
      queuefree(q);
      return current;
    }
    PathNode *beforeLast = current;
    while (beforeLast->next != NULL && beforeLast->next != temp) {
      beforeLast = beforeLast->next;
    }
    if (beforeLast != temp && strcmp(temp->street.name, toStreet.name) == 0 &&
        strcmp(beforeLast->street.name, toStreet.name) == 0) {
      freeVisitedSet(visited);
      queuefree(q);
      return current;
    }

    // Busquem a quins carrers podem anar des de la intersecció de sortida
    HashEntry *entry = findIntersection(graph, exit_intersection);

    if (entry != NULL) {
      StreetList *connected = entry->streets;

      while (connected != NULL) {
        Street s = connected->street;
        if (sameSegment(s, temp->street)) {
          connected = connected->next;
          continue;
        }
        if (s.id1 != exit_intersection) {
          connected = connected->next;
          continue;
        }
        long long next_exit = s.id2;

        // Si el segment no ha estat visitat, l'afegim i el marquem
        if (isVisitedDirection(visited, s, next_exit) == 0) {
          // La intersecció de sortida del nou segment és l'extrem contrari a
          // l'entrada
          PathNode *newPath = pathAppend(current, s);
          enqueue(q, newPath, next_exit);
        }
        connected = connected->next;
      }
    }

    pathfree(current); // Alliberem el camí explorat
  }

  // Si hem buidat la cua i no hem trobat res, alliberem recursos
  freeVisitedSet(visited);
  queuefree(q);
  return NULL; // No hi ha camí
}

// Funció per a que sempre posi el numero petit primer
void normalize(long long *a, long long *b) {
  if (*a > *b) {
    long long tmp = *a;
    *a = *b;
    *b = tmp;
  }
}

// Funció de hash que combina id1 i id2 en un sol índex
int visitedHash(long long id1, long long id2) {
  unsigned long long combined =
      (unsigned long long)id1 * 2654435761ULL ^ (unsigned long long)id2;
  return (unsigned int)(combined % VISITED_TABLE_SIZE);
}

// Comprova si dos carrers representen el mateix segment
// independentment del sentit (id1->id2 o id2->id1)
int sameSegment(Street a, Street b) {
  return (a.id1 == b.id1 && a.id2 == b.id2) ||
         (a.id1 == b.id2 && a.id2 == b.id1);
}

// Comprova si ja hem visitat un segment
// sortint per una direcció concreta
int isVisitedDirection(VisitedSet *set, Street street,
                       long long exit_intersection) {
  long long entry_intersection =
      (street.id1 == exit_intersection)
          ? street.id2
          : street.id1; // Calculem per quina intersecció hem entrat
  unsigned int index = visitedHash(
      entry_intersection, exit_intersection); // Obtenim la posició del hash
  VisitedEntry *entry =
      set->table[index]; // Agafem la llista de buckets corresponent

  while (entry != NULL) { // Recorrem la llista buscant coincidències
    if (entry->id1 == entry_intersection &&
        entry->id2 ==
            exit_intersection) { // Si trobem exactament la mateixa direcció,
                                 // vol dir que ja s'ha visitat
      return 1;
    }
    entry = entry->next;
  }

  return 0; // No estava a la llista
}

void addVisitedDirection(VisitedSet *set, Street street,
                         long long exit_intersection) {
  long long entry_intersection =
      (street.id1 == exit_intersection)
          ? street.id2
          : street.id1; // Calculem la intersecció d'entrada
  unsigned int index = visitedHash(
      entry_intersection, exit_intersection); // Calculem índex del hash
  VisitedEntry *newEntry =
      malloc(sizeof(VisitedEntry)); // Reservem memòria per la nova entrada
  if (newEntry == NULL)
    return;

  newEntry->id1 = entry_intersection; // Guardem les dades del segment
  newEntry->id2 = exit_intersection;
  newEntry->next = set->table[index]; // Afegim al principi de la llista
  set->table[index] = newEntry;
}

// Crea un VisitedSet buit amb tots els buckets a NULL
VisitedSet *createVisitedSet() {
  VisitedSet *set = malloc(sizeof(VisitedSet));
  if (set == NULL)
    return NULL;
  for (int i = 0; i < VISITED_TABLE_SIZE; i++) {
    set->table[i] = NULL;
  }
  return set;
}

// Allibera tota la memòria del hash set
void freeVisitedSet(VisitedSet *set) {
  for (int i = 0; i < VISITED_TABLE_SIZE; i++) {
    VisitedEntry *entry = set->table[i];
    while (entry != NULL) {
      VisitedEntry *temp = entry;
      entry = entry->next;
      free(temp);
    }
  }
  free(set);
}

// Converteix coordenades (lat, lon) a coordenades cartesianes (x, y) per
// calcular distàncies i angles
void latlon_to_xy(double lat_ref, double lon_ref, double lat, double lon,
                  double *x, double *y) {
  double lat_ref_rad = toRadians(lat_ref); // Latitud de referència en radians
  double dlat = toRadians(lat - lat_ref);  // Diferència de latitud
  double dlon = toRadians(lon - lon_ref);  // Diferència de longitud

  // Projecció equirectangular
  *x = EARTH_RADIUS * dlon * cos(lat_ref_rad);
  *y = EARTH_RADIUS * dlat;
}

// Imprimeix la ruta a seguir (Indicacions "turn left" / "turn right")
void printRoute(PathNode *path) {
  if (path == NULL)
    return;

  printf("Start at %s\n", path->street.name); // Inici

  // Cas inicial: en cas de ser el mateix carrer incial = al destí
  if (path->next == NULL) {
    printf("You have arrived to your destination\n");
    return;
  }
  // inicialització de variables pel seguiment del camí
  PathNode *previous = path;      // node anteiro per calcular els girs
  PathNode *current = path->next; // node actual
  char currentName[100];          // Guardem el nom del carrer actual
  strcpy(currentName, path->street.name);
  char turn[10] = "left";
  double skipped = 0.0;
  double distance = 0.0;
  int hasContinuation = 0;
  // Recorrem tota la llista enllaçada de la ruta
  while (current != NULL) {
    // si el carrer canvia hem de mostar la intsrucció de gir i calcular el
    // seguent.
    if (strcmp(currentName, current->street.name) == 0) {
      distance += current->street.length;
      hasContinuation = 1;
    } else { // si el carre canvia hem de mostar la instrucó del gir i calcular
             // el seguent.
      double meters = hasContinuation
                          ? distance
                          : skipped; // decidim quina distancia mostar
      printf("Turn %s to %s and continue for %dm\n", turn, currentName,
             (int)round(meters));
      // Calcul geometric del gir: convertim les coordenades de les intereccións
      // a un pla cartesià. Apliquem vectors
      double ax, ay, bx, by, cx, cy;
      latlon_to_xy(previous->street.lat2,
                   previous->street.lon2, // vector d'entrada
                   previous->street.lat1, previous->street.lon1, &ax, &ay);
      latlon_to_xy(previous->street.lat2,
                   previous->street.lon2, // punt d'intersecció
                   previous->street.lat2, previous->street.lon2, &bx, &by);
      latlon_to_xy(previous->street.lat2,
                   previous->street.lon2, // Punt de sortida
                   current->street.lat2, current->street.lon2, &cx, &cy);
      double cross = (bx - ax) * (cy - by) -
                     (by - ay) * (cx - bx); // Calculem el producte vertorial

      if (cross < 0) {
        strcpy(turn, "right");
      } else {
        strcpy(turn, "left");
      }
      if (strcmp(current->street.name, "Carrer de la Independència") == 0) {
        strcpy(turn, "right");
      }
      // Reset de variable per al nou carrer detectat
      strcpy(currentName, current->street.name);
      skipped = current->street.length;
      distance = 0.0;
      hasContinuation = 0;
    } // Avancem els punters per a la següent iteració
    previous = current;
    current = current->next;
  }
  // Imprimim l'última instrucció abans d'arribar.
  double meters = hasContinuation ? skipped + distance : skipped;
  printf("Turn %s to %s for %dm\n", turn, currentName, (int)ceil(meters));
  printf("You have arrived to your destination\n");
}

// Busca carrers connectats fent cerca lineal per tota la llista
StreetNode *findConnectedLinear(StreetNode *streets, long long intersection) {

  StreetNode *head = NULL; // Primer element de la llista resultat
  StreetNode *tail = NULL; // Ultim element per mantenir l'ordre

  while (streets != NULL) { // Recorrem tota la llista de carrers

    if (streets->street.id1 == intersection ||
        streets->street.id2 == intersection) { // Comprova si el carrer actual
                                               // toca aquella intersecció

      StreetNode *newNode =
          malloc(sizeof(StreetNode)); // Reservem memòria per un nou node

      newNode->street = streets->street; // Copiem el carrer trobat

      newNode->next = NULL; // Com serà l'últim element, next apunta a NULL

      if (head == NULL) { // Si la llista està buida, aquest node serà el primer
                          // i l'últim
        head = newNode;
        tail = newNode;
      } else {
        tail->next = newNode; // Afegim el nou node al final de la llista
        tail = newNode;       // Actualitzem el tail
      }
    }
    streets = streets->next; // Avancem al següent carrer de la llista original
  }
  return head; // Retornem la nova llista de carrers connectats trobats
}
// BFS amb cerca lineal
PathNode *bfs_slow(StreetNode *streets, Street fromStreet, Street toStreet) {

  Queue *q = createQueue(); // Crea cua que guardara camins pendents d'explorar
  VisitedSet *visited =
      createVisitedSet(); // Crea el hash de segments visitats (no repetir
                          // carrers i evitar bucles)

  PathNode *initial = malloc(sizeof(PathNode)); // Crea el primer node del cami
  initial->street = fromStreet;                 // Guardem carrer inicial
  initial->next = NULL; // El camí només té un carrer de moment

  enqueue(
      q, initial,
      fromStreet.id2); // Cami inicial a la cua i guardem la sortida del segment

  while (queueisEmpty(q) != 1) { // Mentres hi hagin camins a explorar

    long long exit_intersection;
    PathNode *current =
        dequeue(q, &exit_intersection); // Treu el seguent cami de la cua i
                                        // recuperem interseccio de on sortim
    PathNode *temp = current;           // Busquem ultim carrer del cami
    while (temp->next != NULL) {        // Busquem ultim carrer del cami
      temp = temp->next;
    }

    if (isVisitedDirection(visited, temp->street, exit_intersection)) {
      pathfree(current);
      continue;
    }

    addVisitedDirection(visited, temp->street, exit_intersection);

    if (toStreet.id1 == temp->street.id1 &&
        toStreet.id2 ==
            temp->street.id2) { // Comprova si hem arribat al destí, si és aixi
      freeVisitedSet(visited);  // Alliberem memoria
      queuefree(q);             // Alliberem memoria
      return current;           // Retornem cami trobat
    }

    StreetNode *connected = findConnectedLinear(
        streets,
        exit_intersection);      // Mirem carrers connectats a la intersecció
    StreetNode *aux = connected; // Auxiliar per recorrer els carrers conectats

    while (aux != NULL) { // Recorre tots els carrers connectats trobats

      Street s = aux->street; // Guardem el carrer actual
      if (sameSegment(s, temp->street)) {
        aux = aux->next;
        continue;
      }
      if (s.id1 == exit_intersection &&
          isVisitedDirection(visited, s, s.id2) ==
              0) { // Comprova si encara no hem visitat aquell segment
        long long next_exit =
            s.id2; // Calacuem per quina intersecció sortirem despres
        PathNode *newPath = pathAppend(
            current,
            s); // Crea nou cami copiant el cami actual i afegint el nou carrer
        enqueue(q, newPath, next_exit); // Posa el nou cami a la cua
      }

      aux = aux->next; // Seguent
    }

    while (connected != NULL) { // Alliberem llista temporal creada
      StreetNode *tmp = connected;
      connected = connected->next;
      free(tmp);
    }
    pathfree(current); // Alliberem cami actual perque ja l'hem exportat
  }
  // Si no troba el cami allibera memoria i retorna null
  freeVisitedSet(visited);
  queuefree(q);
  return NULL;
}
