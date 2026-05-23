#ifndef ASSERTS_H
#define ASSERTS_H
#include "../src/houses.h"
#include <stddef.h>

typedef struct Leve {
  char name[100];
  int distance;
  struct Leve *next;
} Leve;
void freeSugestion(Leve *head);
Leve *addSugestion(Leve *head, char *name, int dist);

void assertEquals(const char *found, const char *expected);
void assertEqualsInt(int found, int expected);
void success();
void running(const char *description);
void successtest();
void runningtest(const char *description);
void allsuccess();
void minuscules(char *str);
void assertNull(void *found);

#endif