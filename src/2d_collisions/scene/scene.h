#ifndef SCENE_H
#define SCENE_H

#include "ball/ball.h"

#include <QGraphicsScene>

#include <memory>
#include <random>
#include <vector>

#define MAX_SPEED 100.0

typedef enum {
    NO_CHECK,
    BRUTE_FORCE,
    QUAD_TREE,
    DYNAMINC_QUAD_TREE,
    HEXA_TREE,
    BIN_TREE
} upd_t;

using collide_func = void (*)(Ball& ball1, Ball& ball2);

class Scene
{
public:
    Scene(size_t n, double r);

    void show(std::shared_ptr<QGraphicsScene> &_qscene);
    void update(double dt, upd_t update_type = NO_CHECK);

    ~Scene();
private:
    double _w = 600, _h = 600;

    size_t _ball_n = 100;
    double _ball_r = 1.0;
    std::vector<Ball> _ball_arr;

    void _init_state();
    void _collide_border(Ball& ball, double x, double y);
    static void _collide_balls(Ball& ball1, Ball& ball2);

    void _brute_force();
    void _quad_tree();
    // Katya

    // Seva

};

#endif // SCENE_H

