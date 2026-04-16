#include "houses.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // Incloem per poder usar strcasecmp i strncasecmp

HouseNode* addHouse(HouseNode *head, House house) {
    HouseNode *newNode = malloc(sizeof(HouseNode)); // Creem nou node i reservem memoria 
    if (!newNode) return head; // Si falla la reserva, retornem la llista actual
    newNode->house = house; // Guardem la casa pasada al nou node
    newNode->next = head;   // Nou node apunta a l'antic cap (insertem al principi)

    return newNode; // Tornem el nou node que ara és el cap
}

HouseNode* loadHouses(char *filename) {
    FILE *file = fopen(filename, "r"); // Obrim el fitxer en mode lectura

    if (file == NULL) { // Comprovem si el fitxer s'ha obert correctament
        printf("No s'ha pogut obrir el fitxer\n");
        return NULL; // Si no existeix, retornem NULL
    }

    HouseNode *head = NULL; // Inicialitzem el cap de la llista a buit
    char line[256]; // Buffer per guardar cada línia llegida del fitxer
    
    while (fgets(line, sizeof(line), file)) { // Llegim el fitxer línia a línia fins al final
        House house; // Creem una estructura temporal per a la casa
        // Parsegem la línia separada per punts i coma i guardem les dades
        if (sscanf(line, "%[^;];%d;%lf;%lf", house.street, &house.number, &house.lat, &house.lon) == 4) {
            head = addHouse(head, house); // Afegim la casa parsejada a la llista
        }
    }

    fclose(file); // Tanquem el fitxer per alliberar recursos

    return head; // Retornem la llista enllaçada completa
}

HouseNode* findHouse(HouseNode *head, char *street, int number) {
    HouseNode *current = head; // Comencem la cerca pel principi de la llista
    // Preparem un string per normalitzar el carrer buscat
    char inputstreet[100];
    strcpy(inputstreet, street); // Copiem el carrer d'entrada
    minuscules(inputstreet); // El passem a minúscules per ignorar diferències

    while (current != NULL) { // Recorrem la llista mentre no arribem al final
        // Preparem un string per normalitzar el carrer del node actual
        char currentStreet[100];
        strcpy(currentStreet, current->house.street); // Copiem el carrer de la casa
        minuscules(currentStreet); // El passem a minúscules
        
        // Comparem el nom normalitzat i el número de la casa
        if (strcmp(currentStreet, inputstreet) == 0 && current->house.number == number) {
            return current; // Si coincideixen, hem trobat la casa i la retornem
        }
        current = current->next; // Si no, avancem al següent node de la llista
    }
    return NULL; // Si acabem el bucle sense trobar res, retornem NULL
}

// Cerca un carrer gestionant abreviatures, números erronis i similituds
void cerca_inteligent(HouseNode *head, char *street, int number) {
    char street_net[100]; // Variable per guardar el nom del carrer "netejat"
    
    // Si comença per "C. ", ho expandim a "Carrer "
    if (strncasecmp(street, "C. ", 3) == 0) { 
        sprintf(street_net, "Carrer %s", street + 3); // Saltem els 3 primers caràcters
    }
    // Si comença per "Av. ", ho expandim a "Avinguda "
    else if (strncasecmp(street, "Av. ", 4) == 0) {
        sprintf(street_net, "Avinguda %s", street + 4); // Saltem els 4 primers caràcters
    }
    else {
        strcpy(street_net, street); // Si no hi ha abreviatura, copiem el nom tal qual
    }

    // Busquem si la casa exacta existeix amb el nom netejat
    HouseNode *resultat = findHouse(head, street_net, number);
    if (resultat != NULL) {
        printCoordinates(resultat->house); // Si existeix, imprimim coordenades
        return; // Sortim de la funció
    }
    printf("No s'ha trobat la casa exacte a %s %d\n", street_net, number);
    
    // Si no hi és, busquem si almenys el carrer existeix per suggerir números
    HouseNode *temp = head; // Punter temporal per recórrer la llista
    int street_trobat = 0; // Indicador per saber si hem trobat el carrer
    while (temp != NULL) {
        // Comparem noms de carrer ignorant majúscules
        if (strcasecmp(temp->house.street, street_net) == 0) { 
            if (street_trobat == 0) printf("Números vàlids disponibles al carrer %s:\n", street_net);
            printf("- %d\n", temp->house.number); // Imprimim cada número que anem trobant
            street_trobat = 1; // Marquem que el carrer existeix
        }
        temp = temp->next; // Passem al següent node
    }

    // Si el carrer no s'ha trobat en absolut, busquem el carrer més semblant
    if (street_trobat == 0) {
        printf("Carrer '%s' NO trobat al sistema.\n", street_net);
        
        HouseNode *temp_lev = head; // Punter per recórrer per Levenshtein
        char millor_suggeriment[100] = ""; // Per guardar el nom més semblant trobat
        int dist_minima = 100; // Inicialitzem amb un valor de distància alt
        
        while (temp_lev != NULL) {
            // Calculem la distància entre el que ha escrit l'usuari i el carrer del node
            int d = levenshteinDistance(street_net, temp_lev->house.street); 
            if (d < dist_minima) { // Si trobem una distància mes petita
                dist_minima = d; // Actualitzem la distància mínima
                strcpy(millor_suggeriment, temp_lev->house.street); // Guardem el nom
            }
            temp_lev = temp_lev->next; // Seguim amb el següent node
        }

        // Si la similitud és bien (distància < 5), fem suggeriment
        if (dist_minima < 5) { 
            printf("No hem trobat '%s'. Volies dir '%s'?\n", street_net, millor_suggeriment);
        } else {
            printf("No s'ha trobat cap carrer similar.\n");
        }
    }
}

void printCoordinates(House house) {
    printf("Trobat a (%f, %f)\n", house.lat, house.lon); // Mostra les coordenades per pantalla
}

int min3(int a, int b, int c) {
    if (a <= b && a <= c) return a; // Retorna a si és el més petit
    if (b <= a && b <= c) return b; // Retorna b si és el més petit
    return c; // Si no, retorna c
}

int levenshteinDistance(char *a, char *b) {
    int m = strlen(a); // Longitud del primer string
    int n = strlen(b); // Longitud del segon string
    int i, j; // Variables per als índexs dels bucles
    int D[m + 1][n + 1]; // Matriu per guardar els càlculs de distància

    // Inicialitzem la primera columna (cost de borrar caràcters)
    for (i = 0; i <= m; i++) {
        D[i][0] = i;
    }

    // Inicialitzem la primera fila (cost d'inserir caràcters)
    for (j = 0; j <= n; j++) {
        D[0][j] = j;
    }

    // Omplim la matriu comparant caràcter a caràcter
    for (i = 1; i <= m; i++) {
        for (j = 1; j <= n; j++) {
            int cost;
            // Si els caràcters són iguals, el cost de substitució és 0
            if (a[i - 1] == b[j - 1]) {
                cost = 0;
            } else {
                cost = 1; // Si són diferents, el cost és 1
            }
            // Triem el camí més curt entre eliminació, inserció i substitució
            D[i][j] = min3(D[i - 1][j] + 1, D[i][j - 1] + 1, D[i - 1][j - 1] + cost);
        }
    }
    return D[m][n]; // Retornem el valor de l'última cel·la (distància total)
}

void freeHouses(HouseNode *head) {
    while (head != NULL) { // Mentre la llista no estigui buida
        HouseNode *temp = head; // Guardem el node actual
        head = head->next; // Avancem el punter al següent node
        free(temp); // Alliberem la memòria del node guardat
    }
}