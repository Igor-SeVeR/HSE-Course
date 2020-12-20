#ifndef SHAPE_H
#define SHAPE_H

#include "line.h"
#include "point.h"
#include "consts.h"
#include <vector>

class Shape {
public:
    virtual double perimeter() const = 0;
    virtual double area() const = 0;
    
    virtual bool isCongruentTo(const Shape& another) const = 0;
    virtual bool isSimilarTo(const Shape& another) const = 0;
    virtual bool containsPoint(Point point) const = 0;

    virtual void rotate(Point center, double angle)     = 0;
    virtual void reflex(Point center) = 0;
    virtual void reflex(Line axis) = 0;
    virtual void scale(Point center, double coefficient) = 0;
     
    virtual bool operator==(const Shape& another) const = 0;
};

#endif