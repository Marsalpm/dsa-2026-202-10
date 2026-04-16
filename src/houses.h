#ifndef HOUSES_H
#define HOUSES_H

typedef struct{
    char street[100];
    int number;
    double lat;
    double lon;
}House;

typedef struct HouseNode {
    House house;
    struct HouseNode *next;
} HouseNode;

HouseNode* addHouse(HouseNode *head, House house);
HouseNode* loadHouses(char *filename);
HouseNode* findHouse(HouseNode *head, char *street, int number);
HouseNode* cerca_inteligent(HouseNode *head, char *street, int number); 
void printCoordinates(House house);
void freeHouses(HouseNode *head);

int levenshteinDistance(char *a, char *b);
int min3 (int a, int b, int c);

#endif