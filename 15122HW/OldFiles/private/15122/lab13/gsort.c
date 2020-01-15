
#include <stdlib.h>
#include <stdbool.h>
#include "lib/gsort.h"


void gsort(void *A, size_t count, size_t elt_size,
           swap_fn *swp,
           compare_fn *compar)
{
  char *B = (char*) A;
  for (size_t i = 0; i < count; i++) {
    size_t min = i;
    for (size_t j = i + 1; j < count; j++) {
      if ((*compar)(B + j * elt_size, B + min * elt_size) < 0) {
        min = j;
      }
    }
    (*swp)(B + i * elt_size, B + min * elt_size);
  }
}



