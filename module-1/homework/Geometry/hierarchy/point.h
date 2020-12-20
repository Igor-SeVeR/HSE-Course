#ifndef POINT_H
#define POINT_H

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
    Point& operator-=(const Point& right);
    Point& operator+=(const Point& right);

    friend Point operator/(const Point& left, double right);
    friend Point operator*(const Point& left, double right);
    Point& operator/=(double right);
    Point& operator*=(double right);

    Point& operator-() const;


    static double pointDistance(const Point& left, const Point& right);
    static double crossProduct(const Point& left, const Point& right);
    static double scalarProduct(const Point& left, const Point& right);
};

#endif