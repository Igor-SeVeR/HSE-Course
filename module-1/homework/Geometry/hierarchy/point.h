#include <cmath>

struct Point {
    double x;
    double y;

    Point();
    Point(double x, double y);

    friend bool operator==(const Point& left, const Point& right);
    friend bool operator!=(const Point& left, const Point& right);

    friend Point operator-(const Point& left, const Point& right);
    friend Point operator+(const Point& left, const Point& right);

    static double pointDistance(const Point& left, const Point& right);
    static double crossProduct(const Point& left, const Point& right);
    static double scalarProduct(const Point& left, const Point& right);
};