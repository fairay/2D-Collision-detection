#ifndef BASE_TREE_H
#define BASE_TREE_H

#include "scene/scene.h"

class ThreadManager;

class BaseTree
{
public:
    BaseTree();
    virtual ~BaseTree() = 0;

    virtual void add_ball(Ball* ball) = 0;
    virtual void collide(collide_func f) = 0;
    virtual void collide_mult(collide_func f, size_t deep);

    bool is_void();
    virtual int deep();
protected:
    size_t _split_n = 70;
    bool _is_leaf = true;
    std::vector<Ball*> _ball_arr;

    void _collide_leaf(collide_func f);
};


#endif // BASE_TREE_H
