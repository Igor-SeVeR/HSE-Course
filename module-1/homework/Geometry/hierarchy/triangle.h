#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "polygon.h"
#include "circle.h"

class Triangle: public Polygon {
public:
    Triangle(const Point& first, const Point& second, const Point& third);
    Triangle(std::vector<Point>& vertices); 

    Circle circumscribedCircle() const ;
    Circle inscribedCircle() const;

};

#endif