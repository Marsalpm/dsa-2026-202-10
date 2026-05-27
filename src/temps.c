#include <stdio.h>
#include <time.h>
#include "streets.h"
#include "intersection.map.h"

int main(){
    StreetNode *streets = loadStreets("maps/xs_1/streets.txt"); //Carreguem tots els carrers del mapa i loadStreets crea una linked list de carrers

    StreetNode *closest = findClosestStreet(streets, 41.403782, 2.193446);  //Busquem quin segment esta mes aprop a aquestes coordenades de exemple

    // VERSIÓ LAB4
    clock_t start1 = clock();   //Torna temps actual de CPU

    printConnectedStreets(streets, closest->street);    //Mira quins carrers comparteixen inteeseccio

    clock_t end1 = clock(); //PArem cronometre (temps CPU)

    double temps1 = ((double)(end1 - start1)) / CLOCKS_PER_SEC; //Calculem segons reals
    // end1 - start1, dona ticks de CPU, no segons
    //Dividir entre CLOCKS_PER_SEC ho pasa a segons

    printf("Temps lineal: %f\n", temps1);

    // VERSIÓ LAB5
    HashMap *graph = buildGraph(streets);   //Construim el hashmap

    clock_t start2 = clock();

    printConnectedStreetsFast(graph, closest->street);

    clock_t end2 = clock();

    double temps2 = ((double)(end2 - start2)) / CLOCKS_PER_SEC;

    printf("Temps hashmap: %f\n", temps2);

    freeHashMap(graph);
    freeStreets(streets);

    return 0;


}