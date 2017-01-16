//
// Created by laeri on 1/16/17.
//

#ifndef RER_TREE_POINT_H
#define RER_TREE_POINT_H


class Point {
public:
    float x,y;
    Point();
    Point(float x,float y);
    Point& operator=(const Point &point);
    bool operator==(const Point &point);
    bool operator!=(const Point &point);
};


#endif //RER_TREE_POINT_H
