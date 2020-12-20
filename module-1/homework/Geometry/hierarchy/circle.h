#ifndef CIRCLE_H
#define CIRCLE_H

#include "ellipse.h"

class Circle: public Ellipse {
public:
    Circle(const Point& center, double radius);

    double radius();
    
};

#endif