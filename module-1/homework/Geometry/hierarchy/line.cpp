#include "line.h"

Line::Line(): first({0, 0}), second({0, 0}) {
    declare_coefficients();
}

Line::Line(const Point& first, const Point& second): first(first), second(second) {
    declare_coefficients();
}

Point Line::get_first_point() const {
    return first;
}

Point Line::get_second_point() const {
    return second;
}

bool operator==(const Line& left, const Line& right) {
    double ans_first_point = left.x_coeff * right.get_first_point().x
                             + left.y_coeff * right.get_first_point().y + left.c_coeff;
    double ans_second_point = left.x_coeff * right.get_second_point().x
                             + left.y_coeff * right.get_second_point().y + left.c_coeff;
    if (fabs(ans_first_point) < Constants::EPS && fabs(ans_second_point) < Constants::EPS)
        return true;
    return false;
}

bool operator!=(const Line& left, const Line& right) {
    return !(left == right);
}

void Line::declare_coefficients() {
    x_coeff = get_first_point().y - get_second_point().y;
    y_coeff = get_second_point().x - get_first_point().x;
    c_coeff = -(x_coeff * get_first_point().x + y_coeff * get_first_point().y);
}

double Line::getXCoeff() const {
    return x_coeff;
}

double Line::getYCoeff() const {
    return y_coeff;
}

double Line::getCCoeff() const {
    return c_coeff;
}

std::vector<Point> Line::intersect(const Line& line1, const Line& line2) {
    Point l1_normal = {line1.getXCoeff(), line1.getYCoeff()};
    Point l2_normal = {line2.getXCoeff(), line2.getYCoeff()};
    if (fabs(Point::crossProduct(l1_normal, l2_normal)) <= Constants::EPS) {
        if (line1 == line2)
            return std::vector<Point>(2);
        return {};
    }
    if (fabs(line2.getXCoeff()) <= Constants::EPS)
        return intersect(line2, line1);
    if (fabs(line1.getXCoeff()) <= Constants::EPS) {
        double y = -line1.getCCoeff() / line1.getYCoeff();
        double x = (-line2.getYCoeff() * y - line2.getCCoeff()) / line2.getXCoeff();
        return {Point(x, y)};
    } else {
        double nb = line2.getYCoeff() - line1.getYCoeff() * line2.getXCoeff() / line1.getXCoeff();
        double nc = line2.getCCoeff() - line1.getCCoeff() * line2.getXCoeff() / line1.getXCoeff();
        double y = -nc / nb;
        double x = (-line2.getYCoeff() * y - line2.getCCoeff()) / line2.getXCoeff();
        return {Point(x, y)};
    }
}