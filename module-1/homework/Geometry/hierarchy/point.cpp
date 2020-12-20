#include "point.h"
#include "consts.h"

Point::Point(): x(0), y(0) {}

Point::Point(double x, double y): x(x), y(y) {}

bool operator==(const Point& left, const Point& right) {
    return (fabs(left.x - right.x) < Constants::EPS && fabs(left.y - right.y) < Constants::EPS);
}

bool operator!=(const Point& left, const Point& right) {
    return !(left == right);
}

Point operator-(const Point& left, const Point& right) {
    return {left.x - right.x, left.y - right.y};
}

Point operator+(const Point& left, const Point& right) {
    return {left.x + right.x, left.y + right.y};
}

Point operator/(const Point& left, double right) {
    return {left.x / right, left.y / right};
}

Point operator*(const Point& left, double right) {
    return {left.x * right, left.y * right};
}

Point& Point::operator+=(const Point& right) {
    *this = *this + right;
    return *this;
}

Point& Point::operator-=(const Point& right) {
    *this = *this - right;
    return *this;
}

Point& Point::operator/=(double right) {
    *this = *this / right;
    return *this;
}

Point& Point::operator*=(double right) {
    *this = *this * right;
    return *this;
}

Point& Point::operator-() const {
    return *(new Point({-this->x, -this->y}));
}

double Point::pointDistance(const Point& left, const Point& right) {
    double quad_ans = (right.x - left.x) * (right.x - left.x)
                     + (right.y - left.y) * (right.y - left.y);
    return sqrt(quad_ans);
}

double Point::crossProduct(const Point& left, const Point& right) {
    return (left.x * right.y - right.x * left.y);
}

double Point::scalarProduct(const Point& left, const Point& right) {
    return (left.x * right.x + left.y * right.y);
}