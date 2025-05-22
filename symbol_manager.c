#include "tictactoe.h"

extern char symbol;

void toggle_symbol() {
    symbol = (symbol == 'X') ? 'O' : 'X';
}