#include "intersection.map.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Funció hash: Converteix un intersectionId (molt gran) en un índex petit dins la taula. Això permet accedir ràpidament al hashmap.

int hash(long long id) {
    return id % TABLE_SIZE;
}

// Crea el hashmap:- Reserva memòria per l'estructura HashMap - Inicialitza totes les posicions de la taula a NULL (buides)
HashMap *createHashMap() {

    HashMap *map = malloc(sizeof(HashMap));

    // Si no hi ha memòria, retornem NULL
    if (map == NULL) return NULL;

    // Inicialitzem totes les posicions del hashmap a buit
    for (int i = 0; i < TABLE_SIZE; i++) {
        map->table[i] = NULL;
    }

    return map;
}

// Inserir un carrer al hashmap: IMPORTANT: aquest primer pas només tracta id1 (una de les dues interseccions)

void insertStreet(HashMap *map, Street s) {

    // Calculem en quin bucket (posició) anirà aquest id
    int index = hash(s.id1);
    
    // Mirem si ja existeix alguna entrada en aquesta posició
    HashEntry *entry = map->table[index];

    // Si no existeix cap entrada, la creem
    if (entry == NULL) {

        // Reservem memòria per una nova entrada del hashmap
        entry = malloc(sizeof(HashEntry));

        // Guardem la intersecció associada a aquesta entrada
        entry->intersectionId = s.id1;

        // Inicialitzem la llista de carrers connectats (buid)
        entry->streets = NULL;

        // No hi ha col·lisions encara
        entry->next = NULL;

        // Guardem aquesta entrada dins la taula
        map->table[index] = entry;
    }
}

