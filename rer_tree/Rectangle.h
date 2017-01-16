//
// Created by laeri on 1/16/17.
//

#ifndef RER_TREE_RECTANGLE_H
#define RER_TREE_RECTANGLE_H


#include "Point.h"

class Rectangle {
public:
    Point min,max;
    Rectangle();
    Rectangle(float x_min, float x_max, float y_min, float y_max);
    Rectangle(Point min, Point max);
};


#endif //RER_TREE_RECTANGLE_H
