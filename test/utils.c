#include "utils.h"
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void minuscules(char *str) {
  for (int i = 0; str[i]; i++) {
    if (str[i] >= 'A' && str[i] <= 'Z') {
      str[i] += 32;
    }
  }
}

void assertEquals(const char *found, const char *expected) {
  if (strcmp(expected, found) != 0) {
    fprintf(stderr, "\033[0;31m    Expected '%s' but found '%s'\033[0m\n\n",
            expected, found);
    assert(0);
  }
}

void assertEqualsInt(int found, int expected) {
  if (expected != found) {
    fprintf(stderr, "\033[0;31m    Expected '%d' but found '%d'\033[0m\n\n",
            expected, found);
    assert(0);
  }
}

void assertNull(void *found) {
  if (NULL != found) {
    fprintf(stderr, "\033[0;31m    Expected '%p' but found '%p'\033[0m\n\n",
            NULL, found);
    assert(0);
  }
}

void success() { fprintf(stderr, "\033[0;32mPASSED\n\033[0m"); }

void running(const char *description) {
  fprintf(stderr, "\033[0;36mRunning: %s\033[0m\n", description);
}

void successtest() { fprintf(stderr, "\033[0;32m    PASSED\033[0m\n"); }

void runningtest(const char *description) {
  fprintf(stderr, "\033[0;36m  - Running: %s\033[0m\n", description);
}

void allsuccess() {
  fprintf(stderr, "\033[0;32m--- ALL TESTS PASSED --- \n\033[0m");
}

// LEVENSHTEIN
int levenshteinDistance(char *a, char *b) {
  int m = strlen(a);   // Longitud del primer string
  int n = strlen(b);   // Longitud del segon string
  int i, j;            // Variables per als índexs dels bucles
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

Leve *addSugestion(Leve *head, char *name, int dist) {
  if (dist > 30)
    return head; // Si la distancia es molt alta, no ens interessa

  Leve *actual = head; // Evitem duplicats
  while (actual != NULL) {
    if (strcasecmp(actual->name, name) == 0)
      return head;
    actual = actual->next;
  }

  Leve *newNode = (Leve *)malloc(sizeof(Leve)); // Creem node nou
  if (!newNode)
    return head; // Error de memoria

  strncpy(newNode->name, name, 99);
  newNode->name[99] = '\0';
  newNode->distance = dist;
  newNode->next = NULL;

  // Incerció ordenada
  if (head == NULL ||
      dist < head->distance) { // Si llista buida o el nou és millor
    newNode->next = head;
    return newNode;
  }

  Leve *temp = head; // Buscar el lloc correcte entremig o final
  while (temp->next != NULL && temp->next->distance <= dist) {
    temp = temp->next;
  }
  newNode->next = temp->next;
  temp->next = newNode;

  return head;
}

void freeSugestion(Leve *head) {
  while (head != NULL) {
    Leve *temp = head;
    head = head->next;
    free(temp);
  }
}
