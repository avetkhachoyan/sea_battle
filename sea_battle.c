#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define BOARD_SIZE 5
#define NUM_SHIPS 3

typedef struct {
    int x;
    int y;
} Coordinates;

typedef struct {
    char board[BOARD_SIZE][BOARD_SIZE];
} GameBoard;

void initialize_board(GameBoard *board) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board->board[i][j] = '-';
        }
    }
}

void print_board(GameBoard *board) {
    printf("  ");
    for (int i = 0; i < BOARD_SIZE; i++) {
        printf("%d ", i);
    }
    printf("\n");

    for (int i = 0; i < BOARD_SIZE; i++) {
        printf("%d ", i);
        for (int j = 0; j < BOARD_SIZE; j++) {
            printf("%c ", board->board[i][j]);
        }
        printf("\n");
    }
}

bool is_valid_coordinate(int x, int y) {
    return (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE);
}

bool is_valid_placement(GameBoard *board, Coordinates coord) {
    return (board->board[coord.x][coord.y] == '-');
}

void place_ships(GameBoard *board, int num_ships) {
    srand(time(NULL));

    for (int ship = 0; ship < num_ships; ship++) {
        Coordinates coord;
        do {
            coord.x = rand() % BOARD_SIZE;
            coord.y = rand() % BOARD_SIZE;
        } while (!is_valid_placement(board, coord));

        board->board[coord.x][coord.y] = 'S';
    }
}

bool is_game_over(GameBoard *board) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board->board[i][j] == 'S') {
                return false;
            }
        }
    }
    return true;
}

void player_turn(GameBoard *computer_board) {
    int x, y;
    do {
        printf("Enter coordinates to attack (x y): ");
        scanf("%d %d", &x, &y);
    } while (!is_valid_coordinate(x, y));

    if (computer_board->board[x][y] == 'S') {
        printf("Hit!\n");
        computer_board->board[x][y] = 'X';
    } else if (computer_board->board[x][y] == '-') {
        printf("Miss!\n");
        computer_board->board[x][y] = 'O';
    } else {
        printf("You've already fired at this location. Try again.\n");
    }
}

void computer_turn(GameBoard *player_board) {
    int x, y;
    do {
        x = rand() % BOARD_SIZE;
        y = rand() % BOARD_SIZE;
    } while (!is_valid_coordinate(x, y));

    if (player_board->board[x][y] == 'S') {
        printf("Computer hits at (%d, %d)! ", x, y);
        player_board->board[x][y] = 'X';
    } else if (player_board->board[x][y] == '-') {
        printf("Computer misses at (%d, %d). ", x, y);
        player_board->board[x][y] = 'O';
    } else {
        printf("Computer already fired at (%d, %d). ", x, y);
    }
}

int main() {
    GameBoard player_board, computer_board;
    int num_ships_remaining = NUM_SHIPS;

    initialize_board(&player_board);
    initialize_board(&computer_board);

    place_ships(&player_board, NUM_SHIPS);
    place_ships(&computer_board, NUM_SHIPS);

    printf("Welcome to Sea Battle!\n");
    printf("Try to sink the enemy ships!\n");

    while (!is_game_over(&computer_board) && !is_game_over(&player_board)) {
        printf("\nYour board:\n");
        print_board(&player_board);
        printf("\nComputer's board:\n");
        print_board(&computer_board);

        player_turn(&computer_board);
        if (is_game_over(&computer_board)) {
            printf("You win! All enemy ships sunk!\n");
            break;
        }

        computer_turn(&player_board);
        if (is_game_over(&player_board)) {
            printf("Computer wins! All your ships sunk!\n");
            break;
        }
    }

    return 0;
}
