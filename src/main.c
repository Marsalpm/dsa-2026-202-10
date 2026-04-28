#include "sample_lib.h"
#include "../test/utils.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <stdbool.h>
#include "houses.h"
#include <strings.h>
#include <ctype.h>
#include "place.h"


void createaleak() {
  char *foo = malloc(20 * sizeof(char));
  printf("Allocated leaking string: %s", foo);
}

int main() {
 
  printf("*****************\nWelcome to GOOGLE MAPS!\n*****************\n");
  // Demanar quin mapa vol utilitzar
  char mapname[20];

  printf("Introdueix el nom del mapa (ex: xs_1, xs_2, md_1, lg_1, xl_1 or 2xl_1): ");
  scanf("%19s", mapname);

  
  
  // Demanar com vol introducir la posició
  int mode;

  while (true)
  {
    printf("Com vols introduir la posició? ((1)address / (2)coordinate / (3)place): ");
    int valid_number=scanf("%d", &mode);
    while(valid_number!=1 || mode<1 || mode>3 ){
      //vaciar el buffer
      while(getchar()!='\n');
      printf("Si us plau introdueixi un nombre vàlid: ");
      valid_number=scanf("%d", &mode);
    }

    if (mode == 1) {
      char filename[100];
      snprintf(filename, sizeof(filename),"maps/%s/houses.txt", mapname);
      HouseNode *houses = loadHouses(filename);
      while(houses == NULL) {
        printf("Error carregant cases\n");
        printf("Introdueix el nom d'un mapa correcte (ex: xs_1, xs_2, md_1, lg_1, xl_1 or 2xl_1): ");
        scanf("%19s", mapname);
        snprintf(filename, sizeof(filename),"maps/%s/houses.txt", mapname);
        houses = loadHouses(filename);}
      printf("Houses carregat correctament\n");
      //Demanar carrer i número
      char carrer[100];
      int numero;
      printf("Introdueix el nom del carrer: ");
      scanf(" %[^\n]", carrer);
      printf("Introdueix el número del carrer: ");
      valid_number=scanf("%d", &numero);
      while(valid_number!=1){
      //vaciar el buffer
      while(getchar()!='\n');
      printf("Si us plau introdueixi un nombre vàlid: ");
      valid_number=scanf("%d", &numero);
      }
      cerca_inteligent_houses(houses, carrer, numero);
      freeHouses(houses);
      break; // sortim del bucle 
      }

    else if(mode == 3){
      // Demanar lloc
      char filename[100];
      snprintf(filename, sizeof(filename),"maps/%s/places.txt", mapname);
      PlaceNode *places = loadPlaces(filename); // també carrega places
      while(places == NULL) {
          printf("Error carregant llocs\n");
          printf("Introdueix el nom d'un mapa correcte (ex: xs_1, xs_2, md_1, lg_1, xl_1 or 2xl_1): ");
          scanf("%19s", mapname);
          char filename[100];
          snprintf(filename, sizeof(filename),"maps/%s/places.txt", mapname);
          places = loadPlaces(filename);
          }
      printf("Places carregat correctament\n");
      char place[100];
      printf("Intodueix el nom del lloc: ");
      scanf(" %[^\n]", place); 
      cerca_inteligent_places(places, place); // funció de cerca
      freePlaces(places); // Alliberem memòria 
      // sortim del bucle 
      break;
    }

    else if (mode == 2) {
      printf("Not implemented yet\n");
      }

  }
return 0;
}