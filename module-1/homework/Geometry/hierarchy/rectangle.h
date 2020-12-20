#include "polygon.h"

class Rectangle: public Polygon {
public:
    Rectangle(const Point& right_bot_diag_point, const Point& left_top_diag_point, double coeff);

    Point center();

    std::pair<Line, Line> diagonals();
};