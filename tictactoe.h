#ifndef TICTACTOE_H
#define TICTACTOE_H

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef enum {
    PLAYING,
    X_WON,
    O_WON,
    DRAW
} GameState;

typedef enum {
    MENU_ADD_SCORE = 1,
    MENU_UPDATE_SCORE,
    MENU_DELETE_SCORE,
    MENU_EXIT
} MenuOption;

typedef struct {
    char name[20];
    int score;
} player_score;

void display_scores();
void display_board(const char* board);
void input_move(char* board);
void toggle_symbol();
GameState check_winner(const char* board);
void sort_player_scores(player_score* array, int n);
int compare_scores(const void* a, const void* b);
void update_player_score(const char* player_name, int new_score);
void delete_player_score(const char* player_name);
int is_name_available(const char* name);
void add_player_score(const char* player_name, int score);
int compare_names(const void* a, const void* b);
int input_integer(const char* prompt);

#endif