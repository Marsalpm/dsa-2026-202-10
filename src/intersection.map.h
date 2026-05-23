#ifndef INTERSECTION_MAP_H
#define INTERSECTION_MAP_H
#include "streets.h"
#define TABLE_SIZE 10007

typedef struct StreetNode StreetNode;
typedef struct Street Street;
typedef struct StreetList {
  Street street;
  struct StreetList *next;
} StreetList;

typedef struct HashEntry {
  long long intersectionId;

  StreetList *streets;

  struct HashEntry *next;

} HashEntry;

typedef struct HashMap {
  HashEntry *table[TABLE_SIZE];
} HashMap;

int hash(long long id);
HashMap *createHashMap();
void insertStreet(HashMap *map, Street s);
HashMap *buildGraph(StreetNode *streets);
HashEntry *findIntersection(HashMap *map, long long id);
void printConnections(HashMap *map, long long id);
void freeHashMap(HashMap *map);
#endif