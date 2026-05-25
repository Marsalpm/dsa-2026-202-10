#include <stdio.h>
#include "bfs.h"
#include "intersection.map.h"

Queue* createQueue(){
    Queue *queue = malloc(sizeof(Queue));
    //Si no hi ha memoria 
    if(queue==NULL) return NULL;
    queue->back = NULL;
    queue->front = NULL;
    return queue;
}

int queueisEmpty(Queue *q){
    if (q->back ==NULL && q->front == NULL) return 1;
    return 0;
}

void enqueue(Queue *q, PathNode *p){
    QueueItem *a = malloc(sizeof(QueueItem));
    a->path = p;
    a->next = NULL;
    if(queueisEmpty(q)==1){
        q->back=a, 
        q->front=a;
    }
    else{
        q->back->next= a;
        q->back = a;
    }
}

PathNode* dequeue(Queue *q){
    if(queueisEmpty(q)==1) return NULL;
    QueueItem *temp = q->front;
    PathNode *path = temp->path;
    q->front = temp->next;
    if(q->front==NULL) q->back=NULL;
    free(temp);
    return path;
}

void queuefree(Queue *q){
    while(queueisEmpty(q)==0){
        pathfree(dequeue(q));
    }
    return;
}

void pathfree(PathNode *p){
    while(p!=NULL){
        PathNode *temp = p;
        p=p->next;
        free(temp);
    }
}

PathNode* pathAppend(PathNode *p, Street street){
    PathNode *copy = pathCopy(p);
    PathNode *head = copy;
    while(copy->next=NULL){
        copy = copy->next;
    }
    PathNode *new = malloc(sizeof(PathNode));
    new->street = street;
    new->next = NULL;
    copy->next = new;
    return head;
}


PathNode* pathCopy(PathNode *p){
    if(p==NULL)return NULL;
    PathNode *new = malloc(sizeof(PathNode));
    new->street = p->street;
    PathNode *head = new;
    p = p->next;
    while(p!=NULL){
        new->next = malloc(sizeof(PathNode));
        new = new->next;
        new->street = p->street;
        p = p->next;
    }
    new->next = NULL;
    return head;
}

PathNode *bfs(HashMap *graph, Street fromStreet, Street toStreet){
    Queue *q = createQueue();
    PathNode *initial = malloc(sizeof(PathNode));
    initial->street = fromStreet;
    initial->next = NULL;
    enqueue(q, initial);
    VisitedNode *visited = NULL;
    while (queueisEmpty(q)!=1){
        PathNode *current = dequeue(q);
        PathNode *temp = current;
        while(temp->next!=NULL){
            temp = temp->next;
        }
        if(toStreet.id1==temp->street.id1 && toStreet.id2==temp->street.id2){
        freeVisited(visited);
        queueFree(q);
        return current;
        }
        if(isVisited(visited,temp->street.id1,temp->street.id2)==0){
            visited=addVisited(visited,temp->street.id1,temp->street.id2);
            HashEntry *entry = findIntersection(graph, temp->street.id2);
            if(entry != NULL){
            StreetList *connected = entry->streets;
            while(connected != NULL){
                if(isVisited(visited, connected->street.id1, connected->street.id2)==0){
                    PathNode *newPath = pathAppend(current, connected->street);
                    enqueue(q, newPath);
                }
                connected = connected->next;
            }
            }
        }
    }
    freeVisited(visited);
    queuefree(q);
    return NULL;
}

int isVisited(VisitedNode *visited, long long id1, long long id2){
    while (visited!=NULL){
        if(visited->id1==id1 && visited->id2==id2)return 1;
        visited = visited->next;
    }
    return 0;
}

VisitedNode* addVisited(VisitedNode *visited, long long id1, long long id2){
    VisitedNode *new = malloc(sizeof(VisitedNode));
    new->id1 = id1;
    new->id2 = id2;
    new->next = visited;
    return new;
}

void freeVisited(VisitedNode *visited){
    while(visited!=NULL){
        VisitedNode *temp = visited;
        visited = visited->next;
        free(temp);
    }

}

void latlon_to_xy(double lat_ref, double lon_ref,
                  double lat, double lon,
                  double *x, double *y) {
    double lat_ref_rad = toRadians(lat_ref);
    double dlat = toRadians(lat - lat_ref);
    double dlon = toRadians(lon - lon_ref);
    *x = EARTH_RADIUS * dlon * cos(lat_ref_rad);
    *y = EARTH_RADIUS * dlat;
}

void printRoute(PathNode *path){
    printf("Start at %s\n", path->street.name);
    PathNode *current = path;
    while(current->next != NULL){
        PathNode *prev = current;
        current = current->next;
        double ax, ay, bx, by, cx, cy;
        latlon_to_xy(prev->street.lat1, prev->street.lon1, prev->street.lat1, prev->street.lon1, &ax, &ay);
        latlon_to_xy(prev->street.lat1, prev->street.lon1, prev->street.lat2, prev->street.lon2, &bx, &by);
        latlon_to_xy(prev->street.lat1, prev->street.lon1, current->street.lat2, current->street.lon2, &cx, &cy);
        double cross  = (bx-ax)*(cy-by) - (by-ay)*(cx-bx);
        if(cross > 0) printf("Turn left to %s and continue for %dm\n", current->street.name, (int)current->street.length);
        else printf("Turn right to %s and continue for %dm\n", current->street.name, (int)current->street.length);
        
        
    }
    printf("You have arrived to %s\n", current->street.name);
}