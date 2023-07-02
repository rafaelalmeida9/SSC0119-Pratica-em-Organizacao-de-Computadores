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

#define BOARD_WIDTH 7 // please put a number <= 9, otherwise the planet will be in danger
#define BOARD_HEIGHT 6

#define CONSECUTIVE_NO 4 // 4 consecutive equal numbers is the win condition
#define NO_DIRECTIONS 4

#include <stdio.h>

char game = 1;
char board[BOARD_HEIGHT][BOARD_WIDTH];
char row, player = '0';
int directions[NO_DIRECTIONS][2] = {{1, 1}, {1, 0}, {0, 1}, {-1, 1}};

void outchar_ij_offset(int i, int j, int char_code) {
    outchar_ij(i + OFFSET_Y, j + OFFSET_X, char_code);
}

void outchar_ij(int i, int j, int char_code) {
    int pos = i * SCREEN_WIDTH_RES + j;
    outchar(char_code, pos);
}

void display_board() {
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            uint16_t color = WHITE;
            if (board[i][j] == '1') {
                color = YELLOW;
            } else if (board[i][j] == '0') {
                color = GREEN;
            }
            outchar_ij_offset(i, j, board[i][j] + color);
        }
    }
    for (int i = 1; i <= BOARD_WIDTH; i++) {
        outchar_ij_offset(6, i - 1, i + '0');
    }
}

void display_msg(int start_i, int start_j, char* msg, uint16_t color) {

    for (int i = 0; *msg != '\0'; msg++) {
        outchar_ij(start_i, start_j, (*msg) + color);
        start_j++;
        if (start_j == SCREEN_WIDTH_RES) {
            start_j = 0;
            start_i++;
        }
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

int inside_board(int i, int j) {
    return (0 <= i) && (i < BOARD_HEIGHT) && (0 <= j) && (j < BOARD_WIDTH);
}

// Bit da paridade: 0 se nao ha winner, 1 se ha
// Prox bit: se bit da paridade eh 0, eh 0. Se nao, informa 0 se 0 ganha e 1 se 1 ganha
int check_win_direction_pos(int i, int j, int* d_is, int* d_js) {
    char curr_pos_val = board[i][j];

    if (curr_pos_val == '_') return 0;

    for (int k = 1; k < CONSECUTIVE_NO; k++) {
        int new_i = i + d_is[k];
        int new_j = j + d_js[k];

        if (!inside_board(new_i, new_j) || board[new_i][new_j] != curr_pos_val) {
            return 0;
        }
    }

    return 1 + ((int)(curr_pos_val - '0') << 1);
}

int check_win_direction_board(int* d_is, int* d_js) {
    int did_i_win = 0;

    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            did_i_win = check_win_direction_pos(i, j, d_is, d_js);
            if (did_i_win != 0) {
                return did_i_win;
            }
        }
    }

    return 0;
}

void print_winner(int vencedor) { printf("O jogador %d venceu!\n", vencedor >> 1); }

void directionize(int* pos_xs, int* pos_ys, int dx, int dy) {
    for (int i = 0; i < CONSECUTIVE_NO; i++) {
        pos_xs[i] = dx * i;
        pos_ys[i] = dy * i;
    }
}

int check_win_direction(int dx, int dy) {
    int positions_x[CONSECUTIVE_NO], positions_y[CONSECUTIVE_NO];
    directionize(positions_x, positions_y, dx, dy);
    int win_code = check_win_direction_board(positions_x, positions_y);

    if (win_code & 1) {
        print_winner(win_code & 2);
        return win_code;
    }

    return 0;
}

int verify_winner() {
    int win_code = 0;
    for (int i = 0; i < NO_DIRECTIONS; i++) {
        win_code = check_win_direction(directions[i][0], directions[i][1]);
        if (win_code) {
            game = 0;
            return win_code;
        }
    }
    return 0;
}

void toggle_player() {
    if (player == '0') {
        player = '1';
    } else {
        player = '0';
    }
}

void setup() {
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            board[i][j] = '_';
        }
    }
}

void loop() {
    if (!game) {
        halt();
    }

    uint16_t current_char = inchar();
    int win_code = 0;
    if (current_char != SENTINEL_KEYPRESSED_VALUE &&
        ('1' <= current_char && current_char <= '0' + BOARD_WIDTH)) {
        place_object(current_char - '1');
        win_code = verify_winner();
        toggle_player();
    }

    display_board();

    if (!game && (win_code & 1)) {
        char msg_vitoria[] = "O jogador %d venceu!";
        sprintf(msg_vitoria, msg_vitoria, (win_code >> 1));
        display_msg(OFFSET_Y - 2, OFFSET_X - strlen(msg_vitoria) / 2 + 3, msg_vitoria, PURPLE);
    }
}