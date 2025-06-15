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

int compare_names(const void* a, const void* b) {
    const player_score* pa = (const player_score*)a;
    const player_score* pb = (const player_score*)b;
    return strcmp(pa->name, pb->name);
}

void search_player_by_name(player_score* scores, int score_count) {
    char search_name[20];
    printf("Unesite ime igraca za pretrazivanje: ");
    scanf("%19s", search_name);

    qsort(scores, score_count, sizeof(player_score), compare_names);

    player_score key;
    strncpy(key.name, search_name, sizeof(key.name));
    key.score = 0;

    player_score* result = (player_score*)bsearch(&key, scores, score_count,
        sizeof(player_score), compare_names);

    if (result) {
        printf("\nPronadjen igrac:\n");
        printf("Igrac: %-20s | Pobjede: %d\n", result->name, result->score);
    }
    else {
        printf("\nIgrac '%s' nije pronadjen.\n", search_name);
    }
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
                printf("\n\t*** REZULTATI ***\n");
                int display_count = score_count > 5 ? 5 : score_count;
                for (int i = 0; i < display_count; i++) {
                    printf("%2d. Igrac: %-20s | Pobjede: %d\n",
                        i + 1, scores[i].name, scores[i].score);
                }

                if (score_count > 5) {
                    printf("\n Zelite li pretraziti igraca izvan Top 5 liste?\n");
                    printf(" Pritisnite [D] za da.\n");
                    printf(" Za povratak na upravljanje rezultatima, pritisnite bilo koju drugu tipku.\n\n");

                    char choice;
                    scanf(" %c", &choice);
                    if (choice == 'D' || choice == 'd') {
                        search_player_by_name(scores, score_count);
                    }
                }

                free(scores);
                scores = NULL;
            }
        }
        fclose(fpointer);
    }
}

void update_player_score(const char* player_name, int new_score) {
    if (new_score < 0) {
        printf("Broj pobjeda ne moze biti negativan!\n");
        return;
    }

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
    char current_name[20];
    int current_score;
    int updated = 0;

    while (fgets(line, sizeof(line), fpointer)) {
        if (line[0] == '-' && sscanf(line + 1, "%19s %d", current_name, &current_score) == 2) {
            if (strcmp(current_name, player_name) == 0) {
                fprintf(temp_file, "-%s %d\n", player_name, new_score);
                updated = 1;
            }
            else {
                fputs(line, temp_file);
            }
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
        printf("Rezultat za igraca '%s' uspjesno azuriran!\n", player_name);
    }
    else {
        remove("temp.txt");
        printf("Igrac '%s' nije pronadjen u datoteci!\n", player_name);
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
    int deleted = 0;

    while (fgets(line, sizeof(line), fpointer)) {
        if (line[0] == '-' && sscanf(line + 1, "%49s %d", current_name, &score) == 2) {
            if (strcmp(current_name, player_name) == 0) {
                deleted = 1;
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
    if (score < 0) {
        printf("Broj pobjeda ne moze biti negativan!\n");
        return;
    }

    FILE* fpointer = fopen(LOG_FILE, "a");
    if (!fpointer) {
        perror("Greska pri otvaranju datoteke za pisanje");
        return;
    }

    fprintf(fpointer, "-%s %d\n", player_name, score);
    fclose(fpointer);
}