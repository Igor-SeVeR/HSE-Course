#include "list.h"
#include "point.h"

class Shape {
public:
    virtual double perimeter();
    virtual double area();
    
    virtual bool isCongruentTo(const Shape& another);
    virtual bool isSimilarTo(const Shape& another);
    virtual bool containsPoint(Point point);

    virtual void rotate(Point center, double angle);
    virtual void reflex(Point center);
    virtual void reflex(Line axis);
    virtual void scale(Point center, double coefficient);
     
    virtual bool operator==(const Shape& another);
}