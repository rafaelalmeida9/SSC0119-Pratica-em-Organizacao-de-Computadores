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

// #define WORD_LEN 5
// #define WORD_MEM_LEN (WORD_LEN + 1)
// #define LAST_CHAR_IDX (WORD_LEN - 1)

// #define WORDS_Y_GAP 1
// #define PLACEHOLDER_CHARACTER_INDEX 31

// #define COLOR_FOR_CURSOR LIME
// #define COLOR_FOR_NOT_PRESENT_LETTER RED
// #define COLOR_FOR_PRESENT_LETTER YELLOW
// #define COLOR_FOR_RIGHT_LETTER GREEN

// #define MAX_N_WORDS 2

// char game_words[MAX_N_WORDS][WORD_MEM_LEN] = {"falso", "gerar"};
// char input_word[WORD_MEM_LEN] = {};
// uint16_t solved[MAX_N_WORDS] = {};
// uint16_t n_words = 0;
// uint16_t current_cursor_index = 0;
// uint16_t current_line_index = 4;

// void decrement_cursor() {
//     if (current_cursor_index != 0) {
//         current_cursor_index--;
//     }
// }

// void increment_cursor() {
//     if (current_cursor_index != LAST_CHAR_IDX) {
//         current_cursor_index++;
//     }
// }

// uint16_t get_base_position(uint16_t word_index) {
//     int gap = (SCREEN_WIDTH_RES - WORD_LEN * n_words) / (n_words + 1);
//     return (gap * (word_index + 1) + WORD_LEN * word_index +
//             current_line_index * SCREEN_WIDTH_RES);
// }

// void show_char(uint16_t word_index, uint16_t letter_index, uint16_t char_code,
//                uint16_t offset) {
//     uint16_t positon = get_base_position(word_index) + offset + letter_index;
//     outchar(char_code, positon);
// }

// void show_placeholders() {
//     uint16_t current_word = 0;

//     while (current_word < n_words) {
//         if (solved[current_word]) {
//             current_word++;
//             continue;
//         }

//         uint16_t current_letter = 0;
//         while (current_letter < WORD_LEN) {
//             uint16_t char_code = PLACEHOLDER_CHARACTER_INDEX;
//             if (current_letter == current_cursor_index) {
//                 char_code += COLOR_FOR_CURSOR;
//             }
//             show_char(current_word, current_letter, char_code, SCREEN_WIDTH_RES);
//             current_letter += 1;
//         }

//         current_word++;
//     }
// }

// void receive_letter(uint16_t char_code) {
//     input_word[current_cursor_index] = char_code;

//     uint16_t current_word = 0;
//     while (current_word < n_words) {
//         if (solved[current_word]) {
//             current_word++;
//             continue;
//         }
//         show_char(current_word, current_cursor_index, char_code, 0);
//         current_word++;
//     }

//     increment_cursor();
// }

// bool has_letter(int word_index, uint16_t char_code) {
//     uint16_t current_letter = 0;
//     while (current_letter < WORD_LEN) {
//         if (game_words[word_index][current_letter] == char_code) {
//             return true;
//         }
//         current_letter++;
//     }
//     return false;
// }

// void on_submit() {
//     uint16_t current_word = 0;
//     while (current_word < n_words) {
//         if (solved[current_word]) {
//             current_word++;
//             continue;
//         }

//         uint16_t right_count = 0;
//         uint16_t current_letter = 0;
//         while (current_letter < WORD_LEN) {
//             uint16_t char_code = input_word[current_letter];
//             uint16_t color = 0;
//             if (has_letter(current_word, char_code)) {
//                 if (game_words[current_word][current_letter] == char_code) {
//                     right_count++;
//                     color = COLOR_FOR_RIGHT_LETTER;
//                 } else {
//                     color = COLOR_FOR_PRESENT_LETTER;
//                 }
//             } else {
//                 color = COLOR_FOR_NOT_PRESENT_LETTER;
//             }

//             show_char(current_word, current_letter, color + char_code, 0);
//             show_char(current_word, current_letter,
//                       color + PLACEHOLDER_CHARACTER_INDEX, SCREEN_WIDTH_RES);
//             current_letter++;

//             if (right_count == WORD_LEN) {
//                 solved[current_word] = true;
//             }
//         }
//         current_word++;
//     }

//     current_cursor_index = 0;
//     current_line_index += 1 + WORDS_Y_GAP;
// }

// void setup() {}

// uint16_t posicao = 0;

// void loop() {
//     uint16_t char_atual = inchar();

//     if (char_atual != SENTINEL_KEYPRESSED_VALUE) {
//         outchar(char_atual, posicao++);
//     }
// }

#include <stdio.h>

char game = 1;
char board[6][7];
char row, player = '0';

void outchar_ij(int i, int j, char char_code) {
    int pos = i * SCREEN_WIDTH_RES + j;
    outchar(char_code, pos);
}

void display_board() {
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 7; j++) {
            uint16_t color = YELLOW;
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