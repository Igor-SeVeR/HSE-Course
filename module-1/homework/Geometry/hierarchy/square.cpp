#include "square.h"

Square::Square(const Point& right_bot_diag_point, const Point& left_top_diag_point): 
        Rectangle(right_bot_diag_point, left_top_diag_point, 1) {}

Circle Square::circumscribedCircle() const{
    Point mid = (vertices[0] + vertices[2]) / 2;
    return Circle(mid, Point::pointDistance(vertices[0], vertices[2]) / 2);
}

Circle Square::inscribedCircle() const {
    Point mid = (vertices[0] + vertices[2]) / 2;
    return Circle(mid, Point::pointDistance(vertices[0], vertices[1]) / 2);
}