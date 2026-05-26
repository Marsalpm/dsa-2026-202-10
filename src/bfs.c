#include <stdio.h>
#include "bfs.h"
#include "streets.h"
#include "intersection.map.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>

// Crea una cua buida i reserva memòria
Queue* createQueue(){
    Queue *queue = malloc(sizeof(Queue)); // Reservem memòria per la cua
    // Si no hi ha memòria disponible, retornem NULL
    if(queue==NULL) return NULL;
    
    // Inicialitzem el davant i el darrere de la cua a NULL
    queue->back = NULL;
    queue->front = NULL;
    
    return queue; // Retornem el punter a la nova cua
}

// Comprova si la cua està buida
int queueisEmpty(Queue *q){
    // Retorna 1 (cert) si no hi ha ni davant ni darrere
    if (q->back ==NULL && q->front == NULL) return 1;
    return 0; // Fals en cas contrari
}

// Afegeix un camí (PathNode) al final de la cua
void enqueue(Queue *q, PathNode *p){
    QueueItem *a = malloc(sizeof(QueueItem)); // Creem un nou element per la cua
    a->path = p;    // Hi guardem el camí
    a->next = NULL; // Com serà l'últim, el següent és NULL
    
    // Si la cua està buida, aquest element serà tant el principi com el final
    if(queueisEmpty(q)==1){
        q->back=a; 
        q->front=a;
    }
    // Si no està buida, l'afegim al final de la llista enllaçada i actualitzem el darrere
    else{
        q->back->next= a;
        q->back = a;
    }
}

// Treu i retorna el primer camí (PathNode) de la cua
PathNode* dequeue(Queue *q){
    if(queueisEmpty(q)==1) return NULL; // Si està buida retornem NULL
    
    QueueItem *temp = q->front; // Guardem temporalment el primer element
    PathNode *path = temp->path; // Extraiem el camí que conté
    
    q->front = temp->next; // El principi de la cua passa a ser el següent element
    
    // Si la cua queda buida, actualitzem també el punter del final a NULL
    if(q->front==NULL) q->back=NULL;
    
    free(temp); // Alliberem la memòria de l'element de la cua extret
    return path; // Retornem el camí que hem tret
}

// Buidem la cua i alliberem tota la memòria associada
void queuefree(Queue *q){
    // Mentre la cua no estigui buida, anem desencuant i alliberant cada camí
    while(queueisEmpty(q)==0){
        pathfree(dequeue(q));
    }
    return;
}

// Alliberem un camí (llista enllaçada de PathNodes)
void pathfree(PathNode *p){
    // Recorrem el camí i alliberem cada node
    while(p!=NULL){
        PathNode *temp = p; // Guardem el node actual
        p=p->next;          // Avancem al següent
        free(temp);         // Alliberem l'actual
    }
}

// Crea un nou camí afegint un carrer al final d'un camí existent
PathNode* pathAppend(PathNode *p, Street street){
    PathNode *copy = pathCopy(p); // Creem una còpia del camí original
    PathNode *head = copy;        // Guardem el cap del nou camí
    
    // Avancem fins a l'últim node de la còpia
    while(copy->next!=NULL){
        copy = copy->next;
    }
    
    // Creem un nou node pel carrer que volem afegir
    PathNode *new = malloc(sizeof(PathNode));
    new->street = street;
    new->next = NULL;
    
    copy->next = new; // L'afegim al final de la còpia
    
    return head; // Retornem el nou camí (còpia + carrer afegit)
}


// Crea una còpia independent d'un camí (Deep Copy)
PathNode* pathCopy(PathNode *p){
    if(p==NULL)return NULL; // Si és nul, la còpia també
    
    // Creem el primer node
    PathNode *new = malloc(sizeof(PathNode));
    new->street = p->street;
    PathNode *head = new; // Guardem el cap per retornar-lo
    
    p = p->next; // Avancem al següent node de l'original
    
    // Recorrem la resta del camí original per anar-lo copiant
    while(p!=NULL){
        new->next = malloc(sizeof(PathNode)); // Reservem el següent
        new = new->next;                      // Ens movem al nou node
        new->street = p->street;              // Copiem el carrer
        p = p->next;                          // Avancem a l'original
    }
    new->next = NULL; // Assegurem que l'últim node apunti a NULL
    
    return head; // Retornem la còpia sencera
}

