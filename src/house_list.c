#include "house_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

HouseNode* addHouse(HouseNode *head, House house){
    HouseNode *newNode= malloc(sizeof(HouseNode));  //Guardem memoria per a cada casa nova

    newNode->house = house; //Guardem casa actual
    newNode->next = head;   //Pasem a la seguent casa (el inici de la llista, pasa a ser el seguent node)

    return newNode;
}

HouseNode* findHouse(HouseNode *head, char *street, int number){
    HouseNode *current = head; //La casa actual es el cap (1r) de la llista

    while(current != NULL){ // Mentre no apunti a memoria buida (final)
        if(strcmp(current->house.street, street) == 0 && current->house.number == number){ // Si tant carrer com casa son iguals
            return current; // Retorna el node
        }
        current = current->next; // Sino, avancem al seguent
    }

    return NULL; // Si no trobem res, la casa no existeix, retorna NULL
}

void printCoordinates(House house){
    printf("Trobat a (%f, %f)\n", house.lat, house.lon); //Mostra les coordenades
}