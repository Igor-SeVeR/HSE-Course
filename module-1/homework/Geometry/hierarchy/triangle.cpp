#include "triangle.h"

Triangle::Triangle(const Point& first, const Point& second, const Point& third): 
        Polygon({first, second, third}) {}

Triangle::Triangle(std::vector<Point>& vertices): Polygon(vertices) {}

Circle Triangle::inscribedCircle() const {
    Point first_edge = vertices[1] - vertices[0];
    Point second_edge = vertices[2] - vertices[0];
    first_edge /= Point::pointDistance(first_edge, {0, 0});
    second_edge /= Point::pointDistance(second_edge, {0, 0});
    Point first_biss = vertices[0] + (first_edge + second_edge) / 2;
    first_edge = vertices[0] - vertices[1];
    second_edge = vertices[2] - vertices[1];
    first_edge /= Point::pointDistance(first_edge, {0, 0});
    second_edge /= Point::pointDistance(second_edge, {0, 0});
    Point second_biss = vertices[1] + (first_edge + second_edge) / 2;
    Point center_in_circle = Line::intersect(Line(vertices[0], first_biss), Line(vertices[1], second_biss))[0];
    double radius = area() / (perimeter() / 2);
    return Circle(center_in_circle, radius);
}

Circle Triangle::circumscribedCircle() const {
    Point first_edge = vertices[1] - vertices[0];
    std::swap(first_edge.x, first_edge.y);
    first_edge.x *= -1;
    Point second_edge = vertices[2] - vertices[1];
    std::swap(second_edge.x, second_edge.y);
    second_edge.x *= -1;
    Point first_mid_perpend = (vertices[1] + vertices[0]) / 2;
    Point second_mid_perpend = (vertices[1] + vertices[2]) / 2;
    Point center_out_circle = Line::intersect(Line(first_mid_perpend, first_mid_perpend + first_edge), 
                                                Line(second_mid_perpend, second_mid_perpend + second_edge))[0];
    double first_side = Point::pointDistance(vertices[0], vertices[1]);
    double second_side = Point::pointDistance(vertices[1], vertices[2]);
    double third_side = Point::pointDistance(vertices[0], vertices[2]);
    double radius = first_side * second_side * third_side / (4 * area());
    return Circle(center_out_circle, radius);
}