#ifndef BASE_TREE_H
#define BASE_TREE_H

#include "scene/scene.h"
#include <thread>
#include <mutex>

class ThreadManager;

class BaseTree
{
public:
    BaseTree();
    virtual ~BaseTree() = 0;

    virtual void add_ball(Ball* ball) = 0;
    virtual void collide(collide_func f) = 0;
    virtual void add_ball_mult(Ball* ball);
    virtual void collide_mult(collide_func f, size_t deep);

    bool is_void();
    virtual int deep();
protected:
    size_t _split_n = 70;
    bool _is_leaf = true;
    std::vector<Ball*> _ball_arr;

    void _collide_leaf(collide_func f);
};

void thread_add_balls(BaseTree* tree, std::vector<Ball> &ball_arr);

#endif // BASE_TREE_H
