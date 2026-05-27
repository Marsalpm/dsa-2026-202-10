#ifndef STREETS_H
#define STREETS_H
typedef struct HashMap HashMap;
typedef struct Street {

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
void processLocation(double lat, double lon, StreetNode *streets,int showConnection);
double toRadians(double degrees);
StreetNode *findClosestStreet(StreetNode *head, double userLat, double userLon);
void printConnectedStreets(StreetNode *head, Street target);
int printConnectedStreetsFast(HashMap *map, Street target);
#endif