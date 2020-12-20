#include "circle.h"

Circle::Circle(const Point& center, double radius): Ellipse(center, center, radius * 2) {}

double Circle::radius() const {
    return major_axes;
}

std::vector<Point> Circle::intersect(const Circle& circle, const Line& line) {
    double dist_to_line = fabs(line.getXCoeff() * circle.center().x + line.getYCoeff() * circle.center().y + line.getCCoeff()) /
                Point::pointDistance({line.getXCoeff(), line.getYCoeff()}, {0, 0});
    if (dist_to_line > circle.radius()) 
        return {};
    Point line_diff = {line.getXCoeff(), line.getYCoeff()};
    if (Point::scalarProduct(line_diff, circle.center()) + line.getCCoeff() > 0) 
        line_diff = -line_diff;
    line_diff /= Point::pointDistance(line_diff, {0, 0});
    line_diff *= dist_to_line;
    Point final_point = circle.center();
    final_point += line_diff;
    line_diff = {line.getXCoeff(), line.getYCoeff()};
    std::swap(line_diff.x, line_diff.y);
    line_diff.x = -line_diff.x;
    line_diff /= Point::pointDistance(line_diff, {0, 0});
    double coeff = sqrt(circle.radius() * circle.radius() - dist_to_line * dist_to_line);
    line_diff *= coeff;
    if (final_point + line_diff == final_point - line_diff) 
        return {final_point + line_diff};
    else 
        return {final_point + line_diff, final_point - line_diff};
}

std::vector<Point> Circle::intersect(const Circle& first, const Circle& second) {
    if (first.center() == second.center() && first.radius() == second.radius()) 
        return std::vector<Point>(3);
    Circle second_moved(second.center() - first.center(), second.radius());
    auto ans = intersect(second_moved, Line(-2 * second_moved.center().x, -2 * second_moved.center().y,
        second_moved.center().x * second_moved.center().x + second_moved.center().y * second_moved.center().y 
        + first.radius() * first.radius() - second_moved.radius() * second_moved.radius()));
    for (size_t i = 0; i < ans.size(); i++)
        ans[i] += first.center();
    return ans;
}