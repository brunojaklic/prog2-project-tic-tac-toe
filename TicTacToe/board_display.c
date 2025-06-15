#include "tictactoe.h"

void display_board(const char* x) {
    system("cls");
    printf("\n\t\t    *** IKS-OKS ***\n\n");
    printf("\t\t       |       |       \n");
    printf("\t\t   %c   |   %c   |   %c   \n", x[0], x[1], x[2]);
    printf("\t\t_______|_______|_______\n");
    printf("\t\t       |       |       \n");
    printf("\t\t   %c   |   %c   |   %c   \n", x[3], x[4], x[5]);
    printf("\t\t_______|_______|_______\n");
    printf("\t\t       |       |       \n");
    printf("\t\t   %c   |   %c   |   %c   \n", x[6], x[7], x[8]);
    printf("\t\t       |       |       \n\n");
}