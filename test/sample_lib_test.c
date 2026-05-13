#include "../src/sample_lib.h"
#include "../src/houses.h"
#include "../src/streets.h"
#include "utils.h"
#include <stddef.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

void test_fact4() {
  runningtest("test_fact4");
  {
    int expectedResult = 24;
    int result = fact(4);

    assertEqualsInt(result, expectedResult);
  }
  successtest();
}

void test_fact3() {
  runningtest("test_fact3");
  {
    int expectedResult = 6;
    int result = fact(3);
    assertEqualsInt(result, expectedResult);
  }
  successtest();
}

void test_fact2() {
  runningtest("test_fact2");
  {
    int expectedResult = 2;
    int result = fact(2);

    // This line fails the test if result != expectedResult
    // Try changing expectedResult to 3 and run "make t" to see what happens
    assertEqualsInt(result, expectedResult);
  }
  successtest();
}

void sample_lib_test() {
  running("sample_lib_test");
  {
    // Call all tests you want to run here
    test_fact4();
    test_fact3();
    test_fact2();
  }
  success();
}

void test_minuscules_majuscules() {
  runningtest("test_minuscules_majuscules");
  {
    char s[] = "CARRER MAJOR";
    minuscules(s);
    assertEquals(s, "carrer major");
  }
  successtest();
}
 
void test_minuscules_ja_minuscules() {
  runningtest("test_minuscules_ja_minuscules");
  {
    char s[] = "carrer major";
    minuscules(s);
    assertEquals(s, "carrer major");
  }
  successtest();
}
 
void test_minuscules_mixt() {
  runningtest("test_minuscules_mixt");
  {
    char s[] = "Avinguda Catalunya";
    minuscules(s);
    assertEquals(s, "avinguda catalunya");
  }
  successtest();
}
 
void test_minuscules_buit() {
  runningtest("test_minuscules_buit");
  {
    char s[] = "";
    minuscules(s);
    assertEquals(s, "");
  }
  successtest();
}
 
void minuscules_test() {
  running("minuscules_test");
  {
    test_minuscules_majuscules();
    test_minuscules_ja_minuscules();
    test_minuscules_mixt();
    test_minuscules_buit();
  }
  success();
}
 

void test_min3_primer() {
  runningtest("test_min3_primer_es_el_minim");
  {
    assertEqualsInt(min3(1, 2, 3), 1);
  }
  successtest();
}
 
void test_min3_segon() {
  runningtest("test_min3_segon_es_el_minim");
  {
    assertEqualsInt(min3(5, 2, 3), 2);
  }
  successtest();
}
 
void test_min3_tercer() {
  runningtest("test_min3_tercer_es_el_minim");
  {
    assertEqualsInt(min3(5, 4, 1), 1);
  }
  successtest();
}
 
void test_min3_iguals() {
  runningtest("test_min3_tots_iguals");
  {
    assertEqualsInt(min3(3, 3, 3), 3);
  }
  successtest();
}
 
void min3_test() {
  running("min3_test");
  {
    test_min3_primer();
    test_min3_segon();
    test_min3_tercer();
    test_min3_iguals();
  }
  success();
}
 
 
void test_levenshtein_identics() {
  runningtest("test_levenshtein_strings_identics");
  {
    assertEqualsInt(levenshteinDistance("carrer", "carrer"), 0);
  }
  successtest();
}
 
void test_levenshtein_un_caracter() {
  runningtest("test_levenshtein_un_caracter_diferent");
  {
    assertEqualsInt(levenshteinDistance("cat", "bat"), 1);
  }
  successtest();
}
 
void test_levenshtein_buit() {
  runningtest("test_levenshtein_string_buit");
  {
    assertEqualsInt(levenshteinDistance("", "abc"), 3);
  }
  successtest();
}
 
void test_levenshtein_insercio() {
  runningtest("test_levenshtein_insercio_caracter");
  {
    assertEqualsInt(levenshteinDistance("abc", "abcd"), 1);
  }
  successtest();
}
 
void test_levenshtein_eliminacio() {
  runningtest("test_levenshtein_eliminacio_caracter");
  {
    assertEqualsInt(levenshteinDistance("abcd", "abc"), 1);
  }
  successtest();
}
 
void levenshtein_test() {
  running("levenshtein_test");
  {
    test_levenshtein_identics();
    test_levenshtein_un_caracter();
    test_levenshtein_buit();
    test_levenshtein_insercio();
    test_levenshtein_eliminacio();
  }
  success();
}
 
void test_addHouse_llista_buida() {
  runningtest("test_addHouse_llista_buida");
  {
    House h = {"Carrer Major", 10, 41.3851, 2.1734};
    HouseNode *list = NULL;
    list = addHouse(list, h);
    assertEquals(list->house.street, "Carrer Major");
    assertEqualsInt(list->house.number, 10);
    freeHouses(list);
  }
  successtest();
}
 
void test_addHouse_insercio_principi() {
  runningtest("test_addHouse_insercio_al_principi");
  {
    House h1 = {"Carrer Major", 10, 41.3851, 2.1734};
    House h2 = {"Avinguda Roma", 5, 41.39, 2.16};
    HouseNode *list = NULL;
    list = addHouse(list, h1);
    list = addHouse(list, h2);
    assertEquals(list->house.street, "Avinguda Roma");
    assertEquals(list->next->house.street, "Carrer Major");
    freeHouses(list);
  }
  successtest();
}
 
