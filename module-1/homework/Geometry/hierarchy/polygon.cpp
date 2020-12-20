#include "polygon.h"

Polygon::Polygon(): vertices({}) {}

Polygon::Polygon(std::vector<Point>& vertices): vertices(vertices) {}

std::vector<Point> Polygon::getVertices() const {
    return vertices;
}

size_t Polygon::verticesCount() const {
    return vertices.size();
}

double Polygon::perimeter() {
    double ans = 0;
    for (size_t i = 0; i < verticesCount(); i++)
        ans += Point::pointDistance(vertices[i], vertices[(i + 1) % verticesCount()]);
    return ans;
}

double Polygon::area() {
    double ans = 0;
    for (size_t i = 0; i < verticesCount(); i++)
        ans += (vertices[(i + 1) % verticesCount()].x - vertices[i].x) 
                * (vertices[(i + 1) % verticesCount()].y + vertices[i].y);
    return fabs(ans) / 2;
}

bool Polygon::isSimilar(const Shape& another, bool need_to_diff) {
    const Polygon* another_polygon = dynamic_cast<const Polygon*>(&another);
    if (another_polygon != nullptr) {
        if (verticesCount() != another_polygon->verticesCount())
            return 0;
        double diff = 1;
        size_t n = verticesCount();
        std::vector<Point> another_vertices = another_polygon->getVertices();
        for (size_t i = 0; i < n; i++) {
            for (size_t j = 0; j < n; j++) {
                int sg_t = 1;
                int sg_a = 1;
                for (size_t q = 0; q < 4; q++) {
                    bool ans = 1;
                    if (need_to_diff)
                        diff = Point::pointDistance(vertices[i], vertices[(i + sg_t + n) % n]) / 
                                Point::pointDistance(another_vertices[j], another_vertices[(j + sg_a + n) % n]);
                    for (size_t k = 0; k < n; k++) {
                        double dist_this = Point::pointDistance(vertices[(i + sg_t * k + n) % n], 
                                            vertices[(i + sg_t + sg_t * k + n) % n]);
                        double dist_other = Point::pointDistance(another_vertices[(j + sg_a * k + n) % n], 
                                            another_vertices[(j + sg_a + sg_a * k + n) % n]);
                        Point cur_this = vertices[(i + sg_t * k + n) % n];
                        Point next_this = vertices[(i + sg_t + sg_t * k + n) % n];
                        Point prev_this = vertices[(i - sg_t + sg_t * k + n) % n];
                        Point cur_another = another_vertices[(j + sg_a * k + n) % n];
                        Point next_another = another_vertices[(j + sg_a + sg_a * k + n) % n];
                        Point prev_another = another_vertices[(j - sg_a + sg_a * k + n) % n];
                        double angle_this = atan2(Point::crossProduct(cur_this - prev_this, next_this - cur_this), 
                                            Point::scalarProduct(cur_this - prev_this, next_this - cur_this));
                        double angle_other = atan2(Point::crossProduct(cur_another - prev_another, next_another - cur_another), 
                                            Point::scalarProduct(cur_another - prev_another, next_another - cur_another));
                        if (fabs(angle_this - angle_other) >= Constants::EPS) {
                            ans = 0;
                            break;
                        }
                        if (fabs(dist_this - dist_other * diff) >= Constants::EPS) {
                            ans = 0;
                            break;
                        }
                    }
                    if (ans)
                        return true;
                    if (q + 1 > 1)
                        sg_a = -1;
                    else
                        sg_a = 1;
                    if ((q + 1) % 2 == 0)
                        sg_t = 1;
                    else
                        sg_t = -1;
                }
            }
        }
    }
    return false;
}

bool Polygon::isCongruentTo(const Shape& another) {
    return isSimilar(another, false);
}

bool Polygon::isSimilarTo(const Shape& another) {
    return isSimilar(another, true);
}

bool Polygon::containsPoint(Point point) {
    double ans = 0;
    std::vector<Point> n_point = getVertices();
    for (size_t i = 0; i < verticesCount(); i++)
        n_point[i] = n_point[i] - point;
    for (size_t i = 0; i < verticesCount(); i++) {
        ans += atan2(Point::crossProduct(n_point[i], n_point[(i + 1) % verticesCount()]), 
                Point::scalarProduct(n_point[i], n_point[(i + 1) % verticesCount()]));
    }
    return !(fabs(ans) < Constants::EPS);
}

