#ifndef PLACE_H
#define PLACE_H
typedef struct StreetNode StreetNode;
typedef struct Place {
  char name[100];
  double lat;
  double lon;
} Place;

typedef struct PlaceNode {
  Place place;
  struct PlaceNode *next;
} PlaceNode;

PlaceNode *addPlace(PlaceNode *head, Place p);
PlaceNode *loadPlaces(char *filename);
PlaceNode *findPlace(PlaceNode *head, char *name);
void cerca_inteligent_places(PlaceNode *head, char *name, StreetNode *streets, double *lat, double *lon);
void freePlaces(PlaceNode *head);
#endif
