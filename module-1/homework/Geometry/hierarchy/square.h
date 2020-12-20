#ifndef SQUARE_H
#define SQUARE_H

#include "rectangle.h"
#include "circle.h"

class Square: public Rectangle {
public:
    Square(const Point& right_bot_diag_point, const Point& left_top_diag_point);

    Circle circumscribedCircle();
    Circle inscribedCircle();

};

#endif