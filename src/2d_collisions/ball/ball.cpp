#include "ball.h"

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
