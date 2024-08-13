#ifndef SUD_H
#define SUD_H

#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>

struct Sudoku {
  uint8_t board[9][9];
};

struct SudokuCell {
  int8_t x;
  int8_t y; 
};

bool sud_game_init(struct Sudoku *game);
bool sud_valid_placement(struct Sudoku *game, struct SudokuCell cell, uint8_t guess);
bool sud_solvable_placement(struct Sudoku *game, struct SudokuCell cell, uint8_t guess);
int unique_solutions(struct Sudoku *game, struct SudokuCell cell, int count);

bool test_func(unsigned int input);

#endif
