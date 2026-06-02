#ifndef BFS_H
#define BFS_H
#define EARTH_RADIUS 6371.0
#include "intersection.map.h"
#include "streets.h"
typedef struct Street Street;

typedef struct PathNode {
  Street street;
  struct PathNode *next;
} PathNode;

typedef struct QueueItem {
  PathNode *path;
  long long current_intersection;
  struct QueueItem *next;
} QueueItem;

typedef struct Queue {
  QueueItem *front;
  QueueItem *back;
} Queue;

typedef struct VisitedNode {
  long long id1;
  long long id2;
  struct VisitedNode *next;
} VisitedNode;

// Hash Set per als segments visitats (millora de rendiment)
#define VISITED_TABLE_SIZE 10007

// Node dins d'un bucket del hash set de visitats
typedef struct VisitedEntry {
  long long id1;
  long long id2;
  struct VisitedEntry *next;
} VisitedEntry;

// Hash Set per als segments visitats
typedef struct VisitedSet {
  VisitedEntry *table[VISITED_TABLE_SIZE];
} VisitedSet;

Queue *createQueue();
int queueisEmpty(Queue *queue);
void enqueue(Queue *q, PathNode *p, long long current_intersection);
PathNode *dequeue(Queue *q, long long *current_intersection);
void queuefree(Queue *q);
void pathfree(PathNode *p);
PathNode *pathCopy(PathNode *p);
PathNode *pathAppend(PathNode *p, Street street);
PathNode *bfs(HashMap *graph, Street fromStreet, Street toStreet);
VisitedSet *createVisitedSet();
void freeVisitedSet(VisitedSet *set);
void printRoute(PathNode *path);
void latlon_to_xy(double lat_ref, double lon_ref, double lat, double lon,
                  double *x, double *y);
StreetNode *findConnectedLinear(StreetNode *streets, long long intersection);
PathNode *bfs_slow(StreetNode *streets, Street fromStreet, Street toStreet);
int sameSegment(Street a, Street b);
int isVisitedDirection(VisitedSet *set, Street street,
                       long long exit_intersection);
void addVisitedDirection(VisitedSet *set, Street street,
                         long long exit_intersection);
int visitedHash(long long id1, long long id2);
#endif