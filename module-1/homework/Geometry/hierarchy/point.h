struct Point {
    double x;
    double y;

    Point();
    Point(double x, double y);

    friend bool operator==(const Point& left, const Point& right);
    friend bool operator!=(const Point& left, const Point& right);
};