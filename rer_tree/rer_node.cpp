//
// Created by laeri on 1/16/17.
//

#include "rer_node.h"
#include "math.h"

rer_node::rer_node() {}

rer_node::rer_node(float x, float y) : pos(x, y) {}

rer_node::rer_node(Point &point) : pos(point.x, point.y) {}

float rer_node::distance(const Point &other) {
    float dx = other.x - pos.x;
    float dy = other.y - pos.y;
    return sqrt(dx * dx + dy * dy);
}

float rer_node::distance(const rer_node &node) {
    float dx = node.pos.x - pos.x;
    float dy = node.pos.y - pos.y;
    return sqrt(dx * dx + dy * dy);
}

void rer_node::add_node(rer_node *node) {
    children.push_back(node);
}

rer_node::~rer_node() {
    for (auto child: children) {
        delete (child);
    }
}