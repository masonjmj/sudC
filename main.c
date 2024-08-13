#include "screen.h"
#include "sud.h"
#include <ctype.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#define ASCII_ZERO 48

int main(int argc, char *argv[]) {
  srand(time(NULL));
  struct Sudoku game;
  sud_game_init(&game);

  screen_init();

  screen_fill_board(game.board);

  struct SudokuCell highlighted_cell = {
      .x = -1,
      .y = -1,
  };

  while (true) {
    char ch = getch();

    // Quit
    if (ch == 'q') {
      break;
    }

    // Select column
    if (isdigit(ch)) {
      uint8_t input = ch - ASCII_ZERO;
      if (input > 0) {
        input--; // User inputs 1-9 but internally 0-indexed
        screen_swap_highlighted_row(input, highlighted_cell.x);
        highlighted_cell.x = input;
      }
    }
    // Select row
    else if (isalpha(ch)) {
      char input = tolower(ch);
      if (input >= 97 && input <= 105) {
        uint8_t offset_from_a = input - 97;
        screen_swap_highlighted_col(offset_from_a, highlighted_cell.y);
        highlighted_cell.y = offset_from_a;
      }
    }

    bool valid_cell_selected =
        highlighted_cell.y >= 0 && highlighted_cell.x >= 0 &&
        game.board[highlighted_cell.y][highlighted_cell.x] == 0;

    if (valid_cell_selected) {
      screen_set_input_cursor(highlighted_cell.y, highlighted_cell.x);

      bool valid_guess_or_should_exit = false;
      while (!valid_guess_or_should_exit) {
        char guess = getch();

        // Cancel guess entry
        if (guess == 'r') {
          screen_clear_highlights(highlighted_cell.y, highlighted_cell.x);
          highlighted_cell.y = -1;
          highlighted_cell.x = -1;
          valid_guess_or_should_exit = true;
        }
        // Enter guess
        else if (isdigit(guess) && guess > '0') {
          uint8_t num = guess - ASCII_ZERO;
          screen_fill_board(game.board);

          bool legal_placement = sud_solvable_placement(&game, highlighted_cell, num);
          if (legal_placement) {
            game.board[highlighted_cell.y][highlighted_cell.x] = num;
          }
          struct SudokuCell first_cell = {.x = 0, .y = 0};
          int solutions = unique_solutions(&game, first_cell, 0);
          move(0, 0);
          printw("%d", solutions);


          screen_enter_guess(guess, highlighted_cell.y, highlighted_cell.x,
                             legal_placement ? CORRECT : INCORRECT);

          screen_clear_highlights(highlighted_cell.y, highlighted_cell.x);
          highlighted_cell.y = -1;
          highlighted_cell.x = -1;
          valid_guess_or_should_exit = true;
        }
      }
    }
  }

  endwin();

  return EXIT_SUCCESS;
}
