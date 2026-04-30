#include "houses.h"
#include "../test/utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // Incloem per poder usar strcasecmp i strncasecmp

HouseNode *addHouse(HouseNode *head, House house) {
  HouseNode *newNode =
      malloc(sizeof(HouseNode)); // Creem nou node i reservem memoria
  if (!newNode)
    return head;          // Si falla la reserva, retornem la llista actual
  newNode->house = house; // Guardem la casa pasada al nou node
  newNode->next = head; // Nou node apunta a l'antic cap (insertem al principi)

  return newNode; // Tornem el nou node que ara és el cap
}

HouseNode *loadHouses(char *filename) {
  FILE *file = fopen(filename, "r"); // Obrim el fitxer en mode lectura

  if (file == NULL) { // Comprovem si el fitxer s'ha obert correctament
    printf("No s'ha pogut obrir el fitxer\n");
    return NULL; // Si no existeix, retornem NULL
  }

  HouseNode *head = NULL; // Inicialitzem el cap de la llista a buit
  char line[256];         // Buffer per guardar cada línia llegida del fitxer

  while (fgets(line, sizeof(line),
               file)) { // Llegim el fitxer línia a línia fins al final
    House house;        // Creem una estructura temporal per a la casa
    // Parsegem la línia separada per punts i coma i guardem les dades
    if (sscanf(line, " %99[^,] ,%d ,%lf ,%lf,%d,%d", house.street, &house.number,
               &house.lat, &house.lon,&house.id1,&house.id2) == 6) {
      head = addHouse(head, house); // Afegim la casa parsejada a la llista
    }
  }

  fclose(file); // Tanquem el fitxer per alliberar recursos

  return head; // Retornem la llista enllaçada completa
}

HouseNode *findHouse(HouseNode *head, char *street, int number) {
  HouseNode *current = head; // Comencem la cerca pel principi de la llista
  // Preparem un string per normalitzar el carrer buscat
  char inputstreet[100];
  strcpy(inputstreet, street); // Copiem el carrer d'entrada
  minuscules(inputstreet); // El passem a minúscules per ignorar diferències

  while (current != NULL) { // Recorrem la llista mentre no arribem al final

    // Preparem un string per normalitzar el carrer del node actual
    char currentStreet[100];
    strcpy(currentStreet, current->house.street); // Copiem el carrer de la casa
    minuscules(currentStreet);                    // El passem a minúscules
    // Comparem el nom normalitzat i el número de la casa //
    if (strcmp(currentStreet, inputstreet) == 0 &&
        current->house.number == number) {
      return current; // Si coincideixen, hem trobat la casa i la retornem
    }
    current = current->next; // Si no, avancem al següent node de la llista
  }
  return NULL; // Si acabem el bucle sense trobar res, retornem NULL
}
// Fucnions auxilars:
void printCoordinates(House house) {
  printf("Trobat a (%f, %f)\n", house.lat,
         house.lon); // Mostra les coordenades per pantalla
}

void printfSetreetSegment(House house){
    printf("Segment de carrer: Intersecció %d i Intersecció %d\n",house.id1,house.id2);
}

