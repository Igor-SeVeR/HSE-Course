#include "rectangle.h"
#include "circle.h"
#include <string>

Rectangle::Rectangle(const Point& right_bot_diag_point, const Point& left_top_diag_point, double coeff) {
    double diag_length = Point::pointDistance(right_bot_diag_point, left_top_diag_point);
    double length = sqrt(diag_length * diag_length / (1 + coeff * coeff));
    double width = length * coeff;
    auto bottom_inter = Circle::intersect(Circle(left_top_diag_point, length), Circle(right_bot_diag_point, width));
    Point right_top_diag_point = (bottom_inter[0].y > bottom_inter[1].y) ? bottom_inter[0] : bottom_inter[1];
    auto top_inter = Circle::intersect(Circle(left_top_diag_point, width), Circle(right_bot_diag_point, length));
    Point left_bot_diag_point = (top_inter[0].y > top_inter[1].y) ? top_inter[1] : top_inter[0];
    vertices = {left_top_diag_point, left_bot_diag_point, right_bot_diag_point, right_top_diag_point};
}

Point Rectangle::center() {
    Point ans = vertices[0] + vertices[2];
    ans.x /= 2;
    ans.y /= 2;
    return ans;
}

std::pair<Line, Line> Rectangle::diagonals() {
    return {{vertices[0], vertices[2]}, {vertices[1], vertices[3]}};
}