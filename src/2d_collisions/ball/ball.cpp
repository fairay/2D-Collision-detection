#include "ball.h"

double length(Point2d p)
{
    return sqrt(p.x*p.x + p.y*p.y);
}
double dist(Point2d p1, Point2d p2)
{
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}
Point2d mid_point(const Point2d& p1, const Point2d& p2)
{
    return Point2d((p1.x+p2.x)/2, (p1.y+p2.y)/2);
}

double length(const Vector2d& p)
{
    return sqrt(p.x*p.x + p.y*p.y);
}
void normalize(Vector2d& v)
{
    double d = length(v);
    if (!d) return;
    v.x /= d;
    v.y /= d;
}
double vector_mult(const Vector2d& v1, const Vector2d& v2)
{
    return v1.x*v2.y - v1.y*v2.x;
}

Ball::Ball() {}
Ball::Ball(double x, double y, double _r)
{
    pos.x = x;
    pos.y = y;
    r = _r;
}

QRgb Ball::get_color() { return _color; }
void Ball::default_color() { _color = _def_color; }
void Ball::collide_color() { _color = _col_color; }

Ball::~Ball() {}
