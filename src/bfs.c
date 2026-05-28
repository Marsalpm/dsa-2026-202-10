#include <stdio.h>
#include "bfs.h"
#include "streets.h"
#include "intersection.map.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>

// Crea una cua buida i reserva memòria
Queue* createQueue(){
    Queue *queue = malloc(sizeof(Queue)); // Reservem memòria per la cua
    // Si no hi ha memòria disponible, retornem NULL
    if(queue==NULL) return NULL;
    
    // Inicialitzem el davant i el darrere de la cua a NULL
    queue->back = NULL;
    queue->front = NULL;
    
    return queue; // Retornem el punter a la nova cua
}

// Comprova si la cua està buida
int queueisEmpty(Queue *q){
    // Retorna 1 (cert) si no hi ha ni davant ni darrere
    if (q->back ==NULL && q->front == NULL) return 1;
    return 0; // Fals en cas contrari
}

// Afegeix un camí (PathNode) al final de la cua
void enqueue(Queue *q, PathNode *p, long long current_intersection){
    QueueItem *a = malloc(sizeof(QueueItem)); // Creem un nou element per la cua
    a->path = p;    // Hi guardem el camí
    a->current_intersection = current_intersection;
    a->next = NULL; // Com serà l'últim, el següent és NULL
    
    // Si la cua està buida, aquest element serà tant el principi com el final
    if(queueisEmpty(q)==1){
        q->back=a; 
        q->front=a;
    }
    // Si no està buida, l'afegim al final de la llista enllaçada i actualitzem el darrere
    else{
        q->back->next= a;
        q->back = a;
    }
}

// Treu i retorna el primer camí (PathNode) de la cua
PathNode* dequeue(Queue *q, long long *current_intersection){
    if(queueisEmpty(q)==1) return NULL; // Si està buida retornem NULL
    
    QueueItem *temp = q->front; // Guardem temporalment el primer element
    PathNode *path = temp->path; // Extraiem el camí que conté
    if (current_intersection != NULL) {
        *current_intersection = temp->current_intersection;
    }
    
    q->front = temp->next; // El principi de la cua passa a ser el següent element
    
    // Si la cua queda buida, actualitzem també el punter del final a NULL
    if(q->front==NULL) q->back=NULL;
    
    free(temp); // Alliberem la memòria de l'element de la cua extret
    return path; // Retornem el camí que hem tret
}

// Buidem la cua i alliberem tota la memòria associada
void queuefree(Queue *q){
    // Mentre la cua no estigui buida, anem desencuant i alliberant cada camí
    while(queueisEmpty(q)==0){
        pathfree(dequeue(q, NULL));
    }
    free(q); // Alliberem també l'estructura de la cua
    return;
}

// Alliberem un camí (llista enllaçada de PathNodes)
void pathfree(PathNode *p){
    // Recorrem el camí i alliberem cada node
    while(p!=NULL){
        PathNode *temp = p; // Guardem el node actual
        p=p->next;          // Avancem al següent
        free(temp);         // Alliberem l'actual
    }
}

// Crea un nou camí afegint un carrer al final d'un camí existent
PathNode* pathAppend(PathNode *p, Street street){
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
    while(copy->next!=NULL){
        copy = copy->next;
    }
    
    copy->next = new; // L'afegim al final de la còpia
    
    return head; // Retornem el nou camí (còpia + carrer afegit)
}


