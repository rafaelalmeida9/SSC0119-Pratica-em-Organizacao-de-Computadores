#include <SDL.h>
#include <stdbool.h>
#include <stdio.h>

#include "game.h"
#include "system.h"

#define CHARMAP_FILE "charmap.bin"

void load_charmap() {
    FILE *charmap_fp = fopen(CHARMAP_FILE, "rb");
    fread(sys_charmap_pattern_buffer, 1, CHARMAP_DEPTH * CHARMAP_WIDTH, charmap_fp);
    fclose(charmap_fp);

    printf("Loaded Charmap!");
}

int main(int argc, char **argv) {
    // SDL init
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    // create SDL window
    SDL_Window *window = SDL_CreateWindow("sdl2_pixelbuffer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_RESIZABLE);
    if (window == NULL) {
        SDL_Log("Unable to create window: %s", SDL_GetError());
        return 1;
    }

    // create sys_renderer
    sys_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (sys_renderer == NULL) {
        SDL_Log("Unable to create sys_renderer: %s", SDL_GetError());
        return 1;
    }

    SDL_RenderSetLogicalSize(sys_renderer, WIN_WIDTH, WIN_HEIGHT);

    // create sys_texture
    sys_texture = SDL_CreateTexture(sys_renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING,
                                    WIN_WIDTH, WIN_HEIGHT);
    if (sys_texture == NULL) {
        SDL_Log("Unable to create sys_texture: %s", SDL_GetError());
        return 1;
    }

    load_charmap();
    setup();

    // main loop
    bool should_quit = false;
    SDL_Event e;
    while (!should_quit) {
        while (SDL_PollEvent(&e) != 0) {
            switch (e.type) {
                case SDL_QUIT:
                    should_quit = true;
                    break;
                case SDL_KEYDOWN:
                    sys_keypressed_buffer = e.key.keysym.sym;
                    break;
            }
        }

        if (!sys_halted) {
            loop();
        }

        int sys_texture_pitch = 0;
        void *sys_texture_sys_pixels = NULL;
        if (SDL_LockTexture(sys_texture, NULL, &sys_texture_sys_pixels, &sys_texture_pitch) != 0) {
            SDL_Log("Unable to lock sys_texture: %s", SDL_GetError());
        } else {
            memcpy(sys_texture_sys_pixels, sys_pixels, sys_texture_pitch * WIN_HEIGHT);
        }
        SDL_UnlockTexture(sys_texture);

        SDL_RenderClear(sys_renderer);
        SDL_RenderCopy(sys_renderer, sys_texture, NULL, NULL);
        SDL_RenderPresent(sys_renderer);
    }

    SDL_DestroyTexture(sys_texture);
    SDL_DestroyRenderer(sys_renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}