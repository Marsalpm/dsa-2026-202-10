#include "intersection.map.h"
#include "streets.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Converteix un intersectionId (molt gran) en un índex petit dins
int hash(long long id) { return id % TABLE_SIZE; }

// Crea el hashmap
HashMap *createHashMap() {

  HashMap *map = malloc(sizeof(HashMap));

  // Si no hi ha memòria, retornem NULL
  if (map == NULL)
    return NULL;

  // Inicialitzem totes les posicions del hashmap a buit
  for (int i = 0; i < TABLE_SIZE; i++) {
    map->table[i] = NULL;
  }

  return map;
}

void insertStreet(HashMap *map, Street s) {

  // Calculem en quina posició anirà aquest id
  long long ids[2] = {s.id1, s.id2};

  // Fem el procés dues vegades:
  // una per id1 i una per id2

  for (int i = 0; i < 2; i++) {

    long long currentId = ids[i];

    // Calculem la posició
    int index = hash(currentId);

    // Busquem si aquesta intersecció ja existeix
    HashEntry *entry = map->table[index];

    while (entry != NULL && entry->intersectionId != currentId) {
      entry = entry->next;
    }

    // Si no existeix cap entrada, la creem
    if (entry == NULL) {

      entry = malloc(sizeof(HashEntry));

      entry->intersectionId = currentId;
      entry->streets = NULL;

      // Inserim al principi de la llista de col·lisions
      entry->next = map->table[index];
      map->table[index] = entry;
    }

    StreetList *newStreet = malloc(sizeof(StreetList));

    newStreet->street = s;

    newStreet->next = entry->streets;

    entry->streets = newStreet;
  }
}

HashMap *buildGraph(StreetNode *streets) {
  // Creem el hashmap buit
  HashMap *map = createHashMap();

  StreetNode *current = streets;
  // Recorrem tots els carrers
  while (current != NULL) {
    // Inserim el carrer al hashmap
    insertStreet(map, current->street);

    current = current->next;
  }

  return map;
}

HashEntry *findIntersection(HashMap *map, long long id) {
  // Calculem la posició
  int index = hash(id);

  HashEntry *entry = map->table[index];
  // Recorrem la linked list
  while (entry != NULL) {
    // Si trobem la intersecció
    if (entry->intersectionId == id) {
      return entry;
    }

    entry = entry->next;
  }

  return NULL;
}

void printConnections(HashMap *map, long long id) {
  // Busquem la intersecció
  HashEntry *entry = findIntersection(map, id);
  // Si no existeix, acabem
  if (entry == NULL)
    return;

  StreetList *current = entry->streets;
  // Recorrem tots els carrers
  while (current != NULL) {
    // Imprimim el nom del carrer
    printf("- %s\n", current->street.name);

    current = current->next;
  }
}

void freeHashMap(HashMap *map) {

  // Recorrem tota la taula hash
  for (int i = 0; i < TABLE_SIZE; i++) {

    HashEntry *entry = map->table[i];

    // Recorrem la linked list de col·lisions
    while (entry != NULL) {

      // Alliberar la llista de carrers
      StreetList *street = entry->streets;

      while (street != NULL) {

        StreetList *tempStreet = street;

        street = street->next;

        free(tempStreet);
      }

      // Guardem el següent HashEntry
      HashEntry *tempEntry = entry;

      entry = entry->next;

      // Alliberem el HashEntry actual
      free(tempEntry);
    }
  }
  // alliberem el hashmap
  free(map);
}
