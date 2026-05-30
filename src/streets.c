#include "streets.h"
#include "intersection.map.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EARTH_RADIUS 6371.0

StreetNode *loadStreets(char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    printf("Could not open streets.txt\n");
    return NULL;
  }

  StreetNode *head = NULL;
  char line[512];

  while (fgets(line, sizeof(line), file)) {
    Street s;
    if (sscanf(line, "%lld,%lf,%lf,%lld,%lf,%lf,%lf, %99[^\n]", &s.id1, &s.lat1,
               &s.lon1, &s.id2, &s.lat2, &s.lon2, &s.length, s.name) == 8) {
      if (s.name[0] == ' ') {
        memmove(s.name, s.name + 1, strlen(s.name) + 1);
      }
      s.name[strcspn(s.name, "\r\n")] = '\0';
      head = addStreet(head, s);
    }
  }

  fclose(file);
  return head;
}

StreetNode *addStreet(StreetNode *head, Street s) { // Afegir nodes a la llista
  StreetNode *newNode = malloc(sizeof(StreetNode));
  if (!newNode)
    return head;
  newNode->street = s;
  newNode->next = NULL;
  if (head == NULL) {
    return newNode;
  }
  StreetNode *current = head;
  while (current->next != NULL) {
    current = current->next;
  }
  current->next = newNode;
  return head;
}

void freeStreets(StreetNode *head) { // Alliberem memoria
  while (head != NULL) {
    StreetNode *temp = head;
    head = head->next;
    free(temp);
  }
}

double toRadians(double degree) { return degree * M_PI / 180.0; }

double haversine(double lat1, double lon1, double lat2, double lon2) {
  double dLat = toRadians(lat2 - lat1);
  double dLon = toRadians(lon2 - lon1);
  lat1 = toRadians(lat1);
  lat2 = toRadians(lat2);

  double a = sin(dLat / 2) * sin(dLat / 2) +
             sin(dLon / 2) * sin(dLon / 2) * cos(lat1) * cos(lat2);
  double c = 2 * atan2(sqrt(a), sqrt(1 - a));

  return EARTH_RADIUS * c;
}

StreetNode *findClosestStreet(StreetNode *head, double userLat,
                              double userLon) {

  StreetNode *current = head;
  StreetNode *closest = NULL;
  double minDistance = INFINITY; // Distancia infinita inicialment

  while (current != NULL) {
    double midLat = (current->street.lat1 + current->street.lat2) /
                    2.0; // Calculem punt mitjà del segment de carrer
    double midLon = (current->street.lon1 + current->street.lon2) / 2.0;
    double distance =
        haversine(userLat, userLon, midLat,
                  midLon); // Calculem la distancia de la persona al punt mitja

    if (distance <=
        minDistance) { // Si es mes petit que el que teniem, actualitzem
      minDistance = distance;
      closest = current;
    }

    current = current->next;
  }
  return closest;
}

void printConnectedStreets(StreetNode *head, Street target) {
  StreetNode *current = head;
  char printed[100][100];
  int printedCount = 0;

  printf("- %s\n", target.name);

  printf("    Which is connected to:\n");
  while (current != NULL) {
    Street s = current->street;
    bool connected = false;
    if (s.id1 == target.id1 || s.id1 == target.id2 || s.id2 == target.id1 ||
        s.id2 == target.id2)
      connected = true;
    if (connected && (s.id1 != target.id1 || s.id2 != target.id2)) {
      bool alreadyPrinted = false;
      for (int i = 0; i < printedCount; i++) {
        if (strcmp(printed[i], s.name) == 0) {
          alreadyPrinted = true;
          break;
        }
      }

      if (!alreadyPrinted) {
        printf("     - %s\n", s.name);
        strcpy(printed[printedCount], s.name);
        printedCount++;
      }
    }
    current = current->next;
  }
}

int printConnectedStreetsFast(HashMap *map, Street target) {
  int printedCount = 0;
  char printed[100][100];
  printf("- %s\n", target.name);

  long long currentId =
      target.id2; // comencem per on acaba el segment més proper

  while (true) { // bucle infinit que sortim manualment amb break
    HashEntry *entry = findIntersection(
        map, currentId); //  busquem la intersecció actual al hashmap
    if (entry == NULL)
      break; // si no existeix, sortim

    int found_next = 0; // variable per saber si hem trobat el segment següent
    // recorrem tots els carrers de la intersecci
    StreetList *StreetList = entry->streets;
    while (StreetList != NULL) {
      if (strcmp(StreetList->street.name, target.name) != 0) {
        bool alreadyPrinted = false;
        for (int j = 0; j < printedCount; j++) {
          if (strcmp(printed[j], StreetList->street.name) == 0) {
            alreadyPrinted = true;
            break;
          }
        }
        if (!alreadyPrinted) {
          if (printedCount == 0)
            printf("    Which is connected to:\n");
          printf("     - %s\n", StreetList->street.name);
          strcpy(printed[printedCount], StreetList->street.name);
          printedCount++;
          return printedCount;
        }
      } else {
        if (StreetList->street.id1 == currentId &&
            StreetList->street.id2 != target.id1) {
          currentId = StreetList->street.id2;
          found_next = 1;
          break;
        }
      }
      StreetList = StreetList->next;
    }
    if (!found_next)
      break; // si no hem trobat continuació del carrer, sortim del bucle
  }
  if (printedCount == 0)
    printf("    No connections found\n");
  return printedCount;
}

void processLocation(double lat, double lon, StreetNode *streets,
                     int showConnection) {
  int count = 0;
  StreetNode *tmp = streets;
  while (tmp != NULL) {
    count++;
    tmp = tmp->next;
  }
  StreetNode *closest = findClosestStreet(streets, lat, lon);

  if (closest == NULL)
    return;

  printf("Closest street: %s\n", closest->street.name);

  printf("Between %lld (%f, %f) and %lld (%f, %f)\n", closest->street.id1,
         closest->street.lat1, closest->street.lon1, closest->street.id2,
         closest->street.lat2, closest->street.lon2);
  if (showConnection == 1) {
    printf("\nFrom this street segment, you can go to:\n");

    HashMap *graph = buildGraph(streets);
    if (graph != NULL) {
      int count = printConnectedStreetsFast(graph, closest->street);
      if (count == 0)
        printf("    No connections found\n");
      freeHashMap(graph);
    } else {
      // versió original si no hi ha memòria
      printConnectedStreets(streets, closest->street);
    }
  }
}
