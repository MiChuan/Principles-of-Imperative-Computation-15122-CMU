#include <stdlib.h>
#include "lib/bitvector.h"
#include "lib/xalloc.h"
#include "lib/contracts.h"


/**********************/
/*** Implementation ***/
/**********************/


/* Get a new bitvector with everything set to 'false'. */
bitvector bitvector_new()
{
  return (bitvector) 0;
}


/* Get the ith bit of the bitvector n. */
bool bitvector_get(bitvector B, uint8_t i)
{
  REQUIRES(i < BITVECTOR_LIMIT);
  bitvector num = 1;
  return (((B >> i) & num) == num);
}


/* Compare two bitvectors for equality. */
bool bitvector_equal(bitvector B1, bitvector B2)
{
  bitvector num = 0;
  return ((B1 ^ B2) == num);
}


/* Toggle the ith bit of the bitvector n, returning a new bitvector. */
/* The old bitvector remains unchanged. */
bitvector bitvector_flip(bitvector B, uint8_t i)
{
  REQUIRES(i < BITVECTOR_LIMIT);
  bitvector num = 1;
  bitvector mask = num << i;
  return B ^ mask;
}







