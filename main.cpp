#include <iostream>
#include <SDL.h>
#include "rer_tree/Point.h"


int main() {
    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 640;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    } else {
        //Create window
        window = SDL_CreateWindow("Gridlight", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                  SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL) {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        }
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_Rect tmp_rect;

    bool quit = false;
    SDL_Event e;
    Point current_mouse;
    Point rect_begin;
    bool ctrl_down = false;
    bool draw_mouse_rect = false;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_KEYDOWN:
                    if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
                        quit = true;
                    } else if (e.key.keysym.sym == SDLK_RCTRL || e.key.keysym.sym == SDLK_LCTRL) {
                        ctrl_down = true;
                    }
                    break;
                case SDL_KEYUP:
                    if (e.key.keysym.sym == SDLK_RCTRL || e.key.keysym.sym == SDLK_LCTRL) {
                        ctrl_down = false;
                    }
                    break;
                case SDL_MOUSEMOTION:
                    current_mouse.x = e.motion.x;
                    current_mouse.y = e.motion.y;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (ctrl_down) {
                        draw_mouse_rect = true;
                        int t_x, t_y;
                        SDL_GetMouseState(&t_x, &t_y);
                        rect_begin.x = t_x;
                        rect_begin.y = t_y;
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    draw_mouse_rect = false;
                    break;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (draw_mouse_rect) {
            tmp_rect.w = current_mouse.x - rect_begin.x;
            tmp_rect.h = current_mouse.y - rect_begin.y;
            tmp_rect.x = rect_begin.x;
            tmp_rect.y = rect_begin.y;
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
            SDL_RenderDrawRect(renderer, &tmp_rect);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}