#ifndef PLACE_H
#define PLACE_H
#include "houses.h"

typedef struct Place {
    char name[100];
    double lat;
    double lon;
} Place;

typedef struct PlaceNode {
    Place place;
    struct PlaceNode *next;
} PlaceNode;

PlaceNode* addPlace(PlaceNode *head, Place p);
PlaceNode* loadPlaces(char *filename);
PlaceNode* findPlace(PlaceNode *head, char *name);

#endif

