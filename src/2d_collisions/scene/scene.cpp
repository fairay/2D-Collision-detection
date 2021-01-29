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
    for (auto ball : _ball_arr)
    {
        _qscene->addEllipse(ball.pos.x - _ball_r, ball.pos.y - _ball_r,
                            _ball_r, _ball_r);
    }
}

void Scene::_init_state()
{
    for (size_t i = 0; i < _ball_n; i++)
    {
        double x = _ball_r + (_w - _ball_r*2) * ((double)rand() / RAND_MAX);
        double y = _ball_r + (_h - _ball_r*2) * ((double)rand() / RAND_MAX);
        _ball_arr.push_back(Ball(x, y, _ball_r));
    }
}


