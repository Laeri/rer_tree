#include <iostream>
#include <SDL.h>
#include <vector>
#include "rer_tree/Point.h"
#include "rer_tree/Rectangle.h"


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

    std::vector<Rectangle> rects;
    SDL_Rect tmp_rect;

    bool quit = false;
    SDL_Event e;
    Point current_mouse;
    Point rect_begin;
    bool ctrl_down = false;
    bool draw_mouse_rect = false;
    bool draw_all_rects = true;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYDOWN:
                    if ((e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
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
                    if (draw_mouse_rect) {
                        Rectangle rect = Rectangle(rect_begin, current_mouse);
                        rects.push_back(rect);
                        draw_mouse_rect = false;
                    }
                    break;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        if (draw_mouse_rect) {
            tmp_rect.w = current_mouse.x - rect_begin.x;
            tmp_rect.h = current_mouse.y - rect_begin.y;
            tmp_rect.x = rect_begin.x;
            tmp_rect.y = rect_begin.y;

            SDL_RenderDrawRect(renderer, &tmp_rect);
        }

        if (draw_all_rects) {
            SDL_Rect sdl_rect;
            for (auto &rect: rects) {
                sdl_rect.x = rect.min.x;
                sdl_rect.y = rect.min.y;
                sdl_rect.w = rect.max.x - rect.min.x;
                sdl_rect.h = rect.max.y - rect.min.y;
                SDL_RenderDrawRect(renderer, &sdl_rect);
            }
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}