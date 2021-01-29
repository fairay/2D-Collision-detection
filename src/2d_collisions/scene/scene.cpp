#include "scene.h"

using namespace std;

Scene::Scene(size_t n, double r)
{
    _ball_n = n;
    _ball_r = r;
    _init_state();
}

Scene::~Scene() {}


void Scene::show(shared_ptr<QGraphicsScene> &_qscene)
{
    _qscene->clear();
    QPen vel_pen = QPen(QColor(Qt::red));

    for (auto ball : _ball_arr)
    {
        _qscene->addEllipse(ball.pos.x - _ball_r, ball.pos.y - _ball_r,
                            _ball_r*2, _ball_r*2);
        _qscene->addLine(ball.pos.x, ball.pos.y,
                         ball.pos.x + ball.vel.x, ball.pos.y + ball.vel.y,
                         QPen(QColor(Qt::red)));
    }
}

void Scene::update(double dt, upd_t update_type)
{
    for (auto ball : _ball_arr)
    {
        ball.pos.x += ball.vel.x * dt;
        ball.pos.y += ball.vel.y * dt;

        if (ball.pos.x - _ball_r < 0)
            _collide_border(ball, 0, ball.pos.y);
        if (ball.pos.x + _ball_r > _w)
            _collide_border(ball, _w, ball.pos.y);
        if (ball.pos.y - _ball_r < 0)
            _collide_border(ball, ball.pos.x, 0);
        if (ball.pos.y + _ball_r > _h)
            _collide_border(ball, ball.pos.x, _h);
    }

    switch (update_type)
    {
    case NO_CHECK:
        break;
    case BRUTE_FORCE:
        _brute_force();
        break;
    default:
        printf("No such collidion algorithm\n");
        break;
    }
}

void Scene::_init_state()
{
    srand(time(NULL));
    for (size_t i = 0; i < _ball_n; i++)
    {
        double x = _ball_r + (_w - _ball_r*2) * ((double)rand() / RAND_MAX);
        double y = _ball_r + (_h - _ball_r*2) * ((double)rand() / RAND_MAX);
        Ball b(x, y, _ball_r);

        double vel = MAX_SPEED * ((double)rand() / RAND_MAX);
        double angle = 2*3.1415 * ((double)rand() / RAND_MAX);
        b.vel.x = vel * cos(angle);
        b.vel.y = vel * sin(angle);

        _ball_arr.push_back(b);
    }

    this->update(0.0, BRUTE_FORCE);
}

void Scene::_collide_border(Ball& ball, double x, double y)
{
    double d = abs(ball.pos.x - x) + abs(ball.pos.y - y);
    double a = ball.pos.x - x;
    double b = ball.pos.y - y;

    double p1 = (a*b) / (d*d);
    double p2 = (a*a) / (d*d);
    double p3 = (b*b) / (d*d);

    double d1 = ball.vel.y * p1 + ball.vel.x * p2;
    double d2 = ball.vel.x * p1 + ball.vel.y * p3;

    // Update speed
    ball.vel.x -= d1*2;
    ball.vel.y -= d2*2;

    // Update position
    p3 = ball.r - d;
    p1 = p3 * (a/d);
    p2 = p3 * (b/d);

    ball.pos.x += p1;
    ball.pos.y += p2;
}

void Scene::_collide_balls(Ball& ball1, Ball& ball2)
{
    double dist = sqrt(pow(ball1.pos.x - ball2.pos.x, 2) +
                       pow(ball1.pos.y - ball2.pos.y, 2));
    if (dist > ball1.r*2)
        return;

    double vel1 = length(ball1.vel);
    double vel2 = length(ball2.vel);

    double a = ball1.pos.x - ball2.pos.x;
    double b = ball1.pos.y - ball2.pos.y;

    double p1, p2, p3;
    if (vel1 + vel2 > 1e-5)
    {
        p1 = (a*b) / (dist*dist);
        p2 = (a*a) / (dist*dist);
        p3 = (b*b) / (dist*dist);

        double d1 = (ball1.vel.y - ball2.vel.y)*p1 + (ball1.vel.x - ball2.vel.x)*p2;
        double d2 = (ball1.vel.x - ball2.vel.x)*p1 + (ball1.vel.y - ball2.vel.y)*p3;

        ball1.vel.x -= d1;    ball1.vel.y -= d2;
        ball2.vel.x += d1;    ball2.vel.y += d2;
    }

    p3 = ball1.r*2 - dist;
    p1 = p3 * (a/dist);
    p2 = p3 * (b/dist);

    double k;
    if (vel1 + vel2 > 1e-5) k = vel1 / (vel1 + vel2);
    else                    k = 0.5;

    ball1.pos.x += p1*k;        ball1.pos.y += p2*k;
    ball2.pos.x -= p1*(1-k);    ball2.pos.y -= p2*(1-k);
}