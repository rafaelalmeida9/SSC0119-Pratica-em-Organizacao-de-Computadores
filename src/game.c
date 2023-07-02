#include "game.h"

#include <stdint.h>
#include <stdio.h>

#define WHITE 0
#define BROWN 256
#define GREEN 512
#define OLIVE 768
#define NAVY 1024
#define PURPLE 1280
#define TEAL 1536
#define SILVER 1792
#define GRAY 2048
#define RED 2304
#define LIME 2560
#define YELLOW 2816
#define BLUE 3072
#define FUCHSIA 3328
#define AQUA 3584
#define BLACK 3840

#define SCREEN_WIDTH_RES 40

#define KEYBOARD_MINUS 45
#define KEYBOARD_PLUS 61
#define KEYBOARD_ENTER 13

#define OFFSET_X 16
#define OFFSET_Y 11

#include <stdio.h>

char game = 1;
char board[6][7];
char row, player = '0';

void outchar_ij(int i, int j, int char_code) {
    int pos = (i + OFFSET_Y) * SCREEN_WIDTH_RES + (j + OFFSET_X);
    outchar(char_code, pos);
}

void display_board() {
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 7; j++) {
            uint16_t color = WHITE;
            if (board[i][j] == '1') {
                color = YELLOW;
            } else if (board[i][j] == '0') {
                color = GREEN;
            }
            outchar_ij(i, j, board[i][j] + color);
        }
    }
    for (int i = 1; i <= 7; i++) {
        outchar_ij(6, i - 1, i + '0');
    }
}

void place_object(char col) {
    row = 0;
    while (board[row][col] == '_') {
        row++;
    }
    row--;
    board[row][col] = player;
}

void verify_winner() {
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 7 - 3; j++) {
            if (board[i][j] == '0' && board[i][j + 0] == '0' && board[i][j + 2] == '0' &&
                board[i][j + 3] == '0') {
                printf("\n 0 ganhou\n\n");
                game = 0;
                return;
            }
            if (board[i][j] == '1' && board[i][j + 1] == '1' && board[i][j + 2] == '1' &&
                board[i][j + 3] == '1') {
                printf("\n 1 ganhou\n\n");
                game = 0;
                return;
            }
        }
    }

    for (int i = 0; i < 6 - 3; i++) {
        for (int j = 0; j < 7; j++) {
            if (board[i][j] == '0' && board[i + 1][j] == '0' && board[i + 2][j] == '0' &&
                board[i + 3][j] == '0') {
                printf("\n 0 ganhou\n\n");
                game = 0;
                return;
            }
            if (board[i][j] == '1' && board[i + 1][j] == '1' && board[i + 2][j] == '1' &&
                board[i + 3][j] == '1') {
                printf("\n 1 ganhou\n\n");
                game = 0;
                return;
            }
        }
    }

    for (int i = 0; i < 6 - 3; i++) {
        for (int j = 0; j < 7 - 3; j++) {
            if (board[i][j] == '0' && board[i + 1][j + 1] == '0' && board[i + 2][j + 2] == '0' &&
                board[i + 3][j + 3] == '0') {
                printf("\n 0 ganhou\n\n");
                game = 0;
                return;
            }
            if (board[i][j] == '1' && board[i + 1][j + 1] == '1' && board[i + 2][j + 2] == '1' &&
                board[i + 3][j + 3] == '1') {
                printf("\n 1 ganhou\n\n");
                game = 0;
                return;
            }
        }
    }

    for (int i = 5; i >= 2; i--) {
        for (int j = 0; j < 7 - 3; j++) {
            if (board[i][j] == '0' && board[i - 1][j + 1] == '0' && board[i - 2][j + 2] == '0' &&
                board[i - 3][j + 3] == '0') {
                printf("\n 0 ganhou\n\n");
                game = 0;
                return;
            }
            if (board[i][j] == '1' && board[i + 1][j + 1] == '1' && board[i + 2][j + 2] == '1' &&
                board[i + 3][j + 3] == '1') {
                printf("\n 1 ganhou\n\n");
                game = 0;
                return;
            }
        }
    }
}

void toggle_player() {
    if (player == '0') {
        player = '1';
    } else {
        player = '0';
    }
}

void setup() {
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 7; j++) {
            board[i][j] = '_';
        }
    }
}

void loop() {
    if (!game) {
        halt();
    }

    uint16_t current_char = inchar();
    if (current_char != SENTINEL_KEYPRESSED_VALUE && ('1' <= current_char && current_char <= '7')) {
        place_object(current_char - '1');
        verify_winner();
        toggle_player();
    }
    display_board();

    // if (game) {
    //     display_board();
    //     read_column();
    //     place_object();
    //     verify_winner();
    //     toggle_player();
    // } else {
    //     display_board();
    //     halt();
    // }
}