#ifndef HOUSE_LIST_H
#define HOUSE_LIST_H

#include "houses.h"

typedef struct HouseNode{
    House house;
    struct HouseNode *next;
}HouseNode;

HouseNode* addHouse(HouseNode *head, House house);
HouseNode* findHouse(HouseNode *head, char *street, int number);
void printCoordinates(House house);

#endif