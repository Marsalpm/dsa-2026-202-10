#include "intersection.map.h"
#include "streets.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Converteix un intersectionId (molt gran) en un índex petit dins la taula hash
int hash(long long id) { return id % TABLE_SIZE; }

// Crea i inicialitza el hashmap
HashMap *createHashMap() {

  HashMap *map = malloc(sizeof(HashMap)); // Reservem memòria pel mapa

  // Si no hi ha memòria, retornem NULL
  if (map == NULL)
    return NULL;

  // Inicialitzem totes les posicions del hashmap a buit (NULL)
  for (int i = 0; i < TABLE_SIZE; i++) {
    map->table[i] = NULL;
  }

  return map; // Retornem el mapa inicialitzat
}

// Insereix un carrer al hashmap usant les seves dues interseccions
void insertStreet(HashMap *map, Street s) {
  // Guardem els dos extrems (interseccions) del carrer
  long long ids[2] = {s.id1, s.id2};

  // Fem el procés dues vegades:
  // una per id1 i una per id2
  for (int i = 0; i < 2; i++) {

    long long currentId = ids[i]; // Agafem l'ID de la intersecció actual

    // Calculem la posició a la taula hash per aquest ID
    int index = hash(currentId);

    // Busquem si aquesta intersecció ja existeix a la taula
    HashEntry *entry = map->table[index];

    // Recorrem la llista de col·lisions fins a trobar-la o arribar al final
    while (entry != NULL && entry->intersectionId != currentId) {
      entry = entry->next;
    }

    // Si no existeix cap entrada per aquesta intersecció, la creem
    if (entry == NULL) {

      entry = malloc(sizeof(HashEntry)); // Reservem memòria per la nova entrada

      entry->intersectionId = currentId; // Li assignem l'ID
      entry->streets = NULL;             // Inicialitzem la llista de carrers a buit

      // Inserim la nova entrada al principi de la llista de col·lisions
      entry->next = map->table[index];
      map->table[index] = entry;
    }

// Creem un nou node per afegir el carrer
StreetList *newStreet = malloc(sizeof(StreetList));
newStreet->street = s;
newStreet->next = NULL;

// Si no hi ha cap carrer encara, serà el primer
if (entry->streets == NULL) {

    entry->streets = newStreet;
}
else {

    // Busquem l'últim carrer de la llista
    StreetList *currentStreet = entry->streets;

    while (currentStreet->next != NULL) {
        currentStreet = currentStreet->next;
    }

    // Afegim el nou carrer al final
    currentStreet->next = newStreet;
}
  }
}

// Construeix el graf sencer a partir d'una llista de carrers
HashMap *buildGraph(StreetNode *streets) {
    HashMap *map = createHashMap(); // Creem un mapa buit
    StreetNode *current = streets;  // Punter temporal per recórrer els carrers
    int count = 0;                  // Comptador (opcional)
    
    // Recorrem tota la llista de carrers
    while (current != NULL) {
        insertStreet(map, current->street); // Afegim cada carrer al graf
        count++;                            // Incrementem el comptador
        current = current->next;            // Passem al següent carrer
    }
    return map; // Retornem el graf construït
}

// Cerca una intersecció específica pel seu ID
HashEntry *findIntersection(HashMap *map, long long id) {
  // Calculem la posició on hauria de ser
  int index = hash(id);
  HashEntry *entry = map->table[index]; // Agafem el primer element d'aquella posició
  
  // Recorrem la linked list de col·lisions
  while (entry != NULL) {
    // Si trobem la intersecció correcta, la retornem
    if (entry->intersectionId == id) {
      return entry;
    }

    entry = entry->next; // Si no, avancem a la següent
  }

  return NULL; // Retornem NULL si no l'hem trobat
}

// Imprimeix els noms de tots els carrers connectats a una intersecció
void printConnections(HashMap *map, long long id) {
  // Busquem la intersecció pel seu ID
  HashEntry *entry = findIntersection(map, id);
  // Si no existeix, sortim de la funció
  if (entry == NULL)
    return;

  StreetList *current = entry->streets; // Agafem el primer carrer connectat
  // Recorrem tots els carrers de la llista
  while (current != NULL) {
    // Imprimim el nom del carrer
    printf("- %s\n", current->street.name);

    current = current->next; // Avancem al següent carrer
  }
}

// Allibera tota la memòria que ocupa el hashmap
void freeHashMap(HashMap *map) {

  // Recorrem tota la taula hash (cada posició de l'array)
  for (int i = 0; i < TABLE_SIZE; i++) {

    HashEntry *entry = map->table[i];

    // Recorrem la linked list de col·lisions de cada posició
    while (entry != NULL) {

      // Agafem la llista de carrers d'aquesta intersecció
      StreetList *street = entry->streets;

      // Recorrem la llista de carrers per alliberar-la node a node
      while (street != NULL) {

        StreetList *tempStreet = street; // Guardem el node actual

        street = street->next; // Avancem al següent node

        free(tempStreet); // Alliberem el node guardat
      }

      // Guardem el següent HashEntry abans d'esborrar l'actual
      HashEntry *tempEntry = entry;

      entry = entry->next; // Avancem al següent HashEntry

      // Alliberem el HashEntry actual
      free(tempEntry);
    }
  }
  // Finalment, alliberem l'estructura general del hashmap
  free(map);
}
