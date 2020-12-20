#ifndef POLYGON_H
#define POLYGON_H

#include "shape.h"

class Polygon: public Shape {

public:
    Polygon();
    Polygon(std::vector<Point>& verticecs);

    std::vector<Point> getVertices() const;
    
    size_t verticesCount() const;

    double perimeter() override;
    double area() override;
    
    bool isCongruentTo(const Shape& another) override;
    bool isSimilarTo(const Shape& another) override;
    bool containsPoint(Point point) override;
    bool isConvex();

    void rotate(Point center, double angle) override;
    void reflex(Point center) override;
    void reflex(Line axis) override;
    void scale(Point center, double coefficient) override;
     
    bool operator==(const Shape& another) override;

protected:
    std::vector<Point> vertices;

private:
    static signed char sign(double value);
    bool isSimilar(const Shape& another, bool need_to_diff);
};

#endif