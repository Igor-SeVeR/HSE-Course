#include "triangle.h"

Triangle::Triangle(const Point& first, const Point& second, const Point& third): 
        Polygon({first, second, third}) {}

Triangle::Triangle(std::vector<Point>& vertices): Polygon(vertices) {}

Circle Triangle::inscribedCircle() {
    Point v1 = vertices[1] - vertices[0];
    Point v2 = vertices[2] - vertices[0];
    double v1_len = Point::pointDistance(v1, {0, 0});
    double v2_len = Point::pointDistance(v2, {0, 0});
    v1.x /= v1_len;
    v1.y /= v1_len;
    v2.x /= v2_len;
    v2.y /= v2_len;
    Point p1 = (v1 + v2);
    p1.x /= 2;
    p1.y /= 2;
    p1 = p1 + vertices[0];
    v1 = vertices[0] - vertices[1];
    v2 = vertices[2] - vertices[1];
    v1_len = Point::pointDistance(v1, {0, 0});
    v2_len = Point::pointDistance(v2, {0, 0});
    v1.x /= v1_len;
    v1.y /= v1_len;
    v2.x /= v2_len;
    v2.y /= v2_len;
    Point p2 = (v1 + v2);
    p2.x /= 2;
    p2.y /= 2;
    p2 = p2 + vertices[1];
    Point c_in = Line::intersect(Line(vertices[0], p1), Line(vertices[1], p2))[0];
    double radius = area() / (perimeter() / 2);
    return Circle(c_in, radius);
}

Circle Triangle::circumscribedCircle() {
    Point v1 = vertices[1] - vertices[0];
    std::swap(v1.x, v1.y);
    v1.x *= -1;
    Point v2 = vertices[2] - vertices[1];
    std::swap(v2.x, v2.y);
    v2.x *= -1;
    Point p1 = (vertices[1] + vertices[0]);
    p1.x /= 2;
    p1.y /= 2;
    Point p2 = (vertices[1] + vertices[2]);
    p2.x /= 2;
    p2.y /= 2;
    Point c_out = Line::intersect(Line(p1, p1 + v1), Line(p2, p2 + v2))[0];
    double a = Point::pointDistance(vertices[0], vertices[1]);
    double b = Point::pointDistance(vertices[1], vertices[2]);
    double c = Point::pointDistance(vertices[0], vertices[2]);
    double radius = a * b * c / (4 * area());
    return Circle(c_out, radius);
}