signed char Polygon::sign(double value) {
    if (fabs(value) < Constants::EPS)
        return 0;
    if (value > 0)
        return 1;
    return -1;
}

bool Polygon::isConvex() {
    bool ans = 1;
    double prev = 0;
    for (size_t i = 0; i < verticesCount(); i++) {
        double cur = Point::crossProduct(vertices[(i + 1) % verticesCount()] - vertices[i], 
                    vertices[(i + 2) % verticesCount()] - vertices[(i + 1) % verticesCount()]);
        ans &= (sign(cur) == sign(prev) || static_cast<int>(sign(cur)) * static_cast<int>(sign(prev)) == 0);
        prev = cur;
    }
    return ans;
}

void Polygon::rotate(Point center, double angle) {
    angle = angle * Constants::PI / 180;
    for (size_t i = 0; i < verticesCount(); i++) {
        double x = vertices[i].x - center.x;
        double y = vertices[i].y - center.y;
        Point ans(x * cos(angle) - y * sin(angle), x * sin(angle) + y * cos(angle));
        vertices[i] = ans + center;
    }
}

void Polygon::reflex(Point center) {
    for (size_t i = 0; i < verticesCount(); i++) {
        vertices[i].x = 2 * center.x - vertices[i].x;
        vertices[i].y = 2 * center.y - vertices[i].y;
    }
}

void Polygon::reflex(Line axis) {
    double axis_a_coeff = axis.get_first_point().y - axis.get_second_point().y;
    double axis_b_coeff = axis.get_second_point().x - axis.get_first_point().x;
    double axis_c_coeff = -(axis_a_coeff * axis.get_first_point().x + axis_b_coeff * axis.get_first_point().y);
    Point axis_normal(axis_a_coeff, axis_b_coeff);
    double axis_len = Point::pointDistance(axis_normal, {0, 0});
    axis_normal.x /= axis_len;
    axis_normal.y /= axis_len;
    for (size_t i = 0; i < verticesCount(); i++) {
        double orient_dist = axis_a_coeff * vertices[i].x + axis_b_coeff * vertices[i].y + axis_c_coeff;
        orient_dist /= Point::pointDistance({axis_a_coeff, axis_b_coeff}, {0, 0});
        Point vec_nomal = axis_normal;
        vec_nomal.x *= orient_dist;
        vec_nomal.y *= orient_dist;
        Point center = vertices[i] - vec_nomal;
        vertices[i].x = 2 * center.x - vertices[i].x;
        vertices[i].y = 2 * center.y - vertices[i].y;
    }
}

void Polygon::scale(Point center, double coefficient) {
    for (size_t i = 0; i < verticesCount(); i++) {
        vertices[i].x = coefficient * (vertices[i].x - center.x) + center.x;
        vertices[i].y = coefficient * (vertices[i].y - center.y) + center.y;
    }
}

bool Polygon::operator==(const Shape& another) {
    const Polygon* another_polygon = dynamic_cast<const Polygon*>(&another);
    if (another_polygon != nullptr) {
        if (verticesCount() != another_polygon->verticesCount())
            return 0;
        size_t n = verticesCount();
        std::vector<Point> another_vertices = another_polygon->getVertices();
        for (size_t i = 0; i < n; i++) {
            for (size_t j = 0; j < n; j++) {
                int sg_t = 1;
                int sg_a = 1;
                for (size_t q = 0; q < 4; q++) {
                    bool ans = 1;
                    for (size_t k = 0; k < n; k++) 
                        if (vertices[(i + sg_t * k + n) % n] != another_vertices[(j + sg_a * k + n) % n]) {
                            ans = 0;
                            break;
                        }
                    if (ans)
                        return true;
                    if (q + 1 > 1)
                        sg_a = -1;
                    else
                        sg_a = 1;
                    if ((q + 1) % 2 == 0)
                        sg_t = 1;
                    else
                        sg_t = -1;
                }
            }
        }
    }
    return false;
}