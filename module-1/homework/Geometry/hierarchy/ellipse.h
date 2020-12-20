#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "shape.h"

class Ellipse: public Shape {
public:
    Ellipse(const Point& first_focus, const Point& second_focus, double dist);
    
    std::pair<Point, Point> focuses() const;
    std::pair<Line, Line> directrices() const;
    double eccentricity() const;
    Point center() const;

    double perimeter() const override;
    double area() const override;
    
    bool isCongruentTo(const Shape& another) const override;
    bool isSimilarTo(const Shape& another) const override;
    bool containsPoint(Point point) const override;

    void rotate(Point center, double angle) override;
    void reflex(Point center) override;
    void reflex(Line axis) override;
    void scale(Point center, double coefficient) override;
     
    bool operator==(const Shape& another) const override;

protected:
    Point first_focus;
    Point second_focus;
    double major_axes;
    double minor_axes;
    double foc_dist;

};

#endif