// Crea una còpia independent d'un camí (Deep Copy)
PathNode* pathCopy(PathNode *p){
    if(p==NULL)return NULL; // Si és nul, la còpia també
    
    // Creem el primer node
    PathNode *new = malloc(sizeof(PathNode));
    new->street = p->street;
    PathNode *head = new; // Guardem el cap per retornar-lo
    
    p = p->next; // Avancem al següent node de l'original
    
    // Recorrem la resta del camí original per anar-lo copiant
    while(p!=NULL){
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
PathNode *bfs(HashMap *graph, Street fromStreet, Street toStreet){
    Queue *q = createQueue(); // Creem la cua per al BFS
    
    VisitedSet *visited = createVisitedSet(); // Hash Set de segments visitats
    
    // Marquem el segment de partida com a visitat immediatament (evitar revisitar)
    addVisitedSet(visited, fromStreet.id1, fromStreet.id2);
    
    // Creem el node inicial sortint per id2 (direcció 1)
    PathNode *initial1 = malloc(sizeof(PathNode));
    initial1->street = fromStreet;
    initial1->next = NULL;
    enqueue(q, initial1, fromStreet.id2);
    
    // Creem el node inicial sortint per id1 (direcció 2)
    PathNode *initial2 = malloc(sizeof(PathNode));
    initial2->street = fromStreet;
    initial2->next = NULL;
    enqueue(q, initial2, fromStreet.id1);
    
    // Mentre hi hagi camins per explorar a la cua
    while (queueisEmpty(q)!=1){
        long long exit_intersection; // La intersecció per on sortim del darrer segment
        PathNode *current = dequeue(q, &exit_intersection);
        PathNode *temp = current;
        
        // Busquem quin és l'últim carrer d'aquest camí
        while(temp->next!=NULL){
            temp = temp->next;
        }
        
        // Si hem arribat al carrer de destí, retornem el camí
        if(toStreet.id1==temp->street.id1 && toStreet.id2==temp->street.id2){
            freeVisitedSet(visited);
            queuefree(q);
            return current;
        }
        
        // Busquem a quins carrers podem anar des de la intersecció de sortida
        HashEntry *entry = findIntersection(graph, exit_intersection);
        
        if(entry != NULL){
            StreetList *connected = entry->streets;
            
            while(connected != NULL){
                Street s = connected->street;
                // Si el segment no ha estat visitat, l'afegim i el marquem
                if(isVisitedSet(visited, s.id1, s.id2)==0){
                    addVisitedSet(visited, s.id1, s.id2); // Marquem en encuar (no en desencuar)
                    // La intersecció de sortida del nou segment és l'extrem contrari a l'entrada
                    long long next_exit = (s.id1 == exit_intersection) ? s.id2 : s.id1;
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

//Funció per a que sempre posi el numero petit primer
void normalize(long long *a, long long *b){
    if(*a > *b){
        long long tmp = *a;
        *a = *b;
        *b = tmp;
    }
}

// Comprova si un carrer (definit pels seus IDs) ja ha estat visitat
int isVisited(VisitedNode *visited, long long id1, long long id2){
    // Recorrem la llista de visitats
    while (visited!=NULL){
        // Si trobem les dues interseccions, ja ha estat visitat
        if(visited->id1==id1 && visited->id2==id2)return 1;
        visited = visited->next;
    }
    return 0; // Si no el trobem, no està visitat
}

// Afegeix una nova intersecció visitada al principi de la llista
VisitedNode* addVisited(VisitedNode *visited, long long id1, long long id2){
    VisitedNode *new = malloc(sizeof(VisitedNode)); // Reservem memòria
    new->id1 = id1; // Guardem els identificadors
    new->id2 = id2;
    new->next = visited; // L'enllacem a l'inici de la llista
    return new; // Retornem el nou cap de la llista
}

// Allibera la llista de nodes visitats
void freeVisited(VisitedNode *visited){
    // Recorrem i alliberem un a un
    while(visited!=NULL){
        VisitedNode *temp = visited; // Guardem l'actual
        visited = visited->next;     // Avancem
        free(temp);                  // Alliberem l'actual
    }
}


// Funció de hash que combina id1 i id2 en un sol índex
static unsigned int visitedHash(long long id1, long long id2) {
    unsigned long long combined = (unsigned long long)id1 * 2654435761ULL ^ (unsigned long long)id2;
    return (unsigned int)(combined % VISITED_TABLE_SIZE);
}

// Crea un VisitedSet buit amb tots els buckets a NULL
VisitedSet *createVisitedSet() {
    VisitedSet *set = malloc(sizeof(VisitedSet));
    if (set == NULL) return NULL;
    for (int i = 0; i < VISITED_TABLE_SIZE; i++) {
        set->table[i] = NULL;
    }
    return set;
}

// Comprova si un segment (id1, id2) ja està dins el hash set
int isVisitedSet(VisitedSet *set, long long id1, long long id2) {

    long long a = id1;
    long long b = id2;

    normalize(&a, &b);

    unsigned int index = visitedHash(a, b);

    VisitedEntry *entry = set->table[index];

    while (entry != NULL) {

        if (entry->id1 == a && entry->id2 == b)
            return 1;

        entry = entry->next;
    }

    return 0;
}

// Afegeix un segment (id1, id2) al hash set
void addVisitedSet(VisitedSet *set, long long id1, long long id2) {

    long long a = id1;
    long long b = id2;

    normalize(&a, &b);

    unsigned int index = visitedHash(a, b);

    VisitedEntry *newEntry = malloc(sizeof(VisitedEntry));
    newEntry->id1 = a;
    newEntry->id2 = b;

    newEntry->next = set->table[index];
    set->table[index] = newEntry;
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

// Converteix coordenades (lat, lon) a coordenades cartesianes (x, y) per calcular distàncies i angles
void latlon_to_xy(double lat_ref, double lon_ref, double lat, double lon, double *x, double *y) {
    double lat_ref_rad = toRadians(lat_ref);    // Latitud de referència en radians
    double dlat = toRadians(lat - lat_ref);     // Diferència de latitud
    double dlon = toRadians(lon - lon_ref);     // Diferència de longitud
    
    // Projecció equirectangular
    *x = EARTH_RADIUS * dlon * cos(lat_ref_rad);
    *y = EARTH_RADIUS * dlat;
}

// Imprimeix la ruta a seguir (Indicacions "turn left" / "turn right")
void printRoute(PathNode *path){
    if (path == NULL) return;
    
    printf("Start at %s\n", path->street.name); // Inici
    
    if (path->next == NULL) {
        printf("You have arrived to your destination\n");
        return;
    }
    
    PathNode *current = path;                   // Punter per recórrer el camí
    int distance = path->street.length;         // Distància acumulada pel mateix carrer
    
    // Recorrem fins al penúltim element per calcular canvis de sentit
    while(current->next != NULL){
        PathNode *prev = current;       // Carrer actual
        current = current->next;        // Següent carrer
        
        // Trobem la intersecció compartida (B) entre prev i current
        double b_lat, b_lon, a_lat, a_lon, c_lat, c_lon;
        
        if ((prev->street.id1 == current->street.id1) || (prev->street.id1 == current->street.id2)) {
            b_lat = prev->street.lat1;
            b_lon = prev->street.lon1;
            a_lat = prev->street.lat2;
            a_lon = prev->street.lon2;
        } else {
            b_lat = prev->street.lat2;
            b_lon = prev->street.lon2;
            a_lat = prev->street.lat1;
            a_lon = prev->street.lon1;
        }
        
        if (current->street.id1 == prev->street.id1 || current->street.id1 == prev->street.id2) {
            c_lat = current->street.lat2;
            c_lon = current->street.lon2;
        } else {
            c_lat = current->street.lat1;
            c_lon = current->street.lon1;
        }
        
        // Coordenades planes referenciades a la intersecció central (B)
        double ax, ay, bx, by, cx, cy;
        
        // Transformem lat/lon a plans per operar amb vectors
        latlon_to_xy(b_lat, b_lon, a_lat, a_lon, &ax, &ay);
        latlon_to_xy(b_lat, b_lon, b_lat, b_lon, &bx, &by);
        latlon_to_xy(b_lat, b_lon, c_lat, c_lon, &cx, &cy);
        
        // Vectors de direcció: AB i BC
        double abx = bx - ax;
        double aby = by - ay;
        double bcx = cx - bx;
        double bcy = cy - by;
        
        // Producte creuat per veure si es gira a la dreta o esquerra
        double cross = abx * bcy - aby * bcx;
        
        // Si continuem pel mateix carrer, acumulem distància
if(strcmp(prev->street.name, current->street.name)==0){

    distance += current->street.length;
}
else{

    // Evitem imprimir canvis absurds molt curts
    if(distance > 15){

        // Determinem si el gir és esquerra o dreta
        if(cross < 0){

            printf("Turn left to %s and continue for %dm\n",
                   current->street.name,
                   (int)distance);
        }
        else{

            printf("Turn right to %s and continue for %dm\n",
                   current->street.name,
                   (int)distance);
        }
    }

    // Reiniciem la distància del nou carrer
    distance = current->street.length;
        }
        
    }
    // Avís d'arribada al destí
    printf("Continue on %s for %dm\n", current->street.name, (int)distance);
    printf("You have arrived to your destination\n");
}

//Busca carrers connectats fent cerca lineal per tota la llista
StreetNode *findConnectedLinear(StreetNode *streets, long long intersection){

    StreetNode *head = NULL; //Primer element de la llista resultat
    StreetNode *tail = NULL; //Ultim element per mantenir l'ordre

    while(streets != NULL){ //Recorrem tota la llista de carrers

        if (streets->street.id1 == intersection || streets->street.id2 == intersection) {   //Comprova si el carrer actual toca aquella intersecció
           
            StreetNode *newNode = malloc(sizeof(StreetNode));//Reservem memòria per un nou node

            newNode->street = streets->street;//Copiem el carrer trobat

            newNode->next = NULL;//Com serà l'últim element, next apunta a NULL

            if(head == NULL){//Si la llista està buida, aquest node serà el primer i l'últim
                head = newNode;
                tail = newNode;
            }
            else{
                tail->next = newNode;//Afegim el nou node al final de la llista
                tail = newNode; //Actualitzem el tail
            }
        }
        streets = streets->next; //Avancem al següent carrer de la llista original
    }
    return head; //Retornem la nova llista de carrers connectats trobats
}
    // BFS amb cerca lineal
PathNode *bfs_slow(StreetNode *streets, Street fromStreet, Street toStreet) {

    Queue *q = createQueue();   //Crea cua que guardara camins pendents d'explorar
    VisitedSet *visited = createVisitedSet();   //Crea el hash de segments visitats (no repetir carrers i evitar bucles)

    addVisitedSet(visited, fromStreet.id1, fromStreet.id2); //Marca que s'ha visitat el carrer inicial 

    PathNode *initial = malloc(sizeof(PathNode));   //Crea el primer node del cami
    initial->street = fromStreet;   //Guardem carrer inicial
    initial->next = NULL;   //El camí només té un carrer de moment

    enqueue(q, initial, fromStreet.id2);    //Cami inicial a la cua i guardem intersecció de sortida

    while (queueisEmpty(q) != 1) {  //Mentres hi hagin camins a explorar

        long long exit_intersection;    
        PathNode *current = dequeue(q, &exit_intersection); //Treu el seguent cami de la cua i recuperem interseccio de on sortim
        PathNode *temp = current;   //Busquem ultim carrer del cami
        while (temp->next != NULL) {    //Busquem ultim carrer del cami
            temp = temp->next;
        }
        
        if (toStreet.id1 == temp->street.id1 && toStreet.id2 == temp->street.id2) { //Comprova si hem arribat al destí, si és aixi
            freeVisitedSet(visited); //Alliberem memoria
            queuefree(q);   //Alliberem memoria
            return current; //Retornem cami trobat
        }

        StreetNode *connected = findConnectedLinear(streets, exit_intersection);    //Mirem carrers connectats a la intersecció
        StreetNode *aux = connected; //Auxiliar per recorrer els carrers conectats
         
        while (aux != NULL) {   //Recorre tots els carrers connectats trobats

            Street s = aux->street; //Guardem el carrer actual
            if (isVisitedSet(visited, s.id1, s.id2) == 0) { //Comprova si encara no hem visitat aquell segment

                addVisitedSet(visited, s.id1, s.id2);   //Marca el segment com visitat
                 long long next_exit = (s.id1 == exit_intersection) ? s.id2 : s.id1;    //Calacuem per quina intersecció sortirem despres
                PathNode *newPath = pathAppend(current, s); //Crea nou cami copiant el cami actual i afegint el nou carrer
                enqueue(q, newPath, next_exit); //Posa el nou cami a la cua
            }

            aux = aux->next;    //Seguent
        }

        while (connected != NULL) { //Alliberem llista temporal creada
            StreetNode *tmp = connected;
            connected = connected->next;
            free(tmp);  
        }
        pathfree(current);  //Alliberem cami actual perque ja l'hem exportat
    }
    //Si no troba el cami allibera memoria i retorna null
    freeVisitedSet(visited);
    queuefree(q);
    return NULL;
}
