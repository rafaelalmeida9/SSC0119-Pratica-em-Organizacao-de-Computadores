#include "system.h"

bool sys_charmap_pattern_buffer[CHARMAP_DEPTH * CHARMAP_WIDTH];
int sys_keypressed_buffer = SENTINEL_KEYPRESSED_VALUE;
uint8_t sys_pixels[WIN_WIDTH * WIN_HEIGHT * 4] = {0};
SDL_Renderer *sys_renderer;
SDL_Texture *sys_texture;
SDL_Surface *sys_screen_surface = NULL;
bool sys_halted = false;

sys_Color sys_colors[] = {
    /* WHITE */ {255, 255, 255},
    /* BROWN */ {165, 42, 42},
    /* GREEN */ {0, 255, 0},
    /* OLIVE */ {120, 130, 30},
    /* NAVY */ {30, 50, 130},
    /* PURPLE */ {132, 42, 120},
    /* TEAL */ {0, 127, 127},
    /* SILVER */ {178, 178, 178},
    /* GRAY */ {188, 188, 188},
    /* RED */ {255, 0, 0},
    /* LIME */ {198, 198, 50},
    /* YELLOW */ {255, 255, 0},
    /* BLUE */ {0, 0, 255},
    /* FUCHSIA */ {255, 27, 170},
    /* AQUA */ {120, 200, 135},
    /* BLACK */ {0, 0, 0},
};

void sys_draw_pixel(int pix_x, int pix_y, sys_Color *color) {
    int pixel_real_size = TILE_SIDE_LENGTH / TILE_WIDTH;

    for (int x = pix_x * pixel_real_size; x < (pix_x + 1) * pixel_real_size; x++) {
        for (int y = pix_y * pixel_real_size; y < (pix_y + 1) * pixel_real_size; y++) {
            int pix = y * WIN_WIDTH + x;
            sys_pixels[pix * 4 + 0] = color->r;
            sys_pixels[pix * 4 + 1] = color->g;
            sys_pixels[pix * 4 + 2] = color->b;
            sys_pixels[pix * 4 + 3] = 255;
        }
    }
}

uint16_t inchar() {
    int tmp = sys_keypressed_buffer;
    sys_keypressed_buffer = SENTINEL_KEYPRESSED_VALUE;
    return tmp;
}

void outchar(int content, int position) {
    static sys_Color color, black = {0, 0, 0};

    int color_code = (content & 0x0F00) >> 8;
    int char_code = content & 0x0FF;

    color = sys_colors[color_code];
    // printf("rgb=(%d,%d,%d)\n", color.r, color.g, color.b);

    int tile_y = position / SCREEN_WIDTH_RES;
    int tile_x = position % SCREEN_WIDTH_RES;

    int base_pix_y = tile_y * TILE_HEIGHT;
    int base_pix_x = tile_x * TILE_WIDTH;

    int base_char_idx = char_code * TILE_PIXEL_COUNT;

    for (int i = 0; i < TILE_WIDTH; i++) {
        int pix_x = base_pix_x + i;
        for (int j = 0; j < TILE_HEIGHT; j++) {
            int pix_y = base_pix_y + j;
            bool is_on = sys_charmap_pattern_buffer[base_char_idx + j * TILE_WIDTH + i];
            if (is_on) {
                sys_draw_pixel(pix_x, pix_y, &color);
            } else {
                sys_draw_pixel(pix_x, pix_y, &black);
            }
        }
    }
}

void halt() { sys_halted = true; }