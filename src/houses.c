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

// Cerca un carrer amb una abreviatura
void cerca_inteligent(HouseNode *head, char *street, int number){
    char street_net[100];
    //Abreviatures de Carrer i Avinguda
    if(strncasecmp(street, "C. ", 3) == 0){ // el 3 es la lletra. el punt i l'espai
        sprintf(street_net, "Carrer %s", street +3);
    }
    else if(strncasecmp(street, "Av. ", 4) == 0){
        sprintf(street_net, "Avinguda %s", street + 4); // el mateix el 4.
    }
    else{
        strcpy(street_net, street);
    }
    
     // Aqui l'huaria de tribar sino té cap abreviatura (Ja tenim funcio creada en el main)     
    HouseNode *resultat = findHouse(head, street_net, number);

    if (resultat != NULL){
        printf("Trobat (%f, %f)\n", resultat->house.lat, resultat-> house.lon);
        return;
    }
    printf("No s'ha trobat la casa",street_net);
    
    // Seguiment dels numeros 
    HouseNode *temp = head;
    int street_trobat = 0;
    while(temp != NULL){
        if(strcasecmp(temp->house.street, street_net) == 0){ // utilitzem strcasacmp (funcio ja feta) per comparar sense importar les majuscules.
            printf("Nùmero vàlid disponible: %d\n", temp->house.number);
            street_trobat = 1;
        }
        temp = temp->next; // seguent node
    }
    if (street_trobat ==1){
        printf("Aquest són tots els números disponibles al carrer %s.\n",street_net);
    }
    
        else {
            printf("Carrer '%s' NO trobat.\n", street_net);
        }
}

