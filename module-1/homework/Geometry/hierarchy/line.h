#ifndef LINE_H
#define LINE_H

#include "point.h"
#include "consts.h"
#include <cmath>
#include <vector>

class Line {
public:
    Line();
    Line(const Point& first, const Point& second);

    Point get_first_point() const;
    Point get_second_point() const;

    friend bool operator==(const Line& left, const Line& right);
    friend bool operator!=(const Line& left, const Line& right);

    double getXCoeff() const;
    double getYCoeff() const;
    double getCCoeff() const;

    static std::vector<Point> intersect(const Line& line1, const Line& line2);

private:
    Point first;
    Point second;
    double x_coeff;
    double y_coeff;
    double c_coeff;

    void declare_coefficients();
};

#endif