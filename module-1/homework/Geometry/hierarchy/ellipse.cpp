#include "ellipse.h"

Ellipse::Ellipse(const Point& first_focus, const Point& second_focus, double dist): 
    first_focus(first_focus), second_focus(second_focus) {
        major_axes = dist / 2;
        foc_dist = Point::pointDistance(first_focus, second_focus) / 2;
        minor_axes = sqrt(major_axes * major_axes - foc_dist * foc_dist);
}

std::pair<Point, Point> Ellipse::focuses() const {
    return {first_focus, second_focus};
}

std::pair<Line, Line> Ellipse::directrices() const {
    double constant;
    if (major_axes > minor_axes) 
        constant = major_axes / eccentricity();
    else
        constant = minor_axes / eccentricity();
    Line left = {{-constant, 0}, {-constant, 1}};
    Line right = {{constant, 0}, {constant, 1}};
    return {left, right};
}

double Ellipse::eccentricity() const {
    return foc_dist / major_axes;
}

Point Ellipse::center() const {
    Point ans = first_focus + second_focus;
    ans.x /= 2;
    ans.y /= 2;
    return ans;
}

double Ellipse::perimeter() {
    return 4 * (Constants::PI * major_axes * minor_axes + major_axes - minor_axes)
         / (major_axes + minor_axes);
}

double Ellipse::area() {
    return Constants::PI * major_axes * minor_axes;
}

bool Ellipse::isCongruentTo(const Shape& another) {
    const Ellipse* another_ellipse = dynamic_cast<const Ellipse*>(&another);
    if (another_ellipse != nullptr) {
        if ((fabs(major_axes - another_ellipse->major_axes) < Constants::EPS) ||
            (fabs(minor_axes - another_ellipse->minor_axes) < Constants::EPS))
            return true;    
    } 
    return false;
}

bool Ellipse::isSimilarTo(const Shape& another) {
    const Ellipse* another_ellipse = dynamic_cast<const Ellipse*>(&another);
    if (another_ellipse != nullptr) {
        double diff = major_axes / another_ellipse->major_axes;
        if (fabs(minor_axes - diff * another_ellipse->minor_axes) < Constants::EPS)
            return true;    
    } 
    return false;
}

bool Ellipse::containsPoint(Point point) {
    double dist_to_first_focus = Point::pointDistance(first_focus, point);
    double dist_to_second_focus = Point::pointDistance(second_focus, point);
    return (dist_to_first_focus + dist_to_second_focus) <= 2 * major_axes;
}

void Ellipse::rotate(Point center, double angle) {
    angle = angle * Constants::PI / 180;
    std::vector<Point> vertices = {first_focus, second_focus};
    for (size_t i = 0; i < vertices.size(); i++) {
        double x = vertices[i].x - center.x;
        double y = vertices[i].y - center.y;
        Point ans(x * cos(angle) - y * sin(angle), x * sin(angle) + y * cos(angle));
        vertices[i] = ans + center;
    }
    first_focus = vertices[0];
    second_focus = vertices[1];
}

void Ellipse::reflex(Point center) {
    std::vector<Point> vertices = {first_focus, second_focus};
    for (size_t i = 0; i < vertices.size(); i++) {
        vertices[i].x = 2 * center.x - vertices[i].x;
        vertices[i].y = 2 * center.y - vertices[i].y;
    }
    first_focus = vertices[0];
    second_focus = vertices[1];
}

void Ellipse::reflex(Line axis) {
    std::vector<Point> vertices = {first_focus, second_focus};
    double axis_a_coeff = axis.get_first_point().y - axis.get_second_point().y;
    double axis_b_coeff = axis.get_second_point().x - axis.get_first_point().x;
    double axis_c_coeff = -(axis_a_coeff * axis.get_first_point().x + axis_b_coeff * axis.get_first_point().y);
    Point axis_normal(axis_a_coeff, axis_b_coeff);
    double axis_len = Point::pointDistance(axis_normal, {0, 0});
    axis_normal.x /= axis_len;
    axis_normal.y /= axis_len;
    for (size_t i = 0; i < vertices.size(); i++) {
        double orient_dist = axis_a_coeff * vertices[i].x + axis_b_coeff * vertices[i].y + axis_c_coeff;
        orient_dist /= Point::pointDistance({axis_a_coeff, axis_b_coeff}, {0, 0});
        Point vec_nomal = axis_normal;
        vec_nomal.x *= orient_dist;
        vec_nomal.y *= orient_dist;
        Point center = vertices[i] - vec_nomal;
        vertices[i].x = 2 * center.x - vertices[i].x;
        vertices[i].y = 2 * center.y - vertices[i].y;
    }
    first_focus = vertices[0];
    second_focus = vertices[1];
}

void Ellipse::scale(Point center, double coefficient) {
    std::vector<Point> vertices = {first_focus, second_focus};
    for (size_t i = 0; i < vertices.size(); i++) {
        vertices[i].x = coefficient * (vertices[i].x - center.x) + center.x;
        vertices[i].y = coefficient * (vertices[i].y - center.y) + center.y;
    }
    first_focus = vertices[0];
    second_focus = vertices[1];
    major_axes *= coefficient;
    minor_axes *= coefficient;
    foc_dist *= coefficient;
}
    
bool Ellipse::operator==(const Shape& another) {
    const Ellipse* another_ellipse = dynamic_cast<const Ellipse*>(&another);
    if (another_ellipse != nullptr) {
        if (((first_focus == another_ellipse->first_focus && second_focus == another_ellipse->second_focus) ||
            (second_focus == another_ellipse->first_focus && first_focus == another_ellipse->second_focus)) &&
            (fabs(major_axes - another_ellipse->major_axes) < Constants::EPS))
            return true;    
    } 
    return false;
}