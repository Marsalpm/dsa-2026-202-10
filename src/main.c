#include "sample_lib.h"
#include "utils.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <stdbool.h>
#include "houses.h"
#include <strings.h>
#include <ctype.h>

void createaleak() {
  char *foo = malloc(20 * sizeof(char));
  printf("Allocated leaking string: %s", foo);
}

int main() {
 
  printf("*****************\nWelcome to DSA!\n*****************\n");

  // how to import and call a function
  printf("Factorial of 4 is %d\n", fact(4));

  // uncomment and run "make v" to see how valgrind detects memory leaks
  // createaleak();


  // Demanar quin mapa vol utilitzar
  char mapname[20];

  printf("Introdueix el nom del mapa (ex: xs_1, xs_2, md_1, lg_1, xl_1 or xl_1): ");
  scanf("%s", mapname);

  char filename[100];
  strcpy(filename, "maps/"); //Construim un string "maps/"
  strcat(filename, mapname); //"maps/mapname"
  strcat(filename, "/houses.txt"); //Adreca final: "maps/mapname/houses.txt"
  HouseNode *houses = loadHouses(filename);
  
  // Demanar com vol introducir la posició
  char mode[20];

  while (true)
  {
    printf("Com vols introduir la posició? (address / coordinate / place): ");
    scanf("%s", mode);

    if (strcmp(mode, "address") == 0) {
      break; // sortim del bucle i continuem el programa
      }

    if (strcmp(mode, "coordinate") == 0 || strcmp(mode, "place") == 0) {
      printf("Not implemented yet\n");
      }
    printf("Opció invalida\n");
  }
  
  //Demanar carrer i número
  char carrer[100];
  int numero;
  printf("Introdueix el nom del carrer: ");
  scanf(" %[^\n]", carrer);
  printf("Introdueix el número del carrer: ");
  scanf("%d", &numero);

  HouseNode *resultat = findHouse(houses, carrer, numero);
  if(resultat==NULL){
    printf("No s'ha trobat la casa");
  } else {
    printf("Trobat a (%f, %f)\n", resultat->house.lat, resultat->house.lon);
  }
return 0;
}