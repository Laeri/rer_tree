//
// Created by laeri on 1/16/17.
//

#ifndef RER_TREE_RER_TREE_H
#define RER_TREE_RER_TREE_H


#include "rer_node.h"
#include "Rectangle.h"

class rer_tree {
public:
    rer_tree();
    rer_node *root;
    rer_node *findNearest(Point pos, std::vector<Rectangle> *rects);
    bool intersect_tree(Point from, Point to);
    ~rer_tree();
};

void drawTree(SDL_Renderer *renderer, rer_tree &tree);

#endif //RER_TREE_RER_TREE_H
