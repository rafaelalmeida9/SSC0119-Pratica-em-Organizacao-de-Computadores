#pragma once

#include <SDL.h>
#include <stdbool.h>
#include <stdint.h>

#define CHARMAP_DEPTH 1024
#define CHARMAP_WIDTH 8

#define TILE_WIDTH 8
#define TILE_HEIGHT 8
#define TILE_PIXEL_COUNT 64

#define SCREEN_WIDTH_RES 40
#define SCREEN_HEIGHT_RES 30

#define TILE_SIDE_LENGTH 32
#define CHANNELS 3

#define WIN_WIDTH (SCREEN_WIDTH_RES * TILE_SIDE_LENGTH)
#define WIN_HEIGHT (SCREEN_HEIGHT_RES * TILE_SIDE_LENGTH)

#define SENTINEL_KEYPRESSED_VALUE 255

extern int sys_keypressed_buffer;
extern bool sys_charmap_pattern_buffer[CHARMAP_DEPTH * CHARMAP_WIDTH];
extern SDL_Surface *sys_screen_surface;
extern SDL_Renderer *sys_renderer;
extern SDL_Texture *sys_texture;
extern uint8_t sys_pixels[WIN_WIDTH * WIN_HEIGHT * 4];
extern bool sys_halted;

typedef struct {
    uint8_t r, g, b;
} sys_Color;

void sys_draw_pixel(int x, int y, sys_Color *color);

uint16_t inchar();
void outchar(int char_code, int position);
void halt();