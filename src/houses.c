#include "houses.h"
#include "house_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

HouseNode* addHouse(HouseNode *head, House house){
    HouseNode *newNode = malloc(sizeof(HouseNode)); //Creem nou node i reservem memoria 
    newNode->house = house; //Guardem la casa pasada al nou node
    newNode->next = head;   //Nou node, apunta al primer node

    return newNode; //Tornem le nou node
}



HouseNode* loadHouses(char *filename){
    FILE *file = fopen(filename, "r");  //Obrim fitxer

    if(file==NULL){ //Comprovem si existeix
        printf("No s'ha pogut obrir el fitxer\n");
        return NULL;
    }

    HouseNode *head = NULL;  //Creem el cap de la linkedlist
    char line[256]; //On guardarem la linia del fitxer

    while(fgets(line, sizeof(line), file)){ //LLegim fitxer linia a linia
        House house;    //Creem una casa
        
        sscanf(line, "%[^;];%d;%lf;%lf", house.street, &house.number, &house.lat, &house.lon);   //Guardem les dades d'un string per parts, cada cop que arriba a ; es guarda a una variable diferent
        head = addHouse(head, house);   //Afegim la casa a la linkedlist
    }

    fclose(file);   //Tanquem el fitxer

    return head;    //Retornar la linked list
}


HouseNode* findHouse(HouseNode *head, char *street, int number){
    HouseNode *current = head;   //Comencem pel principi
    //convertim el carrer rebut en miúscules
    char inputstreet[100];
    strcpy(inputstreet, street);
    minuscules(inputstreet);
    while (current != NULL){    //Recorrem la llista mentres hi hagin nodes
        // convertim el carrer a comparar amb el input en minúcules
        char currentStreet[100];
        strcpy(currentStreet, current->house.street);
        minuscules(currentStreet);
        if (strcmp(currentStreet, inputstreet) == 0 &&
            current->house.number == number) {
            return current;
        }
        current = current->next; //Si no la trobem, seguim recorrent
    }
    return NULL; //Si no existeix, retornem el null
}