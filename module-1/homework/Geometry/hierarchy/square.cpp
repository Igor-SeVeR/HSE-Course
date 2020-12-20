#include "square.h"

Square::Square(const Point& right_bot_diag_point, const Point& left_top_diag_point): 
        Rectangle(right_bot_diag_point, left_top_diag_point, 1) {}

Circle Square::circumscribedCircle() {
    Point mid = (vertices[0] + vertices[2]);
    mid.x /= 2;
    mid.y /= 2;
    return Circle(mid, Point::pointDistance(vertices[0], vertices[2]) / 2);
}

Circle Square::inscribedCircle() {
    Point mid = (vertices[0] + vertices[2]);
    mid.x /= 2;
    mid.y /= 2;
    return Circle(mid, Point::pointDistance(vertices[0], vertices[1]) / 2);
}