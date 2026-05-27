#include "sample_lib_test.c"
#include "utils.h"
#include <stdio.h>

int main() {
  {
    sample_lib_test();
    minuscules_test();
    min3_test();
    levenshtein_test();
    houses_test();
    haversine_test();
    sugestion_test();
    place_test();
    streets_test();
    intersection_test();
    bfs_test();
  }
  allsuccess();
}