// Cerca un carrer gestionant abreviatures, números erronis i similituds
// FUNCIO DE CERCA INTELIGENT LAB 2 I LAB 3:
void cerca_inteligent_houses(HouseNode *head, char *street, int number) {
  char final[100];
  char street_net[100]; // Variable per guardar el nom del carrer "netejat"

  // Si comença per "C. ", ho expandim a "Carrer "
  if (strncasecmp(street, "C. ", 3) == 0) {
    sprintf(street_net, "Carrer %s",
            street + 3); // Saltem els 3 primers caràcters
  }
  // Si comença per "Av. ", ho expandim a "Avinguda "
  else if (strncasecmp(street, "Av. ", 4) == 0) {
    sprintf(street_net, "Avinguda %s",
            street + 4); // Saltem els 4 primers caràcters
  } else {
    strcpy(street_net,
           street); // Si no hi ha abreviatura, copiem el nom tal qual
  }

  // Busquem si la casa exacta existeix amb el nom netejat
  HouseNode *resultat = findHouse(head, street_net, number);
  if (resultat != NULL) {
    printCoordinates(resultat->house); // Si existeix, imprimim coordenades
    printfSetreetSegment(resultat->house);
    return;                            // Sortim de la funció
  } else
    printf("No s'ha trobat la casa exacte a %s %d\n", street_net, number);

  // Si no hi és, busquem si almenys el carrer existeix per suggerir números
  HouseNode *temp = head; // Punter temporal per recórrer la llista
  bool street_trobat = false;  // Indicador per saber si hem trobat el carrer
  while (temp != NULL) {
    // Comparem noms de carrer ignorant majúscules
    if (strcasecmp(temp->house.street, street_net) == 0) {
      if (street_trobat == 0)
        printf("Números vàlids disponibles al carrer %s:\n", street_net);
      street_trobat = true;// Marquem que el carrer existeix
      break; 
    }
    temp = temp->next; // Passem al següent node
  }
  // Si el carrer existeix però el número és incorrecte deixem triar
  if (street_trobat) {
    HouseNode *temp1 = head;
    int count = 0;
    while (temp1 != NULL) {
      if (strcasecmp(temp1->house.street, street_net) == 0) {
        printf("| %-4d ", temp1->house.number);
        count++;

        if (count % 5 == 0)
          printf("|\n");
      }
      temp1 = temp1->next;
    }

    if (count % 5 != 0)
      printf("|\n");
    int nou_numero;
    printf("-Exit (0)\n");
    printf("Introdueix un número vàlid del carrer: ");
    int valid_number = scanf("%d", &nou_numero);
    HouseNode *resultat = findHouse(head, street_net, nou_numero);
    if (resultat != NULL) {
      printCoordinates(resultat->house);
      printfSetreetSegment(resultat->house);
      return;
    }
    if (nou_numero==0)return;
    while (valid_number != 1 || resultat == NULL) {
      // vaciar el buffer
      while (getchar() != '\n');
      printf("Si us plau introdueixi un nombre vàlid: ");
      valid_number = scanf("%d", &nou_numero);
      resultat = findHouse(head, street_net, nou_numero);
      if (nou_numero==0)return;
      if (resultat != NULL) {
        printCoordinates(resultat->house);
        printfSetreetSegment(resultat->house);
        return;
      }
    }
  }

  printf("'%s'No s'ha trobat. Volies dir:\n", street_net);

  char sug1[100] = "", sug2[100] = "",
       sug3[100] = ""; // Creem les tres variables que seran els 3 candidats
  int dist1 = 100, dist2 = 100,
      dist3 =
          100; // les distancies que compararem amb la funcio del Levenshtein.

  HouseNode *temp_lev = head;
  while (temp_lev != NULL) { // recorre tota la llista
    int d = levenshteinDistance(
        street_net,
        temp_lev->house
            .street); // Calculem la distanci amb l'algoritme de Levenshtein
    // Evita que es dupliquin.
    if (strcmp(temp_lev->house.street, sug1) == 0 ||
        strcmp(temp_lev->house.street, sug2) == 0 ||
        strcmp(temp_lev->house.street, sug3) == 0) {
      temp_lev = temp_lev->next;
      continue;
    }

    // Guardar les 3 millors opcions comparant amb el que ha posat el usuari.
    if (d < 20) { // limit de 5 erros del que ha posat l'usuari i el original.
                  // Mirem si entra en el nostre top3.
      // Lògica del desplaçament: SI un carrer entra al podi hem de treure els
      // que ja hi eren.
      if (d <= dist1) { // Cas1: converteix el numero 1.
        dist3 = dist2;
        strcpy(sug3, sug2); // el que estava 2n passa a 3r
        dist2 = dist1;
        strcpy(sug2, sug1); // el que estava 1r passa a ser el 2n
        dist1 = d;
        strcpy(sug1, temp_lev->house.street); // el nou 1r
      } else if (d <= dist2) { // Cas2: converteix en el nou nùmero el 2
        dist3 = dist2;
        strcpy(sug3, sug2); // el 2n passa ser 3r
        dist2 = d;
        strcpy(sug2, temp_lev->house.street); // el nou carrer passa a ser el 2n
      } else {
        dist3 = d;
        strcpy(sug3,
               temp_lev->house.street); // Converteix en el 3r, subtituim el 3r
      }
    }
    temp_lev = temp_lev->next;
  }
  int opcions = 0;
  // Fer print dels 3 millors opcions que s'asembla més al que ha dir l'usuari.
  if (dist1 < 100) {
    printf("1. %s\n", sug1);
    opcions++;
    if (dist2 < 100) {
      printf("2. %s\n", sug2);
      opcions++;
    }
    if (dist3 < 100) {
      printf("3. %s\n", sug3);
      opcions++;
    }
     printf("4. Exit (0)\n");
  } else {
    printf("No s'ha trobat res similar.\n");
  }
    if (opcions >= 1) {
    while (true) {
      int opcio;
      printf("Escull una opció (1-%d): ", opcions);
      int valid_number = scanf("%d", &opcio);
      while (valid_number != 1 || 0 > opcio || opcio > opcions) {
        // vaciar el buffer
        while (getchar() != '\n')
          ;
        printf("Si us plau introdueixi un nombre vàlid: ");
        valid_number = scanf("%d", &opcio);
      }
      if (opcio == 0) return;
      else if (opcio == 1) {
        strcpy(final, sug1);
        break;
      } else if (opcio == 2 && opcions >= 2) {
        strcpy(final, sug2);
        break;
      } else if (opcio == 3 && opcions >= 3) {
        strcpy(final, sug3);
        break;
      } else {
        printf("Opció no vàlida.\n");
      }
    }
    printf("Números vàlids disponibles al carrer %s:\n", final);
    HouseNode *temp2 = head;
    int count = 0;

    while (temp2 != NULL) {
      if (strcasecmp(temp2->house.street, final) == 0) {
        printf("| %-4d ", temp2->house.number);
        count++;

        if (count % 5 == 0)
          printf("|\n");
      }
      temp2 = temp2->next;
    }

    if (count % 5 != 0)
      printf("|\n");
    int finalnum;

    printf("Escull el número: ");
    int valid_number = scanf("%d", &finalnum);
    HouseNode *res = findHouse(head, final, finalnum);
    if (res != NULL) {
      printCoordinates(res->house);
      printfSetreetSegment(resultat->house);
      return;
    }
    while (valid_number != 1 || res == NULL) {
      // vaciar el buffer
      while (getchar() != '\n')
        ;
      printf("Si us plau introdueixi un nombre vàlid: ");
      valid_number = scanf("%d", &finalnum);
      res = findHouse(head, final, finalnum);
      if (res != NULL) {
        printCoordinates(res->house);
        printfSetreetSegment(resultat->house);
        return;
      }
    }
  }
}

int min3(int a, int b, int c) {
  if (a <= b && a <= c)
    return a; // Retorna a si és el més petit
  if (b <= a && b <= c)
    return b; // Retorna b si és el més petit
  return c;   // Si no, retorna c
}


void freeHouses(HouseNode *head) {
  while (head != NULL) {    // Mentre la llista no estigui buida
    HouseNode *temp = head; // Guardem el node actual
    head = head->next;      // Avancem el punter al següent node
    free(temp);             // Alliberem la memòria del node guardat
  }
}
