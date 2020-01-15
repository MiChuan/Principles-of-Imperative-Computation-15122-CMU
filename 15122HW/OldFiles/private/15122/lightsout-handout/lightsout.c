#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "lib/boardutil.h"
#include "lib/bitvector.h"
#include "lib/hdict.h"
#include "board-ht.h"
#include "lib/queue.h"
#include "lib/contracts.h"
#include "lib/xalloc.h"



bitvector press_button(struct board_data *B, uint8_t i, int row, int col,
                       uint8_t width, uint8_t height)
{
  bitvector res = bitvector_flip(B->board, i);
  int row0 = row - 1;
  int col0 = col;
  if (is_valid_pos(row0, col0, width, height)) {
    uint8_t i0 = get_index(row0, col0, width, height);
    res = bitvector_flip(res, i0);
  }
  int row1 = row + 1;
  int col1 = col;
  if (is_valid_pos(row1, col1, width, height)) {
    uint8_t i1 = get_index(row1, col1, width, height);
    res = bitvector_flip(res, i1);
  }
  int row2 = row;
  int col2 = col - 1;
  if (is_valid_pos(row2, col2, width, height)) {
    uint8_t i2 = get_index(row2, col2, width, height);
    res = bitvector_flip(res, i2);
  }
  int row3 = row;
  int col3 = col + 1;
  if (is_valid_pos(row3, col3, width, height)) {
    uint8_t i3 = get_index(row3, col3, width, height);
    res = bitvector_flip(res, i3);
  }
  return res;
}



bool is_solvable(uint8_t width, uint8_t height, struct board_data *start)
{
  if (bitvector_equal(start->board, bitvector_new())) {
    free(start);
    return true;
  }
  size_t capacity = (width * height) * 50;
  hdict_t H = ht_new(capacity);
  queue_t Q = queue_new();
  enq(Q, (void *)start);
  ht_insert(H, start);
  while (!queue_empty(Q)) {
    void *B = deq(Q);
    for (int row = 0; row < height; row++) {
      for (int col = 0; col < width; col++) {
        uint8_t i = get_index(row, col, width, height);
        bitvector newboard = press_button((struct board_data *)B, i,
                                           row, col, width, height);       
        if (bitvector_equal(newboard, bitvector_new())) {
          int sol_row = row;
          int sol_col = col;
          printf("%d : %d\n", sol_row, sol_col);
          bitvector sol_board = newboard;
          while (!bitvector_equal(sol_board, start->board)) {
            struct board_data *sol = xmalloc(sizeof(struct board_data));
            sol->board = sol_board;
            sol->row_index_press = sol_row;
            sol->col_index_press = sol_col;
            uint8_t index = get_index(sol_row, sol_col, width, height);
            sol_board = press_button(sol, index, sol_row, sol_col, width, height);
            hdict_entry e = ht_lookup(H, sol_board);
            sol_row = ((struct board_data *)e)->row_index_press;
            sol_col = ((struct board_data *)e)->col_index_press;
            if (!bitvector_equal(sol_board, start->board)) 
            // check if sol_board == start board find the target(board)
            {
              printf("%d : %d\n", ((struct board_data *)e)->row_index_press, 
                  ((struct board_data *)e)->col_index_press);
            }
            free(sol);
          }
          hdict_free(H);
          queue_free(Q, NULL);
          return true;
        }
        if (ht_lookup(H, newboard) == NULL) {
          struct board_data* newdata = xmalloc(sizeof(struct board_data));
          newdata->board = newboard;
          newdata->row_index_press = row;
          newdata->col_index_press = col;
          ht_insert(H, newdata);
          enq(Q, (void *)newdata);
        }
      }
    }
  }
  hdict_free(H);
  queue_free(Q, NULL);
  return false;
}


int main (int argc, char **argv)
{
	if (argc != 2) {
		fprintf(stderr, "Usage: lightsout <board name>\n");
		return 1;
	}
	char *board_filename = argv[1];
  struct board_data* start = xmalloc(sizeof(struct board_data));
	uint8_t width = 0;
	uint8_t height = 0;
  if (!file_read(board_filename, &(start->board), &width, &height)) {
    free(start);
    return 1;
  }
  if (is_solvable(width, height, start)) {
    printf("Board is solvable!\n");
    return 0;
  }
  else {
    printf("No solution was found!\n");
    return 1;
  }
}

