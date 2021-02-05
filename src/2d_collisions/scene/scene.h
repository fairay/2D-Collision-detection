#ifndef SCENE_H
#define SCENE_H

#include "ball/ball.h"

#include <QGraphicsScene>

#include <memory>
#include <random>
#include <vector>

#define MAX_SPEED 100.0

typedef enum {
    BRUTE_FORCE,
    QUAD_TREE,
    DYNAMIC_QUAD_TREE,
    HEXA_TREE,
    BIN_TREE,
    NO_CHECK
} upd_t;

using collide_func = void (*)(Ball& ball1, Ball& ball2);

class Scene
{
public:
    Scene(size_t n, double r);

    void show(std::shared_ptr<QGraphicsScene> &_qscene);
    void update(double dt, upd_t update_type, bool is_threading);

    ~Scene();
private:
    double _w = 600, _h = 600;

    size_t _ball_n = 100;
    double _ball_r = 1.0;
    std::vector<Ball> _ball_arr;

    void _init_state();
    void _collide_border(Ball& ball, double x, double y);
    static void _collide_balls(Ball& ball1, Ball& ball2);

    void _brute_force(bool is_threading);
    void _quad_tree(bool is_threading);
    void _dynamic_quad_tree(bool is_threading);
    void _hexa_tree(bool is_threading);
    void _bin_tree(bool is_threading);

};

#endif // SCENE_H

