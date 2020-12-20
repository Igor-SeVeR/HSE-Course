#include "point.h"
#include "consts.h"
#include <cmath>

class Line {
public:
    Line();
    Line(const Point& first, const Point& second);

    Point get_first_point() const;
    Point get_second_point() const;

    friend bool operator==(const Line& left, const Line& right);
    friend bool operator!=(const Line& left, const Line& right);

private:
    Point first;
    Point second;
};