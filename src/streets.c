#include "streets.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define EARTH_RADIUS 6371.0

StreetNode *loadStreets(char *filename) {

  FILE *file = fopen(filename, "r");

  if (file == NULL) {

    printf("No s'ha pogut obrir streets.txt\n");

    return NULL;
  }

  StreetNode *head = NULL;

  char line[512];

  while (fgets(line, sizeof(line), file)) {

    Street s;

    if (sscanf(line, "%lld,%lf,%lf,%lld,%lf,%lf,%lf,%99[^\n]", &s.id1, &s.lat1,
               &s.lon1, &s.id2, &s.lat2, &s.lon2, &s.length, s.name) == 8) {
      head = addStreet(head, s);
    }
  }

  fclose(file);
  return head;
}

StreetNode *addStreet(StreetNode *head, Street s) {
  StreetNode *newNode = malloc(sizeof(StreetNode));

  if (!newNode)
    return head;

  newNode->street = s;
  newNode->next = head;

  return newNode;
}

void freeStreets(StreetNode *head) {

  while (head != NULL) {

    StreetNode *temp = head;

    head = head->next;

    free(temp);
  }
}

double toRadians(double degree) {
     return degree * M_PI / 180.0; 
    }

double haversine(double lat1, double lon1, double lat2, double lon2) {
  double dLat = toRadians(lat2 - lat1);
  double dLon = toRadians(lon2 - lon1);
  lat1 = toRadians(lat1);
  lat2 = toRadians(lat2);

  double a = sin(dLat / 2) * sin(dLat / 2) + sin(dLon / 2) * sin(dLon / 2) * cos(lat1) * cos(lat2);
  double c = 2 * atan2(sqrt(a), sqrt(1 - a));

  return EARTH_RADIUS * c;
}

StreetNode *findClosestStreet(StreetNode *head, double userLat,
                              double userLon) {

  StreetNode *current = head;
  StreetNode *closest = NULL;
  double minDistance = 10000;
                            
  while (current != NULL) {
    double midLat = (current->street.lat1 + current->street.lat2) / 2.0;

    double midLon = (current->street.lon1 + current->street.lon2) / 2.0;
    double distance = haversine(userLat, userLon, midLat, midLon);
    if (distance < minDistance) {

      minDistance = distance;
      closest = current;
    }

    current = current->next;
  }

  return closest;
}

void printConnectedStreets(StreetNode *head, Street target) {
  StreetNode *current = head;


  printf("- %s\n", target.name);

  printf("    Which is connected to:\n");
  while (current != NULL) {

    Street s = current->street;

    bool connected = false;

    if (s.id1 == target.id1 || s.id1 == target.id2 || s.id2 == target.id1 ||
        s.id2 == target.id2) {
      connected = true;
    }
    if (connected && !(s.id1 == target.id1 &&
                       s.id2 == target.id2)) { // evitar carrers duplicats
      printf("     - %s\n", s.name);
    }

    current = current->next;
  }
}

void processLocation(double lat, double lon, StreetNode *streets) {
  StreetNode *closest = findClosestStreet(streets, lat, lon);

  if (closest == NULL)
    return;

  printf("\nClosest street: %s\n", closest->street.name);

  printf("Between %lld (%f, %f) and %lld (%f, %f)\n", closest->street.id1,
         closest->street.lat1, closest->street.lon1, closest->street.id2,
         closest->street.lat2, closest->street.lon2);

  printf("\nFrom this street segment, you can go to:\n");

  printConnectedStreets(streets, closest->street);
}
