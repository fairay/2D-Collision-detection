#ifndef BALL_H
#define BALL_H

using Point2d = struct
{
    double x, y;
};

class Ball
{
public:
    Point2d pos;

    Ball();
    Ball(const Point2d p);
    Ball(double x, double y);

    ~Ball();
};

#endif // BALL_H
