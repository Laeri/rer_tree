#include <iostream>
#include <SDL.h>
#include <vector>
#include "rer_tree/Point.h"
#include "rer_tree/Rectangle.h"
#include "rer_tree/rer_tree.h"


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

    rer_tree tree = rer_tree();
    rer_node *start_node = nullptr;
    rer_node *end_node = nullptr;

    bool quit = false;
    SDL_Event e;

    bool started = false;
    bool find_end = true;
    bool direct_line = false;
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
                    } else if (e.key.keysym.sym == SDLK_s) {
                        started = true;
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
                    if (!start_node) {
                        start_node = new rer_node(e.motion.x, e.motion.y);
                        start_node->parent = tree.root;
                        tree.root->add_node(start_node);
                    } else if (!end_node) {
                        end_node = new rer_node(e.motion.x, e.motion.y);
                    } else if (ctrl_down) {
                        draw_mouse_rect = true;
                        rect_begin.x = e.motion.x;
                        rect_begin.y = e.motion.y;
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

        if (started) {

            Point sample;
            bool sample_found = false;
            while (!sample_found) {
                sample_found = true;
                sample.x = std::rand() % SCREEN_WIDTH;
                sample.y = std::rand() % SCREEN_HEIGHT;
                for (auto rect: rects) {
                    if (sample.x > rect.min.x && sample.x < rect.max.x && sample.y > rect.min.y && sample.y < rect.max.y) {
                        sample_found = false;
                    }
                }
            }
            float dx = sample.x;
            float dy = sample.y;
            rer_node *node = tree.findNearest(Point(dx,dy), &rects);
            if (node) {
                if (!direct_line) {
                    float step_size = 10;

                    dx = sample.x - node->pos.x;
                    dy = sample.y - node->pos.y;
                    float len = sqrt(dx * dx + dy * dy);
                    if (len != 0) {
                        dx /= len;
                        dy /= len;
                    } else {
                        dx = dy = 1;
                    }
                    if (step_size > node->distance(sample)) {
                        step_size = node->distance(sample);
                    }
                    dx = node->pos.x + dx * step_size;
                    dy = node->pos.y + dy * step_size;
                }
                rer_node *new_node = new rer_node(dx, dy);
                new_node->parent = node;
                node->children.push_back(new_node);

            }


            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
            drawTree(renderer, tree);

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
        }
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}