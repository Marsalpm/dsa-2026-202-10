#include "sample_lib.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

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



  char mapname[20];

  printf("Introdueix el nom del mapa (ex: xs_1, xs_2, md_1, lg_1, xl_1 or xl_1): ");
  scanf("%s", mapname);

  char filename[100];
  strcpy(filename, "maps/"); //Construim un string "maps/"
  strcat(filename, mapname); //"maps/mapname"
  strcat(filename, "/houses.txt"); //Adreca final: "maps/mapname/houses.txt"

  return 0;
}