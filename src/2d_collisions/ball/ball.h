#ifndef BALL_H
#define BALL_H

#include <QColor>
#include <math.h>

using Point2d = struct Point2d
{
    double x, y;
    Point2d() {}
    Point2d(double x_, double y_): x(x_), y(y_) {}
};
double length(Point2d p);
double dist(Point2d p1, Point2d p2);
Point2d mid_point(const Point2d& p1, const Point2d& p2);

using Vector2d = struct Vector2d
{
    double x, y;
    Vector2d() {}
    Vector2d(double x_, double y_): x(x_), y(y_) {}
    Vector2d(const Point2d& p1, const Point2d& p2): x(p2.x - p1.x), y(p2.y - p1.y) {}
};
double length(const Vector2d& p);
void normalize(Vector2d& v);
double vector_mult(const Vector2d& v1, const Vector2d& v2);


class Ball
{
public:
    double r = 1;
    Point2d pos;
    Point2d vel;

    Ball();
    Ball(double x, double y, double r);

    QRgb get_color();
    void default_color();
    void collide_color();

    ~Ball();
private:
    QRgb _def_color = Qt::black;
    QRgb _col_color = Qt::red;
    QRgb _color = _def_color;
};

#endif // BALL_H
