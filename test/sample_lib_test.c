#include "../src/houses.h"
#include "../src/place.h"
#include "../src/sample_lib.h"
#include "../src/streets.h"
#include "utils.h"
#include <math.h>
#include <stddef.h>
#include <stdio.h>
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
  { assertEqualsInt(min3(1, 2, 3), 1); }
  successtest();
}

void test_min3_segon() {
  runningtest("test_min3_segon_es_el_minim");
  { assertEqualsInt(min3(5, 2, 3), 2); }
  successtest();
}

void test_min3_tercer() {
  runningtest("test_min3_tercer_es_el_minim");
  { assertEqualsInt(min3(5, 4, 1), 1); }
  successtest();
}

void test_min3_iguals() {
  runningtest("test_min3_tots_iguals");
  { assertEqualsInt(min3(3, 3, 3), 3); }
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
  { assertEqualsInt(levenshteinDistance("carrer", "carrer"), 0); }
  successtest();
}

void test_levenshtein_un_caracter() {
  runningtest("test_levenshtein_un_caracter_diferent");
  { assertEqualsInt(levenshteinDistance("cat", "bat"), 1); }
  successtest();
}

void test_levenshtein_buit() {
  runningtest("test_levenshtein_string_buit");
  { assertEqualsInt(levenshteinDistance("", "abc"), 3); }
  successtest();
}

void test_levenshtein_insercio() {
  runningtest("test_levenshtein_insercio_caracter");
  { assertEqualsInt(levenshteinDistance("abc", "abcd"), 1); }
  successtest();
}

void test_levenshtein_eliminacio() {
  runningtest("test_levenshtein_eliminacio_caracter");
  { assertEqualsInt(levenshteinDistance("abcd", "abc"), 1); }
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
    while (aux) {
      count++;
      aux = aux->next;
    }
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

void test_addPlace_llista_buida() {
  runningtest("test_addPLace_llista_buida");
  {
    Place p = {"Parc Güell", 41.4145, 2.1527};
    PlaceNode *list = NULL;
    list = addPlace(list, p);
    assertEquals(list->place.name, "Parc Güell");
    freePlaces(list);
  }
  successtest();
}

void test_addPlace_insercio_principi() {
  runningtest("test_addPlace_insercio_principi");
  {
    Place p1 = {"Parc Güell", 41.4145, 2.1527};
    Place p2 = {"Sagrada Familia", 41.4036, 2.1744};
    PlaceNode *list = NULL;
    list = addPlace(list, p1);
    list = addPlace(list, p2);
    assertEquals(list->place.name, "Sagrada Familia");
    assertEquals(list->next->place.name, "Parc Güell");
    freePlaces(list);
  }
  successtest();
}

void test_findPlace_existeix() {
  runningtest("test_findPlace_lloc_existent");
  {
    Place p1 = {"Parc Güell", 41.4145, 2.1527};
    Place p2 = {"Sagrada Familia", 41.4036, 2.1744};
    PlaceNode *list = NULL;
    list = addPlace(list, p1);
    list = addPlace(list, p2);
    PlaceNode *res = findPlace(list, "Parc Güell");
    assertEquals(res->place.name, "Parc Güell");
    freePlaces(list);
  }
  successtest();
}

void test_findPlace_case_insensitve() {
  runningtest("test_findPlace_case_insensitive");
  {
    Place p = {"Parc Guell", 41.4145, 2.1527};
    PlaceNode *list = NULL;
    list = addPlace(list, p);
    PlaceNode *res = findPlace(list, "parc guell");
    assertEquals(res->place.name, "Parc Guell");
    freePlaces(list);
  }
  successtest();
}

void test_findPlace_inexistent() {
  runningtest("test_findPlace_lloc_inexistent_restorna_null");
  {
    Place p = {"Parc Guell", 41.4145, 2.1527};
    PlaceNode *list = NULL;
    list = addPlace(list, p);
    PlaceNode *res = findPlace(list, "Lloc Fals");
    assertNull(res);
    freePlaces(list);
  }
  successtest();
}

void test_findPlace_llista_buida() {
  runningtest("test_findPlace_llista_buida_retorna_nul");
  {
    PlaceNode *res = findPlace(NULL, "Parc Guell");
    assertNull(res);
  }
  successtest();
}

void place_test() {
  running("places_test");
  {
    test_addPlace_llista_buida();
    test_addPlace_insercio_principi();
    test_findPlace_existeix();
    test_findPlace_case_insensitve();
    test_findPlace_inexistent();
    test_findPlace_llista_buida();
  }
  success();
}

void test_loadStreets_no_null() {
  runningtest("test_loadStreets_no_null");
  {
    StreetNode *list =
        loadStreets("maps/xs_1/streets.txt"); // Intentem carregar el mapa xs_1
    if (list != NULL) { // Comprovem que la llista no estigui buida
      successtest();
    } else {
      fprintf(stderr,
              "    Error: No s'ha pogut carregar maps/xs_1/streets.txt\n");
      assertEqualsInt(1, 0); // Fem que hi hagi error
    }
    freeStreets(list);
  }
}

void test_findClosestStreet_basic() {
  runningtest("test_findClosestStreet_basic");
  {
    Street s = {
        1,    41.0,
        2.0,  2,
        41.2, 2.2,
        1.0,  "Carrer de Prova"}; // Creem una llista amb un carrer per provar
    StreetNode *list = NULL;
    list = addStreet(list, s);

    StreetNode *res = findClosestStreet(
        list, 41.11, 2.11); // Busquem el carrer més proper del midpoint

    if (res != NULL && strcmp(res->street.name, "Carrer de Prova") == 0) {
      successtest();
    } else {
      fprintf(stderr,
              "    Error: No s'ha trobat el carrer més proper correctament\n");
      assertEqualsInt(1, 0);
    }
    freeStreets(list);
  }
}

// Aquesta ajunta els tests de carrers
void streets_test() {
  running("streets_test");
  {
    test_loadStreets_no_null();
    test_findClosestStreet_basic();
  }
  success();
}
