#include "rectangle.h"

Rectangle::Rectangle(const Point& right_bot_diag_point, const Point& left_top_diag_point, double coeff) {
    //write code
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