void test_findHouse_existeix() {
  runningtest("test_findHouse_casa_existent");
  {
    House h1 = {"Carrer Major", 10, 41.3851, 2.1734};
    House h2 = {"Avinguda Roma", 5, 41.39, 2.16};
    HouseNode *list = NULL;
    list = addHouse(list, h1);
    list = addHouse(list, h2);
    HouseNode *res = findHouse(list, "Carrer Major", 10);
    assertEqualsInt(res->house.number, 10);
    freeHouses(list);
  }
  successtest();
}
 
void test_findHouse_case_insensitive() {
  runningtest("test_findHouse_case_insensitive");
  {
    House h = {"Carrer Major", 10, 41.3851, 2.1734};
    HouseNode *list = NULL;
    list = addHouse(list, h);
    HouseNode *res = findHouse(list, "CARRER MAJOR", 10);
    assertEqualsInt(res->house.number, 10);
    freeHouses(list);
  }
  successtest();
}
 
void test_findHouse_numero_incorrecte() {
  runningtest("test_findHouse_numero_incorrecte_retorna_null");
  {
    House h = {"Carrer Major", 10, 41.3851, 2.1734};
    HouseNode *list = NULL;
    list = addHouse(list, h);
    HouseNode *res = findHouse(list, "Carrer Major", 99);
    assertNull(res);
    freeHouses(list);
  }
  successtest();
}
 
void test_findHouse_carrer_inexistent() {
  runningtest("test_findHouse_carrer_inexistent_retorna_null");
  {
    House h = {"Carrer Major", 10, 41.3851, 2.1734};
    HouseNode *list = NULL;
    list = addHouse(list, h);
    HouseNode *res = findHouse(list, "Carrer Fals", 10);
    assertNull(res);
    freeHouses(list);
  }
  successtest();
}
 
void test_findHouse_llista_buida() {
  runningtest("test_findHouse_llista_buida_retorna_null");
  {
    HouseNode *res = findHouse(NULL, "Carrer Major", 10);
    assertNull(res);
  }
  successtest();
}
 
void houses_test() {
  running("houses_test");
  {
    test_addHouse_llista_buida();
    test_addHouse_insercio_principi();
    test_findHouse_existeix();
    test_findHouse_case_insensitive();
    test_findHouse_numero_incorrecte();
    test_findHouse_carrer_inexistent();
    test_findHouse_llista_buida();
  }
  success();
}
 
 
void test_haversine_mateixa_posicio() {
  runningtest("test_haversine_mateixa_posicio_distancia_zero");
  {
    double d = haversine(41.3851, 2.1734, 41.3851, 2.1734);
    if (d > 0.001) {
      fprintf(stderr, "    Expected ~0 but got %f\n", d);
      assertEqualsInt(1, 0);
    }
  }
  successtest();
}
 
void test_haversine_simetrica() {
  runningtest("test_haversine_distancia_simetrica");
  {
    double d1 = haversine(41.3851, 2.1734, 40.4168, -3.7038);
    double d2 = haversine(40.4168, -3.7038, 41.3851, 2.1734);
    if (fabs(d1 - d2) > 0.001) {
      fprintf(stderr, "    Expected symmetric but got %f vs %f\n", d1, d2);
      assertEqualsInt(1, 0);
    }
  }
  successtest();
}
 
void test_haversine_bcn_madrid() {
  runningtest("test_haversine_barcelona_madrid_aprox_504km");
  {
    double dist = haversine(41.3851, 2.1734, 40.4168, -3.7038);
    if (dist < 490.0 || dist > 520.0) {
      fprintf(stderr, "    Expected ~504 km but got %f\n", dist);
      assertEqualsInt(1, 0);
    }
  }
  successtest();
}
 
void haversine_test() {
  running("haversine_test");
  {
    test_haversine_mateixa_posicio();
    test_haversine_simetrica();
    test_haversine_bcn_madrid();
  }
  success();
}
 

void test_addSugestion_una_entrada() {
  runningtest("test_addSugestion_una_entrada");
  {
    Leve *list = NULL;
    list = addSugestion(list, "Carrer Major", 2);
    assertEquals(list->name, "Carrer Major");
    assertEqualsInt(list->distance, 2);
    freeSugestion(list);
  }
  successtest();
}
 
void test_addSugestion_ordre_distancia() {
  runningtest("test_addSugestion_ordenat_per_distancia");
  {
    Leve *list = NULL;
    list = addSugestion(list, "Carrer Major", 3);
    list = addSugestion(list, "Avinguda Roma", 1);
    assertEquals(list->name, "Avinguda Roma");
    assertEquals(list->next->name, "Carrer Major");
    freeSugestion(list);
  }
  successtest();
}
 
void test_addSugestion_sense_duplicats() {
  runningtest("test_addSugestion_sense_duplicats");
  {
    Leve *list = NULL;
    list = addSugestion(list, "Carrer Major", 2);
    list = addSugestion(list, "Carrer Major", 2);
    int count = 0;
    Leve *aux = list;
    while (aux) { count++; aux = aux->next; }
    assertEqualsInt(count, 1);
    freeSugestion(list);
  }
  successtest();
}
 
void test_addSugestion_distancia_alta() {
  runningtest("test_addSugestion_distancia_mes_de_30_no_safegeix");
  {
    Leve *list = NULL;
    list = addSugestion(list, "Carrer Lluny", 31);
    assertNull(list);
  }
  successtest();
}
 
void sugestion_test() {
  running("sugestion_test");
  {
    test_addSugestion_una_entrada();
    test_addSugestion_ordre_distancia();
    test_addSugestion_sense_duplicats();
    test_addSugestion_distancia_alta();
  }
  success();
}
