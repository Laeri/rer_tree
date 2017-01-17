//
// Created by laeri on 1/16/17.
//

#ifndef RER_TREE_NODE_H
#define RER_TREE_NODE_H


#include <vector>
#include "Point.h"

class rer_node {
public:
    Point pos;
    int r, g, b, a;
    bool colored = false;
    std::vector<rer_node *> children;
    rer_node *parent = nullptr;

    rer_node();

    ~rer_node();

    rer_node(float x, float y);

    rer_node(Point &point);

    float distance(const rer_node &node);

    float distance(const Point &other);

    void add_node(rer_node *node);
};


#endif //RER_TREE_NODE_H
