//
// Created by laeri on 1/16/17.
//

#include "Rectangle.h"

Rectangle::Rectangle() {}

Rectangle::Rectangle(float x_min, float x_max, float y_min, float y_max) : min(x_min, y_min), max(x_max, y_max) {}

Rectangle::Rectangle(Point min, Point max) : min(min), max(max) {}