#include <stdio.h>
#include <time.h>

#include "streets.h"
#include "intersection.map.h"
#include "bfs.h"

int main(){
    StreetNode *streets = loadStreets("maps/md_1/streets.txt"); //Carreguem el mapa
    HashMap *graph = buildGraph(streets);   //Crea el hasmap rapid

    Street from = streets->street; //Carrer origen
    //CAS 1
    Street to = streets->street;  //Carrer desti

    // CAS 2
    //Street to = streets->next->next->next->street;
 
    // CAS 3 I 4. (i<100) i (i<1000)
    //StreetNode *tmp = streets;
     //   for(int i=0;i<100;i++)  tmp = tmp->next;
    //Street to = tmp->street;

    // ULTIM CARRER
    //StreetNode *tmp = streets;
    //while(tmp->next != NULL) tmp = tmp->next;
    //Street to = tmp->street;

    clock_t start, end; //Cariables per guardar el temps

    //BFS LENT
    start = clock(); // Guardem temps de CPU al iniciar la busqueda
    PathNode *path1 = bfs_slow(streets, from, to); //Busqueda
    end = clock();  // Guardem temps de CPU al acabar la busqueda

    double slow_time = ((double)(end-start)) / CLOCKS_PER_SEC; //Calcula els segons, de ticks de CPU a segons


    //BFS LENT
    start = clock(); // Guardem temps de CPU al iniciar la busqueda
    PathNode *path2 = bfs(graph, from, to); //Busqueda
    end = clock();  // Guardem temps de CPU al acabar la busqueda

    double fast_time = ((double)(end-start)) / CLOCKS_PER_SEC; //Calcula els segons, de ticks de CPU a segons

    //IMPRIMIM RESULTATS
    printf("BFS slow: %f seconds\n", slow_time);
    printf("BFS fast: %f seconds\n", fast_time);

    printf("FROM: %s\n", from.name);
    printf("TO: %s\n", to.name);

    printf("\n C LINEAL \n");
    printRoute(path1);

    printf("\n HASHMAP \n");
    printRoute(path2);

    //ALLIBEREM MEMÒRIA
    pathfree(path1);
    pathfree(path2);
    freeHashMap(graph);
    freeStreets(streets);


    return 0;
}