//
// Created by laeri on 1/16/17.
//

#include "Point.h"

Point::Point() : x(0), y(0) {}

Point::Point(float x, float y) : x(x), y(y) {}

Point &Point::operator=(const Point &point) {
    x = point.x;
    y = point.y;
}

bool Point::operator==(const Point &point) {
    return (x == point.x) && (y == point.y);
}

bool Point::operator!=(const Point &point) {
    return (x != point.x) || (y != point.y);
}