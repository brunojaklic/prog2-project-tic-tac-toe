#define _CRT_SECURE_NO_WARNINGS
#include "tictactoe.h"

char symbol = 'X';
int count;

int main() {
	player_score players[2] = { {"", 0}, {"", 0} };
	char new_game;
	count = 0;

	printf("\n\t\t\t*** IKS-OKS ***\n\n");
	printf("Pravila:\n");
	printf(" - Postavite tri svoja simbola u jedan red, stupac ili dijagonalu kako biste pobijedili.\n");
	printf(" - Pobjednik prethodne runde zapocinje sljedecu rundu.\n\n");

	printf("Unesite ime igraca 'X': ");
	while (1) {
		char input[100];
		if (scanf("%99s", input) != 1) {
			fprintf(stderr, "Greska pri unosu imena igraca X\n");
			return 1;
		}
		while (getchar() != '\n');

		if (strlen(input) >= 20) {
			printf("Greska: Ime ne smije biti duze od 19 znakova! Unesite ponovo: ");
			continue;
		}

		if (!is_name_available(input)) {
			printf("Ime '%s' je vec zauzeto. Unesite drugo ime: ", input);
			continue;
		}
		strcpy(players[0].name, input);
		break;
	}

	printf("Unesite ime igraca 'O': ");
	while (1) {
		char input[100];
		if (scanf("%99s", input) != 1) {
			fprintf(stderr, "Greska pri unosu imena igraca O\n");
			return 1;
		}
		while (getchar() != '\n');

		if (strlen(input) >= 20) {
			printf("Greska: Ime ne smije biti duze od 19 znakova! Unesite ponovo: ");
			continue;
		}

		if (!is_name_available(input) || strcmp(input, players[0].name) == 0) {
			printf("Ime '%s' je vec zauzeto ili je isto kao prvi igrac. Unesite drugo ime: ", input);
			continue;
		}
		strcpy(players[1].name, input);
		break;
	}

	do {
		char board[9] = { '1', '2', '3', '4', '5', '6', '7', '8', '9' };
		display_board(board);

		if (symbol == 'X') {
			printf("%s je na potezu (%c)!\n", players[0].name, symbol);
		}
		else {
			printf("%s je na potezu (%c)!\n", players[1].name, symbol);
		}

		while (1) {
			printf("Igrac 'X' : %s\n", players[0].name);
			printf("Igrac 'O' : %s\n", players[1].name);
			printf("\t\t%s: %d pobjeda\n", players[0].name, players[0].score);
			printf("\t\t%s: %d pobjeda\n", players[1].name, players[1].score);

			count++;
			input_move(board);
			display_board(board);

			GameState result = check_winner(board);
			if (result == X_WON) {
				players[0].score++;
				printf("%s POBJEDJUJE OVU RUNDU!\n", players[0].name);
				break;
			}
			else if (result == O_WON) {
				players[1].score++;
				printf("%s POBJEDJUJE OVU RUNDU!\n", players[1].name);
				break;
			}
			else if (result == DRAW && count == 9) {
				printf("NERIJESENO!\n");
				break;
			}
			toggle_symbol();
			if (symbol == 'X') {
				printf("%s je na potezu (%c)!\n", players[0].name, symbol);
			}
			else {
				printf("%s je na potezu (%c)!\n", players[1].name, symbol);
			}
		}

		count = 0;
		printf("\n\t*** NOVA IGRA? ***\n\n");
		printf(" Pritisnite [D] za pocetak nove igre\n");
		printf(" Pritisnite [N] za zavrsetak igre.\n\n");

		do {
			if (scanf(" %c", &new_game) != 1) {
				return 1;
			}
		} while (new_game != 'd' && new_game != 'D' &&
			new_game != 'n' && new_game != 'N');
	} while (new_game == 'd' || new_game == 'D');

	FILE* fpointer = fopen("rezultati.txt", "a");
	if (fpointer) {
		fprintf(fpointer, "-%s %d\n", players[0].name, players[0].score);
		fprintf(fpointer, "-%s %d\n", players[1].name, players[1].score);
		fclose(fpointer);
	}
	else {
		perror("Neuspjesno spremanje rezultata");
	}

	display_scores();

	MenuOption choice;
	do {
		printf("\n\t*** UPRAVLJANJE REZULTATIMA ***\n");
		printf("%d. Dodaj novog igraca\n", MENU_ADD_SCORE);
		printf("%d. Azuriraj rezultat igraca\n", MENU_UPDATE_SCORE);
		printf("%d. Obrisi rezultat igraca\n", MENU_DELETE_SCORE);
		printf("%d. Izlaz\n", MENU_EXIT);
		printf("Odaberite opciju: ");

		if (scanf("%d", &choice) != 1) {
			while (getchar() != '\n');
			choice = 0;
		}

		char player_name[20];
		int new_score;

		switch (choice) {
		case MENU_ADD_SCORE:
			printf("Unesite ime novog igraca: ");
			while (1) {
				char player_name[100];
				if (scanf("%99s", player_name) != 1) {
					while (getchar() != '\n');
					printf("Neispravan unos. Unesite ime ponovo: ");
					continue;
				}
				while (getchar() != '\n');

				if (strlen(player_name) >= 20) {
					printf("Greska: Ime ne smije biti duze od 19 znakova! Unesite ponovo: ");
					continue;
				}

				if (!is_name_available(player_name)) {
					printf("Ime '%s' je vec zauzeto. Unesite drugo ime: ", player_name);
					continue;
				}

				new_score = input_integer("Unesite broj pobjeda: ");
				add_player_score(player_name, new_score);
				display_scores();
				break;
			}
			break;

		case MENU_UPDATE_SCORE:
			printf("Unesite ime igraca za azuriranje: ");
			while (1) {
				char player_name[100];
				if (scanf("%99s", player_name) != 1) {
					while (getchar() != '\n');
					printf("Neispravan unos. Unesite ime ponovo: ");
					continue;
				}
				while (getchar() != '\n');

				if (strlen(player_name) >= 20) {
					printf("Greska: Ime ne smije biti duze od 19 znakova! Unesite ponovo: ");
					continue;
				}

				new_score = input_integer("Unesite novi broj pobjeda: ");
				update_player_score(player_name, new_score);
				display_scores();
				break;
			}
			break;
		case MENU_DELETE_SCORE:
			printf("Unesite ime igraca za brisanje: ");
			scanf("%19s", player_name);
			delete_player_score(player_name);
			display_scores();
			break;
		case MENU_EXIT:
			printf("Izlaz...\n");
			break;
		default:
			display_scores();
		}
	} while (choice != MENU_EXIT);
	return 0;
}