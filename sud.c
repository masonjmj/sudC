#include "sud.h"
#include <ncurses.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define BOARD_SIZE 9

bool sud_game_init(struct Sudoku *game) {
  memset(game->board, 0, BOARD_SIZE * BOARD_SIZE);

  // game->board[5][5] = 5;
  // game->board[0][0] = 4;
  // game->board[8][8] = 1;
  return true;
}

void shuffle_array(int *arr, size_t size) {
  if (size <= 0) {
    return;
  }

  for (size_t i = size - 1; i > 0; i--) {
    int j = rand() % (i + 1);

    int temp = arr[j];
    arr[j] = arr[i];
    arr[i] = temp;
  }
}

// Naïve backtracking solver to find solutions
int unique_solutions(struct Sudoku *game, struct SudokuCell cell, int count) {
  // Base case. If we're called with y as the board size then we've checked all cells
  if (cell.y == BOARD_SIZE) {
    return count + 1;
  }

  bool next_row = cell.x == BOARD_SIZE - 1;
  struct SudokuCell new_cell = {
      .x = next_row ? 0 : cell.x + 1,
      .y = next_row ? cell.y + 1 : cell.y,
  };

  // Skip filled cells
  if (game->board[cell.y][cell.x] != 0) {
    return unique_solutions(game, new_cell, count);
  }

  // Check numbers 1-9 that are legal to see how many solutions they produce (once it's more than 1 we know the solution isn't unique);
  for (uint8_t i = 1; (i <= 9) && (count < 2); i++){
    if (sud_valid_placement(game, cell, i)){
      game->board[cell.y][cell.x] = i;

      // Check next cell
      count = unique_solutions(game, new_cell, count);
    }
  }

  // Reset value in cell before unwinding so that we're not actually messing up the board
  game->board[cell.y][cell.x] = 0;
  return count;
}

bool valid_row_placement(struct Sudoku *game, struct SudokuCell cell,
                         uint8_t value) {
  for (int i = 0; i < BOARD_SIZE; i++) {
    if (game->board[cell.y][i] == value) {
      return false;
    }
  }
  return true;
}

bool valid_col_placement(struct Sudoku *game, struct SudokuCell cell,
                         uint8_t value) {
  for (int i = 0; i < BOARD_SIZE; i++) {
    if (game->board[i][cell.x] == value) {
      return false;
    }
  }
  return true;
}

bool valid_subgrid_placement(struct Sudoku *game, struct SudokuCell cell,
                             uint8_t value) {
  // Using integer division and then multipling to get start of subgrid
  int subgrid_row = (cell.x / 3) * 3;
  int subgrid_col = (cell.y / 3) * 3;

  for (int i = subgrid_col; i < subgrid_col + 3; i++) {
    for (int j = subgrid_row; j < subgrid_row + 3; j++) {
      if (game->board[i][j] == value) {
        return false;
      }
    }
  }
  return true;
}

bool sud_valid_placement(struct Sudoku *game, struct SudokuCell cell,
                         uint8_t guess) {
  return valid_row_placement(game, cell, guess) &&
         valid_col_placement(game, cell, guess) &&
         valid_subgrid_placement(game, cell, guess);
}

bool sud_solvable_placement(struct Sudoku *game, struct SudokuCell cell, uint8_t guess){
  struct SudokuCell first_cell = {.x = 0, .y = 0};
  
  game->board[cell.y][cell.x] = guess;
  bool solvable = sud_valid_placement(game, cell, guess) && (unique_solutions(game, first_cell, 0) > 0);
  game->board[cell.y][cell.x] = 0;

  return solvable;
}

bool test_func(unsigned int input) { return input > 0; }
