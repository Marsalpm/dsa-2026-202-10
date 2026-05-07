#include "houses.h"
#include "../test/utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // Incloem per poder usar strcasecmp i strncasecmp

HouseNode *addHouse(HouseNode *head, House house) {
  HouseNode *newNode = malloc(sizeof(HouseNode)); // Creem nou node i reservem memoria
  if (!newNode) return head;  // Si falla la reserva, retornem la llista actual
  newNode->house = house;     // Guardem la casa pasada al nou node
  newNode->next = head;       // Nou node apunta a l'antic cap (insertem al principi)

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

  while (fgets(line, sizeof(line), file)) { // Llegim el fitxer línia a línia fins al final
    House house;        // Creem una estructura temporal per a la casa
    // Parsegem la línia separada per punts i coma i guardem les dades
    if (sscanf(line, " %99[^,] ,%d ,%lf ,%lf", house.street, &house.number, &house.lat, &house.lon) == 4) {
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
    if (strcmp(currentStreet, inputstreet) == 0 && current->house.number == number) {
      return current; // Si coincideixen, hem trobat la casa i la retornem
    }
    current = current->next; // Si no, avancem al següent node de la llista
  }
  return NULL; // Si acabem el bucle sense trobar res, retornem NULL
}
// Fucnions auxilars:
void printCoordinates(House house) {
  printf("Trobat a (%f, %f)\n", house.lat, house.lon); // Mostra les coordenades per pantalla
}

void printfSetreetSegment(House house){
    printf("Segment de carrer: Intersecció %d i Intersecció %d\n",house.id1,house.id2);
}

// Cerca un carrer gestionant abreviatures, números erronis i similituds
// FUNCIO DE CERCA INTELIGENT LAB 2 I LAB 3:
void cerca_inteligent_houses(HouseNode *head, char *street, int number) {
  char final[100]= "";
  char street_net[100]; // Variable per guardar el nom del carrer "netejat"

  // Si comença per "C. ", ho expandim a "Carrer "
  if (strncasecmp(street, "C. ", 3) == 0) {
    sprintf(street_net, "Carrer %s", street + 3); // Saltem els 3 primers caràcters
  }
  // Si comença per "Av. ", ho expandim a "Avinguda "
  else if (strncasecmp(street, "Av. ", 4) == 0) {
    sprintf(street_net, "Avinguda %s", street + 4); // Saltem els 4 primers caràcters
  } else {
    strcpy(street_net, street); // Si no hi ha abreviatura, copiem el nom tal qual
  }

  // Busquem si la casa exacta existeix amb el nom netejat
  HouseNode *resultat = findHouse(head, street_net, number);
  if (resultat != NULL) {
    printCoordinates(resultat->house); // Si existeix, imprimim coordenades
    printfSetreetSegment(resultat->house);
    return;                            // Sortim de la funció
  } else printf("No s'ha trobat la casa exacte a %s %d\n", street_net, number);

  // Si no hi és, busquem si almenys el carrer existeix per suggerir números
  HouseNode *temp = head; // Punter temporal per recórrer la llista
  bool street_trobat = false;  // Indicador per saber si hem trobat el carrer
  while (temp != NULL) {
    // Comparem noms de carrer ignorant majúscules
    if (strcasecmp(temp->house.street, street_net) == 0) {
      street_trobat = true;// Marquem que el carrer existeix
      break; 
    }
    temp = temp->next; // Passem al següent node
    
  }
  // Si el carrer existeix però el número és incorrecte deixem triar
  if (street_trobat) {
    strcpy(final, street_net);
  }else{
    printf("Carrers semblants a %s:\n", street_net);
    Leve *sugList = NULL;
    HouseNode *temp1 = head;
    while (temp1 != NULL) {
      int d = levenshteinDistance(street_net, temp1->house.street);
      sugList = addSugestion(sugList, temp1->house.street, d);
      temp1 = temp1->next;
    }

    Leve *aux = sugList;
    int count = 0;
    while (aux && count<3){
      printf("%d. %s\n", count+1, aux->name);
      aux = aux->next;
      count++;
    }

    printf("0. Exit\n");

    int opcio;
    printf("Escull una opcio: ");
    int valid_number = scanf("%d", &opcio);
    while (valid_number != 1 || opcio<0 || opcio>count ) {
      // vaciar el buffer
      while (getchar() != '\n');
      printf("Si us plau introdueixi un nombre vàlid: ");
      valid_number = scanf("%d", &opcio);
    }
    
    if(opcio>0 && opcio<=count){
      aux = sugList;
      for(int j = 1; j < opcio; j++) aux = aux->next;
      strcpy(final, aux->name);
    }
    freeSugestion(sugList);
    if(opcio==0) return;
    }
  
  if(strlen(final)>0){
    printf("Números vàlids disponibles al carrer %s:\n", final);
    HouseNode *temp2 = head;
    int count2 = 0; 

    while (temp2 != NULL) {
      if (strcasecmp(temp2->house.street, final) == 0) {
        printf("| %-4d ", temp2->house.number);
        count2++;
        if (count2 % 5 == 0)printf("|\n");
      }
      temp2 = temp2->next;
    }

    if (count2 % 5 != 0) printf("|\n");
    printf("0. Exit\n");
    int finalnum;
    printf("Escull el número: ");
    int valid_number = scanf("%d", &finalnum);
    HouseNode *res = findHouse(head, final, finalnum);
    if (res != NULL) {
      printCoordinates(res->house);
      printfSetreetSegment(resultat->house);
      return;
    }
    if(finalnum==0) return;
    while (valid_number != 1 || res == NULL) {
      // vaciar el buffer
      while (getchar() != '\n');
      printf("Si us plau introdueixi un nombre vàlid: ");
      valid_number = scanf("%d", &finalnum);
      res = findHouse(head, final, finalnum);
      if (res != NULL) {
        printCoordinates(res->house);
        printfSetreetSegment(resultat->house);
        return;
      }
      if(valid_number == 1 && res == NULL && finalnum==0) return;
    }
  }

}


int min3(int a, int b, int c) {
  if (a <= b && a <= c) return a; // Retorna a si és el més petit
  if (b <= a && b <= c) return b; // Retorna b si és el més petit
  return c;   // Si no, retorna c
}


void freeHouses(HouseNode *head) {
  while (head != NULL) {    // Mentre la llista no estigui buida
    HouseNode *temp = head; // Guardem el node actual
    head = head->next;      // Avancem el punter al següent node
    free(temp);             // Alliberem la memòria del node guardat
  }
}
