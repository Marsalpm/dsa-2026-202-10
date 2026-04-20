#include <stdio.h>
#include "place.h"
#include <stdlib.h>
#include <strings.h>

PlaceNode* addPlace(PlaceNode *head, Place p) {
    PlaceNode *newNode = malloc(sizeof(PlaceNode));
    newNode->place = p;
    newNode->next = head;
    return newNode;
}

PlaceNode* loadPlaces(char *filename) {
    FILE *file = fopen(filename, "r");

    if (file==NULL){ // Comprovem si el fitxer s'ha obert correctament
        printf("No s'ha pogut obrir el fitxer\n");
        return NULL; // Si no existeix, retornem NULL
    }

    PlaceNode *head = NULL;
    char line[256];  // Buffer per guardar cada línia llegida del fitxer

    while (fgets(line, sizeof(line), file)) { // Llegim el fitxer línia a línia fins al final
        Place p; // Creem una estructura temporal per a la casa
        // Parsegem la línia separada per punts i coma i guardem les dades
        if (sscanf(line, "%[^;];%lf;%lf", p.name, &p.lat, &p.lon) == 3) {
            head = addPlace(head, p);// Afegim el lloc parsejat a la llista
        }
    }

    fclose(file); // Tanquem el fitxer per alliberar recursos
    return head; // Retornem la llista enllaçada completa
}

PlaceNode* findPlace(PlaceNode *head, char *name) {
    PlaceNode *current = head; // Comencem la cerca pel principi de la llista
    while (current != NULL) { // Mentres hi hagi carrer a la llista i no haguem trobat el carrer, seguirem buscant
        if (strcasecmp(current->place.name, name) == 0) {
            return current;// Si coincideixen, hem trobat el lloc i el retornem
        }
        current = current->next; // Si no, avancem al següent node de la llista
    }

    return NULL; // Si acabem el bucle sense trobar res, retornem NULL
}