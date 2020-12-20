#include "point.h"

Point::Point(): x(0), y(0) {}

Point::Point(double x, double y): x(x), y(y) {}

bool operator==(const Point& left, const Point& right) {
    return (left.x == right.x && left.y == right.y);
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

static double pointDistance(const Point& left, const Point& right) {
    double quad_ans = (right.x - left.x) * (right.x - left.x)
                     + (right.y - left.y) * (right.y - left.y);
    return sqrt(quad_ans);
}

static double crossPoint(const Point& left, const Point& right) {
    return (left.x * right.y - right.x * left.y);
}

static double scalarPoint(const Point& left, const Point& right) {
    return (left.x * right.x + left.y * right.y);
}