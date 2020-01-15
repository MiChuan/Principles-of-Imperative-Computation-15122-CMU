#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "lib/contracts.h"
#include "lib/xalloc.h"
#include "lib/bitvector.h"
#include "lib/hdict.h"
#include "board-ht.h"


/**********************/
/*** Implementation ***/
/**********************/

//get the key from an entry
hdict_key get_key_from_entry(hdict_entry e)
{
  struct board_data *entry = (struct board_data *)e;
  return (void *)(&entry->board);
}

//check whether two keys are the same
bool check_key_equal(hdict_key k1, hdict_key k2)
{
  return bitvector_equal(*(bitvector *)k1, *(bitvector *)k2);
}

// achieve the hash value of key
size_t get_hash_value(hdict_key k)
{
  size_t h = 0;
  for (uint8_t i = 0; i < BITVECTOR_LIMIT; i++) {
    size_t num = 0;
    if (bitvector_get(*(bitvector *)k, i)) {
      num = 1;
    }
    h = h + (num << i);
  }
  return h;
}


/* Initializes a new hash table with the given capacity */
hdict_t ht_new(size_t capacity)
{
  REQUIRES(0 < capacity);
  hdict_t new = hdict_new(capacity,
                  &get_key_from_entry,
                  &check_key_equal,
                  &get_hash_value,
                  &free);
  ENSURES(new != NULL);
  return new;
}

//insert into an hashtable
void ht_insert(hdict_t H, struct board_data *DAT)
{
  REQUIRES(H != NULL);
  ENSURES(ht_lookup(H, DAT->board) == NULL);
  hdict_entry e = hdict_insert(H, (void *)DAT);
  if (e != NULL) {
    free(e);
  }
  return;
}

/* ht_lookup(H,B) returns
 * NULL if no struct containing the board B exists in H
 * A struct containing the board B if one exists in H.
 */
struct board_data *ht_lookup(hdict_t H, bitvector B)
{
  REQUIRES(H != NULL);
  struct board_data *res = hdict_lookup(H, (void*)(&B));
  if (res != NULL)
  {
    return res;
  }
  return NULL;
}





