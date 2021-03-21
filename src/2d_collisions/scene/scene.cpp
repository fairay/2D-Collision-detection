#include "scene.h"
#include "algorithms/base_tree.h"
#include <thread>
#include <iostream>

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
                         ball.pos.x + ball.vel.x/10, ball.pos.y + ball.vel.y/10,
                         QPen(QColor(Qt::red)));
    }

    if (_alg)
        _alg->show(_qscene);
}

void Scene::upd_ball(Ball& ball, double dt)
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

void Scene::update(double dt, upd_t update_type, bool is_threading)
{
    _alg = shared_ptr<BaseTree>(nullptr);

    if (false)  // is_threading
        _upd_pos_mul(dt);
    else
        _upd_pos(dt);

    switch (update_type)
    {
    case NO_CHECK:
        break;
    case BRUTE_FORCE:
        _brute_force(is_threading);
        break;
    case QUAD_TREE:
        _quad_tree(is_threading);
        break;
    case DYNAMIC_QUAD_TREE:
        _dynamic_quad_tree(is_threading);
        break;
    case HEXA_TREE:
        _hexa_tree(is_threading);
        break;
    case BIN_TREE:
        _bin_tree(is_threading);
        break;
    case BIN_ALG:
        _bin_alg(is_threading);
        break;
    default:
        printf("No such collision algorithm\n");
        break;
    }
}
void Scene::_upd_pos(double dt)
{
    for (size_t i=0; i<_ball_n; i++)
        upd_ball(_ball_arr[i], dt);
}

void thread_upd_pos(Scene* scene, vector<Ball>* ball_arr, int from_, int to_, double dt)
{
    for (int i=from_; i<to_; i++)
        scene->upd_ball((*ball_arr)[i], dt);
}
void Scene::_upd_pos_mul(double dt)
{
    int thread_n = ADD_THREAD_N;
    array<thread, ADD_THREAD_N> thread_arr;
    int ball_n = _ball_arr.size();

    for (int i = 0; i < thread_n; i++)
       thread_arr[i] = thread(thread_upd_pos, this, &_ball_arr,
                                   (ball_n * i) / thread_n,
                                   (ball_n * (i+1)) / thread_n,
                                    dt);
    for (int i = 0; i < thread_n; i++)
       thread_arr[i].join();
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

    this->update(0.0, BRUTE_FORCE, false);
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
