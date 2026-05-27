#include "../test/utils.h"
#include "houses.h"
#include "place.h"
#include "sample_lib.h"
#include "streets.h"
#include <ctype.h>
#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include "intersection.map.h"
#include "bfs.h"

int main() {

  printf("*****************\nWelcome to GOOGLE MAPS!\n*****************\n");
  // Demanar quin mapa vol utilitzar
  char mapname[20];
  printf("Enter the map name (ex: xs_1, xs_2, md_1, lg_1, xl_1 or "
         "2xl_1): ");
  scanf("%19s", mapname);
  char filename[150];
  double initial_lat, initial_lon, final_lat, final_lon;


  // Carregar houses
  snprintf(filename, sizeof(filename), "maps/%s/houses.txt", mapname);
  HouseNode *houses = loadHouses(filename);
  while (houses == NULL) {
    printf("Error loading houses\n");
    printf("Enter a valid map name (ex: xs_1, xs_2, md_1, "
            "lg_1, xl_1 or 2xl_1): ");
    scanf("%19s", mapname);
    snprintf(filename, sizeof(filename), "maps/%s/houses.txt", mapname);
    houses = loadHouses(filename);
  }
  printf("Houses loaded correctly\n");
  //Carregar streets
  snprintf(filename, sizeof(filename), "maps/%s/streets.txt", mapname); 
  StreetNode *streets = loadStreets(filename);
  while (streets == NULL) {
    printf("Error loading streets\n");
    printf("Enter a valid map name (ex: xs_1, xs_2, md_1, "
            "lg_1, xl_1 or 2xl_1): ");
    scanf("%19s", mapname);
    snprintf(filename, sizeof(filename), "maps/%s/streets.txt", mapname);
    streets = loadStreets(filename);
  }
  printf("Streets loaded correctly\n");

  // Carregar places
  snprintf(filename, sizeof(filename), "maps/%s/places.txt", mapname);
  PlaceNode *places = loadPlaces(filename);
  while (places == NULL) {
    printf("Error loading places\n");
    printf("Enter a valid map name (ex: xs_1, xs_2, "
            "md_1,lg_1, xl_1 or 2xl_1): ");
    scanf("%19s", mapname);
    snprintf(filename, sizeof(filename), "maps/%s/places.txt", mapname);
    places = loadPlaces(filename);
  }
  printf("Places loaded correctly\n");

  int mode;
  printf("\n\n--- ORIGIN ---\n");
  while (true) {
    printf("How do you want to input the position? ((1)address / (2)place / (3)coordinate / (0)exit): ");
    int valid_number = scanf("%d", &mode);
    while (valid_number != 1 || mode<0 || mode>3) {
      while (getchar() != '\n')
        ; // buidar el buffer
        printf("Enter a valid number: ");
        valid_number = scanf("%d", &mode);
    }

    if (mode == 0) {
      freeHouses(houses);
      freePlaces(places);
      freeStreets(streets);
      return 0;
    }

    else if (mode == 1) {
      // Demanar carrer i número
      char carrer[100];
      int numero;
      printf("Enter street name: ");
      scanf(" %99[^\n]", carrer);

      printf("Enter street number: ");
      valid_number = scanf("%d", &numero);
      while (valid_number != 1) {
        // buidar el buffer
        while (getchar() != '\n')
          ;
        printf("Enter a valid number:  ");
        valid_number = scanf("%d", &numero);
      }
      int success = cerca_inteligent_houses(houses, carrer, numero, streets, &initial_lat, &initial_lon,1);
      if (success == 1) break; // sortim del bucle
      printf("\n");
    } else if (mode == 2) {
      // Demanar lloc
      char place[100];
      printf("Enter the place name: ");
      scanf(" %99[^\n]", place);
      int success = cerca_inteligent_places(places, place, streets, &initial_lat, &initial_lon, 1); // funció de cerca
      // sortim del bucle
      if (success == 1) break;
      printf("\n");
    } else if (mode == 3) {
      printf("Lat: ");
      int valid_number = scanf("%lf", &initial_lat);
      while (valid_number != 1) {
        // buidar el buffer
        while (getchar() != '\n')
          ;
        printf("Enter a valid number: ");
        valid_number = scanf("%lf", &initial_lat);
      }

      printf("Lon: ");
      valid_number = scanf("%lf", &initial_lon);
      while (valid_number != 1) {
        // buidar el buffer
        while (getchar() != '\n')
          ;
        printf("Enter a valid number: ");
        valid_number = scanf("%lf", &initial_lon);
      }
      StreetNode *closest = findClosestStreet(streets, initial_lat, initial_lon);
      double dist = haversine(initial_lat, initial_lon, closest->street.lat1, closest->street.lon1);
      while(dist > 10.0) {
          printf("Coordinates out of range, please try again\n");
          printf("Lat: ");
          valid_number = scanf("%lf", &initial_lat);
          while (valid_number != 1) {
            // buidar el buffer
            while (getchar() != '\n')
              ;
            printf("Enter a valid number: ");
            valid_number = scanf("%lf", &initial_lat);
          }
          printf("Lon: ");
          valid_number = scanf("%lf", &initial_lon);
          while (valid_number != 1) {
            // buidar el buffer
            while (getchar() != '\n')
              ;
            printf("Enter a valid number: ");
            valid_number = scanf("%lf", &initial_lon);
          }
          closest = findClosestStreet(streets, initial_lat, initial_lon);
          dist = haversine(initial_lat, initial_lon, closest->street.lat1, closest->street.lon1);
      }
      printf("\n");
      processLocation(initial_lat, initial_lon, streets, 1);
  
      break;
    } 
  }
  
  printf("\n\n--- DESTINATION ---\n");
  while(true){
    printf("Where do you want to go? Address (1), Place (2), Coordinate (3) or Exit (0): ");
    int valid_number = scanf("%d", &mode);
    while (valid_number != 1 || mode<0 || mode>3) {
      while (getchar() != '\n')
        ; // buidar el buffer
        printf("Please enter a valid number: ");
        valid_number = scanf("%d", &mode);
    }
    if(mode==0) {
      freeHouses(houses);
      freePlaces(places);
      freeStreets(streets);
      return 0;
    }
    else if(mode==1){
    char destination[150];
    int number;
    printf("Enter street name: ");
      scanf(" %149[^\n]", destination);

      printf("Enter street number: ");
      valid_number = scanf("%d", &number);
      while (valid_number != 1) {
        // buidar el buffer
        while (getchar() != '\n')
          ;
        printf("Enter a valid number: ");
        valid_number = scanf("%d", &number);
      }
      int success = cerca_inteligent_houses(houses, destination,number, streets, &final_lat, &final_lon, 0);
      if (success == 1)break;
      printf("\n");
    }
    else if(mode==2){
      char place[100];
      printf("Enter the place name: ");
      scanf(" %99[^\n]", place);
      int success = cerca_inteligent_places(places, place, streets, &final_lat, &final_lon, 0); // funció de cerca
      // sortim del bucle
      if (success == 1) break;
      printf("\n");
    }
    else if(mode==3){
       printf("Lat: ");
      int valid_number = scanf("%lf", &final_lat);
      while (valid_number != 1) {
        // buidar el buffer
        while (getchar() != '\n')
          ;
        printf("Enter a valid number: ");
        valid_number = scanf("%lf", &final_lat);
      }

      printf("Lon: ");
      valid_number = scanf("%lf", &final_lon);
      while (valid_number != 1) {
        // buidar el buffer
        while (getchar() != '\n')
          ;
        printf("Enter a valid number: ");
        valid_number = scanf("%lf", &final_lon);
      }
      StreetNode *closest = findClosestStreet(streets, final_lat, final_lon);
      double dist = haversine(final_lat, final_lon, closest->street.lat1, closest->street.lon1);
      while(dist > 10.0) {
          printf("Coordinates out of range, please try again\n");
          printf("Lat: ");
          valid_number = scanf("%lf", &final_lat);
          while (valid_number != 1) {
            // buidar el buffer
            while (getchar() != '\n')
              ;
            printf("Enter a valid number: ");
            valid_number = scanf("%lf", &final_lat);
          }
          printf("Lon: ");
          valid_number = scanf("%lf", &final_lon);
          while (valid_number != 1) {
            // buidar el buffer
            while (getchar() != '\n')
              ;
            printf("Enter a valid number: ");
            valid_number = scanf("%lf", &final_lon);
          }
          closest = findClosestStreet(streets, final_lat, final_lon);
          dist = haversine(final_lat, final_lon, closest->street.lat1, closest->street.lon1);
      }
      printf("\n");
      processLocation(final_lat, final_lon, streets, 0);
      break;
    } 
  }
  printf("\n\n--- ROUTE ---\n");
  Street fromStreet = findClosestStreet(streets, initial_lat, initial_lon)->street;
  Street toStreet = findClosestStreet(streets, final_lat, final_lon)->street;
  HashMap *graph = buildGraph(streets);
  PathNode *path= bfs(graph,fromStreet, toStreet);
  if(path != NULL){
  printRoute(path);
  pathfree(path);
  }
  freeHashMap(graph);
  freeStreets(streets);
  freeHouses(houses);
  freePlaces(places);
  return 0;
}
