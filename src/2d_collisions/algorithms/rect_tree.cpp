#include "rect_tree.h"
#include <iostream>

using namespace std;

RectTree::RectTree(Point2d min_p, Point2d max_p):
    _min_p(min_p), _max_p(max_p) {}
RectTree::~RectTree() {}

void RectTree::add_ball(Ball* ball)
{
    if (is_ball_in(ball))
    {
        if (_is_leaf)
        {
            _ball_arr.push_back(ball);
            if (_ball_arr.size() >= _split_n)
            {
                _init_leaves();
                for (size_t i=0; i<_ball_arr.size(); i++)
                    _add_ball_leaves(_ball_arr[i]);
                _ball_arr.clear();
            }
        }
        else
            _add_ball_leaves(ball);
    }
}
void RectTree::add_ball_mult(Ball *ball)
{
    if (is_ball_in(ball))
    {
        _m.lock();
        if (_is_leaf)
        {
            _ball_arr.push_back(ball);
            if (_ball_arr.size() >= _split_n)
            {
                _init_leaves();
                for (size_t i=0; i<_ball_arr.size(); i++)
                    _add_ball_leaves(_ball_arr[i], false);
                _ball_arr.clear();
            }
            _m.unlock();
        }
        else
        {
            _m.unlock();
            _add_ball_leaves(ball, true);
        }
    }
}


void RectTree::collide(collide_func f)
{
    if (_is_leaf)
    {
        if (_ball_arr.size() < 2) return;
        _collide_leaf(f);
    }
    else
    {
        for (size_t i=0; i<_leaf_n; i++)
            _leaf_arr[i]->collide(f);
    }
}
void RectTree::collide_mult(collide_func f, int deep)
{
    if (_is_leaf)
    {
        _collide_leaf(f);
    }
    else
    {
        vector<thread> thread_arr;
        thread_arr.reserve(_leaf_n-1);
        for (size_t i=1; i<_leaf_n; i++)
            thread_arr.push_back(thread(thread_collide_balls, _leaf_arr[i], f, deep-1));
        thread_collide_balls(_leaf_arr[0], f, deep-1);
        for (size_t i=0; i<_leaf_n-1; i++)
            thread_arr[i].join();
    }
}

void RectTree::_add_ball_leaves(Ball* ball, bool is_threading)
{
    for (size_t i=0; i<_leaf_n; i++)
    {
        if (is_threading)
            _leaf_arr[i]->add_ball_mult(ball);
        else
            _leaf_arr[i]->add_ball(ball);
    }
}

int RectTree::deep()
{
    if (_is_leaf)
        return 1;
    else
    {
        int deep = -1;
        for (size_t i=0; i<_leaf_n; i++)
            deep = max(deep, _leaf_arr[i]->deep());
        return deep + 1;
    }
}

void RectTree::split_space() {}

bool RectTree::is_ball_in(Ball *ball)
{
    return _min_p.x - ball->r < ball->pos.x && ball->pos.x < _max_p.x + ball->r &&
            _min_p.y - ball->r < ball->pos.y && ball->pos.y < _max_p.y + ball->r;
}
