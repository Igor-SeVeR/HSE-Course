#ifndef CIRCLE_H
#define CIRCLE_H

#include "ellipse.h"

class Circle: public Ellipse {
public:
    Circle(const Point& center, double radius);

    double radius() const;
    
    static std::vector<Point> intersect(const Circle& circle, const Line& line);
    static std::vector<Point> intersect(const Circle& first, const Circle& second);

};

#endif