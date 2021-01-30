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
