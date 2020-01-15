#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include "lib/contracts.h"
#include "lib/bitvector.h"


// test functions of bitvector
void test_bitvector() {
  // tests for bitvector_new()
  bitvector b1 = bitvector_new();
  ASSERT(b1 == 0);

  // tests for bitvector_flip()
  bitvector b2 = bitvector_flip(b1, BITVECTOR_LIMIT - 1);
  bitvector b3 = bitvector_flip(b1, 0);
  bitvector b4 = bitvector_new();
  bitvector b5 = bitvector_new();


  // tests for bitvector_get()
  ASSERT(bitvector_get(b1, BITVECTOR_LIMIT - 1) == false);
  ASSERT(bitvector_get(b2, BITVECTOR_LIMIT - 1) == true);
  ASSERT(bitvector_get(b2, BITVECTOR_LIMIT - 2) == false);
  ASSERT(bitvector_equal(b1, b2) == false);
  ASSERT(bitvector_get(b1, 0) == false);
  ASSERT(bitvector_get(b3, 0) == true);
  ASSERT(bitvector_equal(b1, b3) == false);
  ASSERT(bitvector_equal(b4, b5) == true);
  ASSERT(bitvector_equal(b4, b1) == true);
  printf("All Passed!");

  return;
}

int main() {
  test_bitvector();
  return 0;
}