// Cerca en amplada (BFS) per trobar el camí més curt
PathNode *bfs(HashMap *graph, Street fromStreet, Street toStreet){
    Queue *q = createQueue(); // Creem la cua per al BFS
    
    // Creem el node inicial pel carrer de partida
    PathNode *initial = malloc(sizeof(PathNode));
    initial->street = fromStreet;
    initial->next = NULL;
    enqueue(q, initial); // L'encuem com a punt de partida
    
    VisitedNode *visited = NULL; // Llista d'interseccions visitades
    
    // Mentre hi hagi camins per explorar a la cua
    while (queueisEmpty(q)!=1){
        PathNode *current = dequeue(q); // Traiem el primer camí
        PathNode *temp = current;
        
        // Busquem quin és l'últim carrer d'aquest camí
        while(temp->next!=NULL){
            temp = temp->next;
        }
        
        // Si hem arribat al carrer de destí
        if(toStreet.id1==temp->street.id1 && toStreet.id2==temp->street.id2){
            freeVisited(visited); // Alliberem la memòria dels visitats
            queuefree(q);         // Alliberem la cua
            return current;       // Retornem el camí trobat
        }
        
        // Si el carrer actual (les seves interseccions) encara no s'ha visitat
        if(isVisited(visited,temp->street.id1,temp->street.id2)==0){
            // El marquem com a visitat
            visited=addVisited(visited,temp->street.id1,temp->street.id2);
            
            // Busquem a quins carrers podem anar des de l'extrem (id2) del carrer actual
            HashEntry *entry = findIntersection(graph, temp->street.id2);
            
            if(entry != NULL){
                StreetList *connected = entry->streets; // Carrers connectats
                
                // Iterem per tots els carrers connectats
                while(connected != NULL){
                    // Si el carrer connectat no ha estat visitat
                    if(isVisited(visited, connected->street.id1, connected->street.id2)==0){
                        // Creem un nou camí afegint el carrer connectat
                        PathNode *newPath = pathAppend(current, connected->street);
                        enqueue(q, newPath); // L'encuem per explorar-lo més endavant
                    }
                    connected = connected->next; // Següent carrer connectat
                }
            }
        }
    }
    
    // Si hem buidat la cua i no hem trobat res, alliberem recursos
    freeVisited(visited);
    queuefree(q);
    return NULL; // Retornem NULL perquè no hi ha camí
}

// Comprova si un carrer (definit pels seus IDs) ja ha estat visitat
int isVisited(VisitedNode *visited, long long id1, long long id2){
    // Recorrem la llista de visitats
    while (visited!=NULL){
        // Si trobem les dues interseccions, ja ha estat visitat
        if(visited->id1==id1 && visited->id2==id2)return 1;
        visited = visited->next;
    }
    return 0; // Si no el trobem, no està visitat
}

// Afegeix una nova intersecció visitada al principi de la llista
VisitedNode* addVisited(VisitedNode *visited, long long id1, long long id2){
    VisitedNode *new = malloc(sizeof(VisitedNode)); // Reservem memòria
    new->id1 = id1; // Guardem els identificadors
    new->id2 = id2;
    new->next = visited; // L'enllacem a l'inici de la llista
    return new; // Retornem el nou cap de la llista
}

// Allibera la llista de nodes visitats
void freeVisited(VisitedNode *visited){
    // Recorrem i alliberem un a un
    while(visited!=NULL){
        VisitedNode *temp = visited; // Guardem l'actual
        visited = visited->next;     // Avancem
        free(temp);                  // Alliberem l'actual
    }
}

// Converteix coordenades (lat, lon) a coordenades cartesianes (x, y) per calcular distàncies i angles
void latlon_to_xy(double lat_ref, double lon_ref,
                  double lat, double lon,
                  double *x, double *y) {
    double lat_ref_rad = toRadians(lat_ref);    // Latitud de referència en radians
    double dlat = toRadians(lat - lat_ref);     // Diferència de latitud
    double dlon = toRadians(lon - lon_ref);     // Diferència de longitud
    
    // Projecció equirectangular
    *x = EARTH_RADIUS * dlon * cos(lat_ref_rad);
    *y = EARTH_RADIUS * dlat;
}

// Imprimeix la ruta a seguir (Indicacions "turn left" / "turn right")
void printRoute(PathNode *path){
    printf("Start at %s\n", path->street.name); // Inici
    PathNode *current = path;                   // Punter per recórrer el camí
    int distance = path->street.length;         // Distància acumulada pel mateix carrer
    
    // Recorrem fins al penúltim element per calcular canvis de sentit
    while(current->next != NULL){
        PathNode *prev = current;       // Carrer actual
        current = current->next;        // Següent carrer
        
        // Coordenades del carrer actual (A->B) i del següent (B->C)
        double ax, ay, bx, by, cx, cy;
        
        // Transformem lat/lon a plans per operar amb vectors
        latlon_to_xy(prev->street.lat1, prev->street.lon1, prev->street.lat1, prev->street.lon1, &ax, &ay);
        latlon_to_xy(prev->street.lat1, prev->street.lon1, prev->street.lat2, prev->street.lon2, &bx, &by);
        latlon_to_xy(prev->street.lat1, prev->street.lon1, current->street.lat2, current->street.lon2, &cx, &cy);
        
        // Vectors de direcció: AB i BC
        double abx = bx - ax;
        double aby = by - ay;
        double bcx = cx - bx;
        double bcy = cy - by;
        
        // Producte creuat per veure si es gira a la dreta o esquerra
        double cross = abx * bcy - aby * bcx;
        
        // Si seguim al mateix carrer, només sumem la distància
        if(strcmp(prev->street.name, current->street.name)==0){
            distance += current->street.length;
        }
        else{ // Si canviem de carrer
            // Si el producte creuat > 0 és esquerra, sinó dreta
            if(cross > 0) printf("Turn left to %s and continue for %dm\n", current->street.name, (int)distance);
            else printf("Turn right to %s and continue for %dm\n", current->street.name, (int)distance);
            
            // Reiniciem la distància pel nou carrer
            distance = 0;
        }
        
    }
    // Avís d'arribada al destí
    printf("You have arrived to %s\n", current->street.name);
}