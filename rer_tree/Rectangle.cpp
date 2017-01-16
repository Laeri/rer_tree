//
// Created by laeri on 1/16/17.
//

#include "Rectangle.h"

Rectangle::Rectangle() {}

Rectangle::Rectangle(float x_min, float x_max, float y_min, float y_max) : min(x_min, y_min), max(x_max, y_max) {}

Rectangle::Rectangle(Point min, Point max) : min(min), max(max) {}

bool ccw(Point a, Point b, Point c) {
    float slope_comp = (b.y - a.y) * (c.x - b.x) - (c.y - b.y) * (b.x - a.x);
    return slope_comp < 0;
}

bool segment_intersection(Point a, Point b, Point c, Point d) {
    return (ccw(a, b, c) + ccw(a, b, d) == 1 && ccw(c, d, b) + ccw(c, d, a) == 1);
}

bool Rectangle::intersect(Point from, Point to) {
    Point rect_from = Point(min.x, min.y);
    Point rect_to = Point(max.x, min.y);
    bool i_a = segment_intersection(rect_from, rect_to, from, to);
    rect_from = rect_to;
    rect_to.x = max.x;
    rect_to.y = max.y;
    bool i_b = segment_intersection(rect_from, rect_to, from, to);
    rect_from = rect_to;
    rect_to.x = min.x;
    rect_to.y = max.y;
    bool i_c = segment_intersection(rect_from, rect_to, from, to);
    rect_from = rect_to;
    rect_to.x = min.x;
    rect_to.y = min.y;
    bool i_d = segment_intersection(rect_from, rect_to, from, to);
    return i_a || i_b || i_c || i_d;
}