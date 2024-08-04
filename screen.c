#include "screen.h"
#include <locale.h>
#include <ncurses.h>
#include <stdint.h>
#include <stdlib.h>

#define SUCCESS_COLOR 1
#define FAIL_COLOR 2

static WINDOW *board_win;

void screen_init() {
  if (board_win) {
    delwin(board_win);
  }

  setlocale(LC_ALL, "");
  initscr();
  noecho();
  cbreak();
  curs_set(0);

  start_color();
  init_color(COLOR_BLACK, 0, 0, 0);
  init_pair(SUCCESS_COLOR, COLOR_GREEN, COLOR_BLACK);
  init_pair(FAIL_COLOR, COLOR_RED, COLOR_BLACK);

  const char grid[] = "    1   2   3   4   5   6   7   8   9  \n"
                      "  ╔═══╤═══╤═══╦═══╤═══╤═══╦═══╤═══╤═══╗\n"
                      "A ║   │   │   ║   │   │   ║   │   │   ║\n"
                      "  ╟───┼───┼───╫───┼───┼───╫───┼───┼───╢\n"
                      "B ║   │   │   ║   │   │   ║   │   │   ║\n"
                      "  ╟───┼───┼───╫───┼───┼───╫───┼───┼───╢\n"
                      "C ║   │   │   ║   │   │   ║   │   │   ║\n"
                      "  ╠═══╪═══╪═══╬═══╪═══╪═══╬═══╪═══╪═══╣\n"
                      "D ║   │   │   ║   │   │   ║   │   │   ║\n"
                      "  ╟───┼───┼───╫───┼───┼───╫───┼───┼───╢\n"
                      "E ║   │   │   ║   │   │   ║   │   │   ║\n"
                      "  ╟───┼───┼───╫───┼───┼───╫───┼───┼───╢\n"
                      "F ║   │   │   ║   │   │   ║   │   │   ║\n"
                      "  ╠═══╪═══╪═══╬═══╪═══╪═══╬═══╪═══╪═══╣\n"
                      "G ║   │   │   ║   │   │   ║   │   │   ║\n"
                      "  ╟───┼───┼───╫───┼───┼───╫───┼───┼───╢\n"
                      "H ║   │   │   ║   │   │   ║   │   │   ║\n"
                      "  ╟───┼───┼───╫───┼───┼───╫───┼───┼───╢\n"
                      "I ║   │   │   ║   │   │   ║   │   │   ║\n"
                      "  ╚═══╧═══╧═══╩═══╧═══╧═══╩═══╧═══╧═══╝\n";

  board_win = newwin(20, 45, 10, 3);

  waddstr(board_win, grid);

  refresh();
  wrefresh(board_win);
}

static inline uint8_t board_to_screen_col(int8_t col) { return col * 2 + 2; }

static inline uint8_t board_to_screen_row(int8_t row) { return row * 4 + 4; }

void screen_fill_board(uint8_t board[9][9]) {
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {

      char value = '0' + board[i][j];

      if (value == '0') {
        value = ' ';
      }

      screen_enter_guess(value, i, j, DEFAULT);
    }
  }
}

void screen_swap_highlighted_col(int8_t new_col, int8_t old_col) {
  if (old_col >= 0) {
    wmove(board_win, board_to_screen_col(old_col), 0);
    wchgat(board_win, 1, A_NORMAL, 0, NULL);
    wrefresh(board_win);
  }

  if (new_col >= 0) {
    wmove(board_win, board_to_screen_col(new_col), 0);
    wchgat(board_win, 1, A_STANDOUT, 0, NULL);
    wrefresh(board_win);
  }
}

void screen_swap_highlighted_row(int8_t new_row, int8_t old_row) {
  if (old_row >= 0) {
    wmove(board_win, 0, board_to_screen_row(old_row));
    wchgat(board_win, 1, A_NORMAL, 0, NULL);
    wrefresh(board_win);
  }

  if (new_row >= 0) {
    wmove(board_win, 0, board_to_screen_row(new_row));
    wchgat(board_win, 1, A_STANDOUT, 0, NULL);
    wrefresh(board_win);
  }
}

void screen_set_input_cursor(int8_t col, int8_t row) {
  uint8_t screen_col = board_to_screen_col(col);
  uint8_t screen_row = board_to_screen_row(row);

  curs_set(1);
  wmove(board_win, screen_col, screen_row);
  waddch(board_win, ' ');
  wmove(board_win, screen_col, screen_row);
  wrefresh(board_win);
}

void screen_clear_highlights(int8_t col, int8_t row) {
  uint8_t screen_col = board_to_screen_col(col);
  uint8_t screen_row = board_to_screen_row(row);

  curs_set(0);
  wmove(board_win, screen_col, 0);
  wchgat(board_win, 1, A_NORMAL, 0, NULL);

  wmove(board_win, 0, screen_row);
  wchgat(board_win, 1, A_NORMAL, 0, NULL);

  wrefresh(board_win);
}

void screen_enter_guess(char guess, uint8_t col, uint8_t row,
                        enum GuessType guess_type) {
  uint8_t screen_col = board_to_screen_col(col);
  uint8_t screen_row = board_to_screen_row(row);

  wmove(board_win, screen_col, screen_row);

  switch (guess_type) {
  case CORRECT:
    wattron(board_win, COLOR_PAIR(SUCCESS_COLOR));
    break;
  case INCORRECT:
    wattron(board_win, COLOR_PAIR(FAIL_COLOR));
    break;
  case DEFAULT:
  default:
    wattron(board_win, COLOR_PAIR(0));
  }

  wrefresh(board_win);
  waddch(board_win, guess);

  wattroff(board_win, COLOR_PAIR(SUCCESS_COLOR));
  wattroff(board_win, COLOR_PAIR(FAIL_COLOR));
  wattroff(board_win, COLOR_PAIR(0));
  wrefresh(board_win);
}
