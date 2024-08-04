#ifndef SCREEN_H
#define SCREEN_H

#include <stdint.h>

enum GuessType {
  DEFAULT,
  CORRECT,
  INCORRECT,
};

void screen_init();
void screen_swap_highlighted_col(int8_t new_col, int8_t old_cold);
void screen_swap_highlighted_row(int8_t new_row, int8_t old_row);
void screen_fill_board(uint8_t board[9][9]);
void screen_set_input_cursor(int8_t col, int8_t row);
void screen_clear_highlights(int8_t col, int8_t row);
void screen_enter_guess(char guess, uint8_t col, uint8_t row, enum GuessType guess_type);

#endif
