#include "point.h"

Point::Point(): x(0), y(0) {}

Point::Point(double x, double y): x(x), y(y) {}

bool operator==(const Point& left, const Point& right) {
    return (left.x == right.x && left.y == right.y);
}

bool operator!=(const Point& left, const Point& right) {
    return !(left == right);
}