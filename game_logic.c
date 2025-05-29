#include "tictactoe.h"

static int check_line(const char* board, int a, int b, int c, char symbol) {
    if (a < 0 || b < 0 || c < 0 || a > 8 || b > 8 || c > 8) {
        return 0;
    }
    return (board[a] == symbol && board[b] == symbol && board[c] == symbol);
}

GameState check_winner(const char* board) {
    const int winning_combinations[8][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8},
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8},
        {0, 4, 8}, {2, 4, 6}
    };

    for (int i = 0; i < 8; i++) {
        if (check_line(board, winning_combinations[i][0],
            winning_combinations[i][1],
            winning_combinations[i][2], 'X')) {
            return X_WON;
        }
        if (check_line(board, winning_combinations[i][0],
            winning_combinations[i][1],
            winning_combinations[i][2], 'O')) {
            return O_WON;
        }
    }

    for (int i = 0; i < 9; i++) {
        if (board[i] != 'X' && board[i] != 'O') {
            return PLAYING;
        }
    }
    return DRAW;
}