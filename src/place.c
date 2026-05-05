#include "place.h"
#include "houses.h"
#include "../test/utils.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

PlaceNode *addPlace(PlaceNode *head, Place p) {
  PlaceNode *newNode = malloc(sizeof(PlaceNode));
  newNode->place = p;
  newNode->next = head;
  return newNode;
}

PlaceNode *loadPlaces(char *filename) {
  FILE *file = fopen(filename, "r");

  if (file == NULL) { // Comprovem si el fitxer s'ha obert correctament
    printf("No s'ha pogut obrir el fitxer\n");
    return NULL; // Si no existeix, retornem NULL
  }

  PlaceNode *head = NULL;
  char line[256]; // Buffer per guardar cada línia llegida del fitxer

  while (fgets(line, sizeof(line),
               file)) { // Llegim el fitxer línia a línia fins al final
    Place p;            // Creem una estructura temporal per a la casa
    // Parsegem la línia separada per punts i coma i guardem les dades
    if (sscanf(line, "%*[^,],%[^,],%*[^,],%lf,%lf", p.name, &p.lat, &p.lon) ==
        3) {
      head = addPlace(head, p); // Afegim el lloc parsejat a la llista
    }
  }

  fclose(file); // Tanquem el fitxer per alliberar recursos
  return head;  // Retornem la llista enllaçada completa
}

PlaceNode *findPlace(PlaceNode *head, char *name) {
  PlaceNode *current = head; // Comencem la cerca pel principi de la llista
  while (current != NULL) {  // Mentres hi hagi carrer a la llista i no haguem
                             // trobat el carrer, seguirem buscant
    if (strcasecmp(current->place.name, name) == 0) {
      return current; // Si coincideixen, hem trobat el lloc i el retornem
    }
    current = current->next; // Si no, avancem al següent node de la llista
  }

  return NULL; // Si acabem el bucle sense trobar res, retornem NULL
}

void cerca_inteligent_places(PlaceNode *head, char *name) {
  PlaceNode *resultat = findPlace(head, name);
  // Cerca exacta
  if (resultat != NULL) {
    printf("Trobat a (%f, %f)\n", resultat->place.lat, resultat->place.lon);
    return;
  }
  // Cerca d'opcions similars
  printf("Lloc '%s' no trobat. Volies dir:\n", name);

 // char sug1[100] = "", sug2[100] = "", sug3[100] = "";
  //int d1 = 100, d2 = 100, d3 = 100;

  Leve *sugList = NULL;
  PlaceNode *temp = head;

  while (temp != NULL) {   
    int d = levenshteinDistance(name, temp->place.name);  // Trobem la distància de Levenshtein del place actual
    sugList = addSugestion(sugList, temp->place.name, d);
    temp = temp->next;
  }

  Leve *aux = sugList;
  int count = 0;
  while(aux && count<3){
    printf("%d. %s\n", count +1, aux->name);
    aux = aux->next;
    count++;
  }

  if (count > 0){
    int opcio;
    printf("Escull una opcio (0 per sortir): ");
    if (scanf("%d", &opcio) ==1 && opcio > 0 && opcio<=count){
      aux = sugList;
      for(int j = 1; j < opcio; j++) aux = aux->next;
      PlaceNode *final_res = findPlace (head, aux->name);
      if(final_res != NULL) printf("Tribat a (%f. %f)\n", final_res->place.lat, final_res->place.lon);
    }else{
      printf("NO s'ha trobat\n");
    }
    freeSugestion(sugList);
  }

}
  /*/}
    // Evitar duplicats
    if (strcmp(temp->place.name, sug1) == 0 ||
        strcmp(temp->place.name, sug2) == 0 ||
        strcmp(temp->place.name, sug3) == 0) {
      temp = temp->next;
      continue;
    }
    // Si el place actual té una distància de Levenshtein més petita que alguna
    // de les opcions es fa intercanvi, sempre ordenat de més petita a més gran
    if (d < d1) {
      d3 = d2;
      strcpy(sug3, sug2);
      d2 = d1;
      strcpy(sug2, sug1);
      d1 = d;
      strcpy(sug1, temp->place.name);
    } else if (d < d2) {
      d3 = d2;
      strcpy(sug3, sug2);
      d2 = d;
      strcpy(sug2, temp->place.name);
    } else if (d < d3) {
      d3 = d;
      strcpy(sug3, temp->place.name);
    }

    temp = temp->next;
  

  int opcions = 0;
  // Imprimir les opcions si aquestes tenen una distància de Levenshtein més
  // petita que 100
  if (d1 < 100) {
    printf("1. %s\n", sug1);
    opcions++;
  if (d2 < 100) {
    printf("2. %s\n", sug2);
    opcions++;
  }
  if (d3 < 100) {
    printf("3. %s\n", sug3);
    opcions++;
  }
   printf("4. Exit (0)\n");
  }
  // Si no s'ha imprès ninguna vol dir que no s'ha trobat ningún carrer similar,
  // per tant ho imprimim
  if (opcions == 0) {
    printf("No s'ha trobat cap lloc similar.\n");
    return;
  }

  if (opcions > 0) {
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
      if (opcio==0) return;
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
    PlaceNode *final_res = findPlace(head, final);

    if (final_res != NULL) {
      printf("Trobat a (%f, %f)\n", final_res->place.lat, final_res->place.lon);
    } else {
      printf("Error inesperat.\n");
    }
  }
}*/

void freePlaces(PlaceNode *head) {
  while (head != NULL) {    // Mentre la llista no estigui buida
    PlaceNode *temp = head; // Guardem el node actual
    head = head->next;      // Avancem el punter al següent node
    free(temp);             // Alliberem la memòria del node guardat
  }
}