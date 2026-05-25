#ifndef BFS_H
#define BFS_H
#define EARTH_RADIUS 6371.0
typedef struct Street Street;

typedef struct PathNode{
    Street street;
    PathNode *next;
} PathNode;

typedef struct QueueItem{
    PathNode *path;
    QueueItem *next;
} QueueItem;

typedef struct  Queue{
    QueueItem *front;
    QueueItem *back;
} Queue;

typedef struct VisitedNode {
    long long id1;
    long long id2;
    struct VisitedNode *next;
} VisitedNode;

Queue* createQueue();
int queueisEmpty(Queue *queue);
void enqueue(Queue *q, PathNode *p);
PathNode* dequeue(Queue *q);
void queuefree(Queue *q);
void pathfree(PathNode *p);
PathNode* pathCopy(PathNode *p);
PathNode* pathAppend(PathNode *p, Street street);
PathNode *bfs(HashMap *graph, Street fromStreet, Street toStreet);
int isVisited(VisitedNode *visited, long long id1, long long id2);
void freeVisited(VisitedNode *visited);
void printRoute(PathNode *path);
void latlon_to_xy(double lat_ref, double lon_ref,
double lat, double lon, double *x, double *y);
#endif