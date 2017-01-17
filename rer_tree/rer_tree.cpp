//
// Created by laeri on 1/16/17.
//

#include <stack>
#include <limits>
#include <SDL_system.h>
#include <iostream>
#include "rer_tree.h"


rer_tree::rer_tree() {
    root = new rer_node();
}

rer_tree::~rer_tree() {
        delete root;
}


bool rer_tree::intersect_tree(Point from, Point to) {
    rer_node *current;

    std::stack<rer_node *> stack;
    for (auto node:root->children) {
        stack.push(node);
    }
    while (!stack.empty()) {
        current = stack.top();
        stack.pop();
        rer_node &n = *current;
        if (current->children.size() > 0) {
            for (auto child:current->children) {
                Point n_from = Point(current->pos);
                Point n_to = Point(child->pos);
                if (n_from != from && n_to != from) {
                    if (segment_intersection(n_from, n_to, from, to)) {
                        return true;
                    }
                }
                stack.push(child);
            }
        }
    }
    return false;
}

rer_node *rer_tree::findNearest(Point pos, std::vector<Rectangle> *rects) {
    float distance = std::numeric_limits<float>::max();
    rer_node *nearest;
    rer_node *current;

    std::stack<rer_node *> stack;
    for (auto node:root->children) {
        stack.push(node);
    }

    while (!stack.empty()) {
        current = stack.top();
        float dist = current->distance(pos);
        if (dist < distance) {
            bool intersect = false;
            Point n_a = Point(current->pos.x, current->pos.y);
            Point n_b = Point(pos);
            for (auto &rect: *rects) {
                if (rect.intersect(n_a, n_b)) intersect = true;
            }
            if (!intersect && !intersect_tree(n_a, n_b)) {
                distance = dist;
                nearest = current;
            }
        }
        stack.pop();
        rer_node &n = *current;
        if (current->children.size() > 0) {
            for (auto child:current->children) {
                stack.push(child);
            }
        }
    }

    return nearest;
}

void drawTree(SDL_Renderer *renderer, rer_tree &tree, SDL_Color& color) {
    rer_node *current;
    rer_node *root = tree.root;
    std::stack<rer_node *> stack;
    for (auto node:root->children) {
        stack.push(node);
    }

    while (!stack.empty()) {
        current = stack.top();
        stack.pop();
        SDL_SetRenderDrawColor(renderer,color.r, color.g, color.b, color.a);
        if (current->colored) {
            SDL_SetRenderDrawColor(renderer, current->r, current->g, current->b, current->a);
        }
        if (current->children.size() > 0) {
            for (auto child:current->children) {
                SDL_RenderDrawLine(renderer, current->pos.x, current->pos.y, child->pos.x, child->pos.y);
                stack.push(child);
            }
        }
    }
}