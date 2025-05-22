#define _CRT_SECURE_NO_WARNINGS
#include "tictactoe.h"

static const char* LOG_FILE = "rezultati.txt";

static void quick_sort_recursive(player_score* array, int low, int high) {
    if (low < high) {
        player_score pivot = array[high];
        int i = low - 1;

        for (int j = low; j <= high - 1; j++) {
            if (compare_scores(&array[j], &pivot) < 0) {
                i++;
                player_score temp = array[i];
                array[i] = array[j];
                array[j] = temp;
            }
        }

        player_score temp = array[i + 1];
        array[i + 1] = array[high];
        array[high] = temp;

        int pi = i + 1;

        quick_sort_recursive(array, low, pi - 1);
        quick_sort_recursive(array, pi + 1, high);
    }
}

void sort_player_scores(player_score* array, int n) {
    quick_sort_recursive(array, 0, n - 1);
}

int compare_scores(const void* a, const void* b) {
    const player_score* pa = (const player_score*)a;
    const player_score* pb = (const player_score*)b;

    if (pa->score > pb->score) return -1;
    if (pa->score < pb->score) return 1;
    return 0;
}

void display_scores() {
    FILE* fpointer = fopen("rezultati.txt", "r");
    if (fpointer) {
        fseek(fpointer, 0, SEEK_END);
        long size = ftell(fpointer);
        rewind(fpointer);

        if (size > 0) {
            player_score* scores = NULL;
            int score_count = 0;
            char line[100];

            while (fgets(line, sizeof(line), fpointer)) {
                if (line[0] == '-') {
                    score_count++;
                }
            }
            rewind(fpointer);

            if (score_count > 0) {
                scores = malloc(score_count * sizeof(player_score));
                if (scores == NULL) {
                    perror("Greska pri alokaciji memorije");
                    fclose(fpointer);
                    return;
                }

                int index = 0;
                while (fgets(line, sizeof(line), fpointer) && index < score_count) {
                    if (line[0] == '-') {
                        if (sscanf(line + 1, "%19s %d",
                            scores[index].name, &scores[index].score) != 2) {
                            fprintf(stderr, "Greska pri citanju rezultata\n");
                        }
                        index++;
                    }
                }

                sort_player_scores(scores, score_count);

                system("cls");
                printf("\n\t=== REZULTATI ===\n");
                for (int i = 0; i < score_count; i++) {
                    printf("%2d. Igrac: %-20s | Pobjede: %d\n",
                        i + 1, scores[i].name, scores[i].score);
                }

                free(scores);
                scores = NULL;
            }
        }
        fclose(fpointer);
    }
}

void swap_players(player_score* p1, player_score* p2) {
    player_score temp = *p1;
    *p1 = *p2;
    *p2 = temp;
}

void update_player_score(const char* player_name, int new_score) {
    FILE* fpointer = fopen(LOG_FILE, "r");
    if (!fpointer) {
        perror("Greska pri otvaranju datoteke za citanje");
        return;
    }

    FILE* temp_file = fopen("temp.txt", "w");
    if (!temp_file) {
        perror("Greska pri otvaranju privremene datoteke");
        fclose(fpointer);
        return;
    }

    char line[100];
    bool updated = false;

    while (fgets(line, sizeof(line), fpointer)) {
        if (line[0] == '-' && strstr(line + 1, player_name)) {
            fprintf(temp_file, "-%s %d\n", player_name, new_score);
            updated = true;
        }
        else {
            fputs(line, temp_file);
        }
    }

    fclose(fpointer);
    fclose(temp_file);

    if (updated) {
        remove(LOG_FILE);
        rename("temp.txt", LOG_FILE);
    }
    else {
        remove("temp.txt");
    }
}

void delete_player_score(const char* player_name) {
    FILE* fpointer = fopen(LOG_FILE, "r");
    if (!fpointer) {
        perror("Greska pri otvaranju datoteke za citanje");
        return;
    }

    FILE* temp_file = fopen("temp.txt", "w");
    if (!temp_file) {
        perror("Greska pri otvaranju privremene datoteke");
        fclose(fpointer);
        return;
    }

    char line[100];
    char current_name[50];
    int score;
    bool deleted = false;

    while (fgets(line, sizeof(line), fpointer)) {
        if (line[0] == '-' && sscanf(line + 1, "%49s %d", current_name, &score) == 2) {
            if (strcmp(current_name, player_name) == 0) {
                deleted = true;
                continue; 
            }
        }
        fputs(line, temp_file);
    }

    fclose(fpointer);
    fclose(temp_file);

    if (deleted) {
        remove(LOG_FILE);
        rename("temp.txt", LOG_FILE);
    }
    else {
        remove("temp.txt");
    }
}


int is_name_available(const char* name) {
    FILE* fpointer = fopen(LOG_FILE, "r");
    if (!fpointer) {
        return 1;
    }

    char line[100];
    while (fgets(line, sizeof(line), fpointer)) {
        if (line[0] == '-') {
            char existing_name[20];
            if (sscanf(line + 1, "%19s", existing_name) == 1) {
                if (strcmp(existing_name, name) == 0) {
                    fclose(fpointer);
                    return 0;
                }
            }
        }
    }

    fclose(fpointer);
    return 1;
}

void add_player_score(const char* player_name, int score) {
    FILE* fpointer = fopen(LOG_FILE, "a");
    if (!fpointer) {
        perror("Greska pri otvaranju datoteke za pisanje");
        return;
    }
    if (is_name_available(player_name)) {
        fprintf(fpointer, "-%s %d\n", player_name, score);
    }
    fclose(fpointer);
}