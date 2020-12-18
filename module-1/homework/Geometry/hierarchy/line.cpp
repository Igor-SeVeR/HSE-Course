#include "line.h"

Line::Line(): first({0, 0}), second({0, 0}) {}

Line::Line(const Point& first, const Point& second): first(first), second(second) {}

Point Line::get_first_point() const {
    return first;
}

Point Line::get_second_point() const {
    return second;
}

bool operator==(const Line& left, const Line& right) {
    double left_a_coeff = left.get_first_point().y - left.get_second_point.y;
    double left_b_coeff = left.get_second_point().x - left.get_first_point().x;
    double left_c_coeff = -(left_a_coeff * left.get_first_point().x + left_b_coeff * left.get_first_point().y);
    double ans_first_point = left_a_coeff * right.get_first_point().x
                             + left_b_coeff * right.get_first_point().y + left_c_coeff;
    double ans_second_point = left_a_coeff * right.get_second_point().x
                             + left_b_coeff * right.get_second_point().y + left_c_coeff;
    if (fabs(ans_first_point) < Consts::EPS && fabs(ans_second_point) < Consts::EPS)
        return true;
    return false;
}

bool operator!=(const Line& left, const Line& right) {
    return !(left == right);
}