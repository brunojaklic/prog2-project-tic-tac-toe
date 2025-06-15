#include "tictactoe.h"

extern char symbol;

void toggle_symbol() {
    if (symbol == 'X') {
        symbol = 'O';
    }
    else {
        symbol = 'X';
    }
}