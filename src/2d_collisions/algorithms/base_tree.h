#ifndef BASE_TREE_H
#define BASE_TREE_H

#include "scene/scene.h"
#include "config.h"
#include <thread>
#include <mutex>
#include <vector>

#define THREAD_N    8
#define MAX_BALL_N  100
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

    virtual void show(std::shared_ptr<QGraphicsScene> &_qscene);

    virtual void select_nodes(std::vector<BaseTree*> &v, int to, int from=0);

    bool is_void();
    virtual int deep();
    bool is_leaf();
protected:
    size_t _split_n = 70;
    bool _is_leaf = true;
    //std::vector<Ball*> _ball_arr;
    Ball* _ball_arr[MAX_BALL_N+1];
    size_t _ball_n = 0;
    std::mutex _m;

    void _collide_leaf(collide_func f);

    virtual void _init_leaves();
    virtual void _add_ball_leaves(Ball* ball, bool is_threading=false);
};

void thread_add_balls(BaseTree* tree, std::vector<Ball> &ball_arr);
void thread_collide_balls(BaseTree* tree, collide_func f, int deep);

void collide_balls(Ball* b1, Ball* b2, collide_func f);

#endif // BASE_TREE_H
