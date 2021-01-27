#include "ball.h"

Ball::Ball() {}
Ball::Ball(const Point2d p): pos(p) {}
Ball::Ball(double x, double y)
{
    pos.x = x;
    pos.y = y;
}

Ball::~Ball() {}
