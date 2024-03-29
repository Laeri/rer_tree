#include <iostream>
#include <SDL.h>
#include <vector>
#include <numeric>
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
        window = SDL_CreateWindow("Rapidly Exploring Random Tree", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                  SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL) {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        }
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    std::vector<Rectangle> rects;
    SDL_Rect tmp_rect;

    rer_tree *tree = new rer_tree();
    rer_node *start_node = nullptr;
    rer_node *end_node = nullptr;

    // lines of the tree edges will have this color
    SDL_Color tree_color;
    tree_color.a = 255;
    tree_color.r = 0;
    tree_color.g = 0;
    tree_color.b = 255;

    SDL_Color rect_color;
    rect_color.a = 255;
    rect_color.r = 255;
    rect_color.g = 255;
    rect_color.b = 0;

    bool quit = false;
    SDL_Event e;

    // start expanding the tree
    // you can start / stop by pressing the 's' key
    bool started = false;

    /*if find_end is not set, the tree expands in all directions
     * without looking for the end point. (For taking screenshots) */
    bool find_end = true;

    // either increment in new direction or draw a direct line from node to sample
    bool direct_line = false;

    // used for drawing the obstacles (rectangles)
    Point current_mouse;
    Point rect_begin;
    bool ctrl_down = false;

    // toggle visualizing of obstacles (rectangles) and current rectangle drawing
    bool draw_mouse_rect = false;
    bool draw_all_rects = true;
    // fill rectangles (true) or only draw outlines (false)
    // toggle by pressing the 'f' key
    bool fill_rects = true;


    while (!quit) {
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYDOWN:
		    // quit
                    if ((e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) || e.key.keysym.sym == SDLK_q){
                        quit = true;
                    } else if (e.key.keysym.sym == SDLK_RCTRL || e.key.keysym.sym == SDLK_LCTRL) {
                        ctrl_down = true;
                    } else if (e.key.keysym.sym == SDLK_s) { // start / stop
                        started = started ? false : true;
                    } else if (e.key.keysym.sym == SDLK_d) { // show or hide rectangles
                        draw_all_rects = draw_all_rects ? false : true;
                    } else if (e.key.keysym.sym == SDLK_r) { // reset everything
                        started = false;
                        delete tree;
                        tree = new rer_tree();
                        start_node = nullptr;
                        end_node = nullptr;
                        ctrl_down = false;
                        rects.clear();
                        continue;
                    } else if (e.key.keysym.sym == SDLK_f) { // fill or draw outlines of rectangles
			fill_rects = fill_rects ? false: true;
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
                    // when clicking the first two times, the start and end nodes are set
                    if (!start_node) {
                        start_node = new rer_node(e.motion.x, e.motion.y);
                        start_node->parent = tree->root;
                        tree->root->add_node(start_node);
                        continue;
                    } else if (!end_node) {
                        end_node = new rer_node(e.motion.x, e.motion.y);
                        continue;
                    } else if (ctrl_down) {
                        // if ctrl is down a new rectangle should be drawn (and created when the mouse is up again
                        draw_mouse_rect = true;
                        rect_begin.x = e.motion.x;
                        rect_begin.y = e.motion.y;
                    } else {
                        // otherwise when mouse is clicked the tree expands to the new position
                        rer_node *node = tree->findNearest(Point(e.motion.x, e.motion.y), &rects);
                        if (node) {
                            rer_node *new_node = new rer_node(e.motion.x, e.motion.y);
                            new_node->parent = node;
                            node->add_node(new_node);
                        }
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    // create a new obstacle
                    if (draw_mouse_rect) {
                        Rectangle rect = Rectangle(rect_begin, current_mouse);
                        rects.push_back(rect);
                        draw_mouse_rect = false;
                    }
                    break;
            }
        }

        if (started) {

            if (find_end) {
                // try to connect directly, color and stop the simulation
                rer_node *node = tree->findNearest(end_node->pos, &rects);
                if (node) {
                    end_node->parent = node;
                    node->children.push_back(end_node);
                    started = false;
                    rer_node *parent = end_node;
                    while ((parent = parent->parent)) {
                        parent->r = 255;
                        parent->g = 0;
                        parent->b = 0;
                        parent->a = 255;
                        parent->colored = true;
                    }
                    continue;
                }
            }

            // find a new direction in which the tree should be expanded
            Point sample;
            bool sample_found = false;
            while (!sample_found) {
                sample_found = true;
                sample.x = std::rand() % SCREEN_WIDTH;
                sample.y = std::rand() % SCREEN_HEIGHT;
                // check whether sample is inside an obstacle (rectangle)
                for (auto rect: rects) {
                    if (sample.x > rect.min.x && sample.x < rect.max.x && sample.y > rect.min.y &&
                        sample.y < rect.max.y) {
                        sample_found = false;
                    }
                }
            }
            float dx = sample.x;
            float dy = sample.y;
            rer_node *node = tree->findNearest(Point(dx, dy), &rects);
            if (node) {
                // if no direct line should be drawn, the tree is expanded a delta length into the new direction
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
        }

        // clear screen with black
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        drawTree(renderer, *tree, tree_color);

        SDL_SetRenderDrawColor(renderer, rect_color.r, rect_color.g, rect_color.b, rect_color.a);

        if (draw_all_rects) {
            // draw current mouse rectangle
            if (draw_mouse_rect) {
                tmp_rect.w = current_mouse.x - rect_begin.x;
                tmp_rect.h = current_mouse.y - rect_begin.y;
                tmp_rect.x = rect_begin.x;
                tmp_rect.y = rect_begin.y;
		if (fill_rects) {
		    SDL_RenderFillRect(renderer, &tmp_rect);
 		} else {
                    SDL_RenderDrawRect(renderer, &tmp_rect);
		}
            }
            // draw all obstacles
            SDL_Rect sdl_rect;
            for (auto &rect: rects) {
                sdl_rect.x = rect.min.x;
                sdl_rect.y = rect.min.y;
                sdl_rect.w = rect.max.x - rect.min.x;
                sdl_rect.h = rect.max.y - rect.min.y;
		if (fill_rects){
		    SDL_RenderFillRect(renderer, &sdl_rect);
		} else {
                    SDL_RenderDrawRect(renderer, &sdl_rect);
		}
            }
        }

        // draw box around start and end point
        if (find_end) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_Rect sdl_rect;
            int length = 10;
            if (start_node) {
                sdl_rect.x = start_node->pos.x - length;
                sdl_rect.y = start_node->pos.y - length;
                sdl_rect.w = 2 * length;
                sdl_rect.h = 2 * length;
                SDL_RenderDrawRect(renderer, &sdl_rect);
            }
            if (end_node) {
                sdl_rect.x = end_node->pos.x - length;
                sdl_rect.y = end_node->pos.y - length;
                sdl_rect.w = 2 * length;
                sdl_rect.h = 2 * length;
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
