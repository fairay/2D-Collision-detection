#ifndef SCENE_H
#define SCENE_H

#include "ball/ball.h"

#include <QGraphicsScene>

#include <memory>
#include <random>
#include <vector>

class Scene
{
public:
    Scene(size_t n, double r);

    void show(std::shared_ptr<QGraphicsScene> &_qscene);

    ~Scene();
private:
    double _w = 600, _h = 600;

    size_t _ball_n = 100;
    double _ball_r = 1.0;
    std::vector<Ball> _ball_arr;

    void _init_state();
};

#endif // SCENE_H

