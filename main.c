#include "screen.h"
#include "sud.h"
#include <ctype.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define ASCII_ZERO 48

int main(int argc, char *argv[]) {
  struct Sudoku game;
  sud_game_init(&game);

  screen_init();

  screen_fill_board(game.board);

  int8_t highlighted_col = -1;
  int8_t highlighted_row = -1;

  while (true) {
    char ch = getch();

    if (ch == 'q') {
      break;
    }

    if (isdigit(ch)) {
      uint8_t input = ch - ASCII_ZERO;
      if (input > 0) {
        input--; // User inputs 1-9 but internally 0-indexed
        screen_swap_highlighted_row(input, highlighted_row);
        highlighted_row = input;
      }
    } else if (isalpha(ch)) {
      char input = tolower(ch);
      if (input >= 97 && input <= 105) {
        uint8_t offset_from_a = input - 97;
        screen_swap_highlighted_col(offset_from_a, highlighted_col);
        highlighted_col = offset_from_a;
      }
    }

    bool valid_cell = highlighted_col >= 0 && highlighted_row >= 0 &&
                      game.board[highlighted_col][highlighted_row] == 0;

    if (valid_cell) {
      screen_set_input_cursor(highlighted_col, highlighted_row);

      bool valid_guess_or_should_exit = false;
      while (!valid_guess_or_should_exit) {
        char guess = getch();
        if (guess == 'r') {
          screen_clear_highlights(highlighted_col, highlighted_row);
          highlighted_col = -1;
          highlighted_row = -1;
          valid_guess_or_should_exit = true;
        } else if (isdigit(guess) && guess > '0') {
          uint8_t num = guess - ASCII_ZERO;
          game.board[highlighted_col][highlighted_row] = num;
          screen_fill_board(game.board);
          screen_enter_guess(guess, highlighted_col, highlighted_row, CORRECT);

          screen_clear_highlights(highlighted_col, highlighted_row);
          highlighted_col = -1;
          highlighted_row = -1;
          valid_guess_or_should_exit = true;
        }
      }
    }
  }

  endwin();

  return EXIT_SUCCESS;
}
