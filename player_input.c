#define _CRT_SECURE_NO_WARNINGS
#include "tictactoe.h"

extern char symbol;

inline int is_valid_move(const char* board, int position) {
    return position >= 1 && position <= 9 &&
        board[position - 1] != 'X' && board[position - 1] != 'O';
}

void input_move(char* board) {
    int position;
    printf("Unesite svoje polje izbora: \n");

    while (1) {
        if (scanf("%d", &position) != 1) {
            while (getchar() != '\n');
            printf("Neispravan unos. Unesite broj (1-9): ");
            continue;
        }

        if (is_valid_move(board, position)) {
            board[position - 1] = symbol;
            break;
        }
        printf("Nevazeci potez. Pokusajte ponovo: ");
    }
}

int input_integer(const char* prompt) {
    int value;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &value) == 1) {
            while (getchar() != '\n');
            break;
        }
        printf("Neispravan unos! Molimo unesite broj.\n");
        while (getchar() != '\n');
    }
    return value;
}