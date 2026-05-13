#include "../test/utils.h"
#include "houses.h"
#include "place.h"
#include "sample_lib.h"
#include <ctype.h>
#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include "streets.h"


int main() {

  printf("*****************\nWelcome to GOOGLE MAPS!\n*****************\n");
  // Demanar quin mapa vol utilitzar
  char mapname[20];
  printf("Introdueix el nom del mapa (ex: xs_1, xs_2, md_1, lg_1, xl_1 or "
         "2xl_1): ");
  scanf("%19s", mapname);

  int mode;
  while (true) {
    printf("Com vols introduir la posició? ((1)address / (2)coordinate / (3)place / (0)exit): ");
    int valid_number = scanf("%d", &mode);
    if (valid_number != 1) {
      while (getchar() != '\n'); // buidar el buffer
      continue;
    }

    if (mode == 0) return 0;

    else if (mode == 1) {
      char filename[150];
      snprintf(filename, sizeof(filename), "maps/%s/houses.txt", mapname);
      HouseNode *houses = loadHouses(filename);
      while (houses == NULL) {
        printf("Error carregant cases\n");
        printf("Introdueix el nom d'un mapa correcte (ex: xs_1, xs_2, md_1, lg_1, xl_1 or 2xl_1): ");
        scanf("%19s", mapname);
        snprintf(filename, sizeof(filename), "maps/%s/houses.txt", mapname);
        houses = loadHouses(filename);
      }
      snprintf(filename,sizeof(filename),"maps/%s/streets.txt",mapname); // també carreguem carrers
      StreetNode *streets = loadStreets(filename);
      printf("Houses carregat correctament\n");
      // Demanar carrer i número
      char carrer[100];
      int numero;
      printf("Introdueix el nom del carrer: ");
      scanf(" %[^\n]", carrer);

      printf("Introdueix el número del carrer: ");
      valid_number = scanf("%d", &numero);
      while (valid_number != 1) {
        // buidar el buffer
        while (getchar() != '\n');
        printf("Si us plau introdueixi un nombre vàlid: ");
        valid_number = scanf("%d", &numero);
      }

      cerca_inteligent_houses(houses, carrer, numero, streets);
      freeHouses(houses);
      freeStreets(streets);
      break; // sortim del bucle
    }
    else if (mode == 2){
      char filename[150];
      snprintf(filename,sizeof(filename),"maps/%s/streets.txt",mapname); // també carreguem carrers
      StreetNode *streets = loadStreets(filename);
      while(streets==NULL){
        printf("Error carregant streets\n");
        printf("Introdueix el nom d'un mapa correcte (ex: xs_1, xs_2, md_1, lg_1, xl_1 or 2xl_1): ");
        scanf("%19s", mapname);
        snprintf(filename,sizeof(filename),"maps/%s/streets.txt",mapname);
        streets = loadStreets(filename);
      }
      printf("Streets carregat corectament\n");
      double lat, lon;

      printf("Lat: ");
      int valid_number = scanf("%lf", &lat);
      while (valid_number != 1) {
        // buidar el buffer
        while (getchar() != '\n');
        printf("Si us plau introdueixi un nombre vàlid: ");
        valid_number = scanf("%lf", &lat);
      }
    
      printf("Lon: ");
      valid_number=scanf("%lf", &lon);
      while (valid_number != 1) {
        // buidar el buffer
        while (getchar() != '\n');
        printf("Si us plau introdueixi un nombre vàlid: ");
        valid_number = scanf("%lf", &lon);
      }
      printf("\n");
      processLocation(lat,lon,streets);
      freeStreets(streets);
      break;
    }
    else if (mode == 3) {
      // Demanar lloc
      char filename[150];
      snprintf(filename, sizeof(filename), "maps/%s/places.txt", mapname);
      PlaceNode *places = loadPlaces(filename); 
      
      while (places == NULL) {
        printf("Error carregant llocs\n");
        printf("Introdueix el nom d'un mapa correcte (ex: xs_1, xs_2, md_1,lg_1, xl_1 or 2xl_1): ");
        scanf("%19s", mapname);
        snprintf(filename, sizeof(filename), "maps/%s/places.txt", mapname);
        places = loadPlaces(filename);
      }
      snprintf(filename,sizeof(filename),"maps/%s/streets.txt",mapname); // també carreguem carrers
      StreetNode *streets = loadStreets(filename);
      printf("Places carregat correctament\n");
      char place[100];
      printf("Intodueix el nom del lloc: ");
      scanf(" %[^\n]", place);
      cerca_inteligent_places(places, place, streets); // funció de cerca
      freePlaces(places);  
      freeStreets(streets);// Alliberem memòria
      // sortim del bucle
      break;
    }
  }
  return 0;
}
