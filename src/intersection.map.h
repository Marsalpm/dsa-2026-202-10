#ifndef INTERSECTION.MAP_H
#define INTERSECTION.MAP_H
#include "streets.h"
#define TABLE_SIZE 10007

typedef struct SegmentNode {
    Street street;
    struct SegmentNode *next;
} SegmentNode;

typedef struct HashEntry {
    long long intersectionId;

    SegmentNode *streets;

    struct HashEntry *next;

} HashEntry;

int hash(long long id);
HashMap *createHashMap();
void insertStreet(HashMap *map, Street s);
#endif