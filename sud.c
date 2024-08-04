#include "sud.h"
#include <string.h>

bool sud_game_init(struct Sudoku *game) {
  memset(game->board, 0, 9 * 9);

  game->board[5][5] = 5;
  game->board[0][0] = 4;
  game->board[8][8] = 1;
  return true;
}
bool test_func(unsigned int input) { return input > 0; }
