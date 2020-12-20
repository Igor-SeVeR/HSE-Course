#include "circle.h"

Circle::Circle(const Point& center, double radius): Ellipse(center, center, radius * 2) {}

double Circle::radius() const {
    return major_axes;
}

std::vector<Point> Circle::intersect(const Circle& circle, const Line& line) {
    double d = fabs(line.getXCoeff() * circle.center().x + line.getYCoeff() * circle.center().y + line.getCCoeff()) /
                Point::pointDistance({line.getXCoeff(), line.getYCoeff()}, {0, 0});
    if (d > circle.radius()) 
        return {};
    Point dn = {line.getXCoeff(), line.getYCoeff()};
    if (Point::scalarProduct(dn, circle.center()) + line.getCCoeff() > 0) {
        dn.x = -dn.x;
        dn.y = -dn.y;
    }
    double dn_sqrlen = Point::pointDistance(dn, {0, 0});
    dn.x /= dn_sqrlen;
    dn.y /= dn_sqrlen;
    dn.x *= d;
    dn.y *= d;
    Point p = circle.center();
    p = p + dn;
    dn = {line.getXCoeff(), line.getYCoeff()};
    std::swap(dn.x, dn.y);
    dn.x = -dn.x;
    dn_sqrlen = Point::pointDistance(dn, {0, 0});
    dn.x /= dn_sqrlen;
    dn.y /= dn_sqrlen;
    double kek = sqrt(circle.radius() * circle.radius() - d * d);
    dn.x *= kek;
    dn.y *= kek;
    if (p + dn == p - dn) {
        return {p + dn};
    } else {
        return {p + dn, p - dn};
    }
}

std::vector<Point> Circle::intersect(const Circle& first, const Circle& s) {
    if (first.center() == s.center() && first.radius() == s.radius()) 
        return std::vector<Point>(3);
    Circle second(s.center() - first.center(), s.radius());
    auto ans = intersect(second, Line(-2 * second.center().x, -2 * second.center().y,
        second.center().x * second.center().x + second.center().y * second.center().y + first.radius() * first.radius() - second.radius() * second.radius()));
    for (auto &p : ans)
        p = p + first.center();
    return ans;
}