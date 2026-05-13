#include "sample_lib_test.c"
#include "utils.h"
#include <stdio.h>

int main() {
  {
    // Call all test modules you want to run here
    sample_lib_test();
    minuscules_test();
    min3_test();
    levenshtein_test();
    houses_test();
    haversine_test();
    sugestion_test();
  }
  allsuccess();
}