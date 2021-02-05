#ifndef RECT_TREE_H
#define RECT_TREE_H

#include "base_tree.h"
#define MAX_LEAF_N 16

class RectTree: public BaseTree
{
public:
    RectTree(Point2d min_p, Point2d max_p);
    virtual ~RectTree() = 0;

    void add_ball(Ball* ball);
    void add_ball_mult(Ball* ball);

    void collide(collide_func f);
    void collide_mult(collide_func f, int deep);

    int deep();
    virtual void split_space();
    virtual bool is_ball_in(Ball *ball);
protected:
    Point2d _min_p, _max_p;
    RectTree* _leaf_arr[MAX_LEAF_N];

    size_t _leaf_n = 16;

    void _add_ball_leaves(Ball* ball, bool is_threading=false);
};

#endif // RECT_TREE_H
