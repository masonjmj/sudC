#ifndef SUD_H
#define SUD_H

#include <stdbool.h>
#include <stdint.h>

struct Sudoku {
  uint8_t board[9][9];
};

bool sud_game_init(struct Sudoku *game);

bool test_func(unsigned int input);

#endif
