#ifndef STREETS_H
#define STREETS_H


typedef struct {

  long long id1;

  double lat1;
  double lon1;

  long long id2;

  double lat2;
  double lon2;

  double length;

  char name[100];

} Street;

typedef struct StreetNode {
  Street street;
  struct StreetNode *next;
} StreetNode;

StreetNode *addStreet(StreetNode *head, Street s);

StreetNode *loadStreets(char *filename);

void freeStreets(StreetNode *head);

double haversine(double lat1, double lon1, double lat2, double lon2);
void printConnectedStreets(StreetNode *head, Street target);
void processLocation(double lat, double lon, StreetNode *streets);

StreetNode *findClosestStreet(StreetNode *head, double userLat, double userLon);
void printConnectedStreets(StreetNode *head, Street target);
void processLocation(double lat, double lon, StreetNode *streets);
#endif