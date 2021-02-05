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
    virtual void collide_mult(collide_func f, int deep);

    bool is_void();
    virtual int deep();
protected:
    size_t _split_n = 70;
    bool _is_leaf = true;
    std::vector<Ball*> _ball_arr;
    std::mutex _m;

    void _collide_leaf(collide_func f);

    virtual void _init_leaves() = 0;
    virtual void _add_ball_leaves(Ball* ball, bool is_threading=false);
};

void thread_add_balls(BaseTree* tree, std::vector<Ball> &ball_arr);
void thread_collide_balls(BaseTree* tree, collide_func f, int deep);

#endif // BASE_TREE_H
