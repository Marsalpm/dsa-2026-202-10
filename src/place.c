#include "place.h"
#include "../test/utils.h"
#include "houses.h"
#include "streets.h"
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
    printf("Could not open the file\n");
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

int cerca_inteligent_places(PlaceNode *head, char *name, StreetNode *streets, double *lat, double *lon, int showConnection) {
  PlaceNode *resultat = findPlace(head, name);
  // Cerca exacta
  if (resultat != NULL) {
    printf("\nFound at (%f, %f)\n", resultat->place.lat, resultat->place.lon);
    processLocation(resultat->place.lat, resultat->place.lon, streets, showConnection);
    *lat=resultat->place.lat;
    *lon=resultat->place.lon;
    return 1;
  }
  // Cerca d'opcions similars
  printf("Place '%s' not found. Did you mean:\n", name);

  Leve *sugList = NULL;
  PlaceNode *temp = head;

  while (temp != NULL) {
    int d = levenshteinDistance(
        name, temp->place
                  .name); // Trobem la distància de Levenshtein del place actual
    sugList = addSugestion(sugList, temp->place.name, d);
    temp = temp->next;
  }

  Leve *aux = sugList;
  int count = 0;
  while (aux && count < 3) {
    printf("%d. %s\n", count + 1, aux->name);
    aux = aux->next;
    count++;
  }
  if (count == 0) {
    printf("No similar options found\n");
    return 0;
  }
  printf("0. Exit\n");

  int opcio;
  printf("Choose an option: ");
  int valid_number = scanf("%d", &opcio);
  while (valid_number != 1 || opcio < 0 || opcio > count) {
    while (getchar() != '\n')
      ;
    printf("Please enter a valid number: ");
    valid_number = scanf("%d", &opcio);
    if (0 < opcio && opcio <= count) {
      aux = sugList;
      for (int j = 1; j < opcio; j++)
        aux = aux->next;
      PlaceNode *final_res = findPlace(head, aux->name);
      if (final_res != NULL) {
        printf("\nFound at (%f, %f)\n", final_res->place.lat,final_res->place.lon);
        *lat=final_res->place.lat;
        *lon=final_res->place.lon;
        processLocation(final_res->place.lat, final_res->place.lon, streets, showConnection);
        return 1;
      }
    }
  }
  if (opcio == 0) {
    freeSugestion(sugList);
    return 0;
  } else if (0 < opcio && opcio <= count) {
    aux = sugList;
    for (int j = 1; j < opcio; j++)
      aux = aux->next;
    PlaceNode *final_res = findPlace(head, aux->name);
    if (final_res != NULL) {
      printf("\nFound at (%f, %f)\n", final_res->place.lat, final_res->place.lon);
      *lat=final_res->place.lat;
      *lon=final_res->place.lon;
      processLocation(final_res->place.lat, final_res->place.lon, streets, showConnection);
    }
  }
  freeSugestion(sugList);
  return 1;
}

void freePlaces(PlaceNode *head) {
  while (head != NULL) {    // Mentre la llista no estigui buida
    PlaceNode *temp = head; // Guardem el node actual
    head = head->next;      // Avancem el punter al següent node
    free(temp);             // Alliberem la memòria del node guardat
  }
}