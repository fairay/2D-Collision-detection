#include "rect_tree.h"
#include <iostream>

#define SPLIT_N 10 // !!! 70 !!!

using namespace std;
/*

min_p
 x   \
  0  \  1
 ----x-----
  2  \  3
     \    x
          max_p
*/

class QuadTree: public RectTree
{
public:
    QuadTree(Point2d min_p, Point2d max_p);
    ~QuadTree();

    virtual void show(shared_ptr<QGraphicsScene> &_qscene);
private:
    Point2d _center;
    virtual void _init_leaves();
};

QuadTree::QuadTree(Point2d min_p, Point2d max_p): RectTree(min_p, max_p)
{
    _split_n = SPLIT_N;
    _leaf_n = 4;
    _center.x = (min_p.x + max_p.x) / 2;
    _center.y = (min_p.y + max_p.y) / 2;
}
QuadTree::~QuadTree()
{
    if (!_is_leaf)
        for (size_t i=0; i<_leaf_n; i++)
            delete _leaf_arr[i];
}

void QuadTree::show(shared_ptr<QGraphicsScene> &_qscene)
{
    if (_is_leaf) return;
    _qscene->addLine(_min_p.x, _center.y,
                     _max_p.x, _center.y,
                     QPen(Qt::blue));
    _qscene->addLine(_center.x, _min_p.y,
                     _center.x, _max_p.y,
                     QPen(Qt::blue));

    for (size_t i = 0; i<_leaf_n; i++)
        _leaf_arr[i]->show(_qscene);
}

/*
void QuadTree::add_ball(Ball* ball)
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
void QuadTree::add_ball_mult(Ball *ball)
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


void QuadTree::collide(collide_func f)
{
    if (_is_leaf)
    {
        if (_ball_arr.size() < 2) return;
        _collide_leaf(f);
    }
    else
    {
        for (size_t i=0; i<4; i++)
            _leaf_arr[i]->collide(f);
    }
}
void QuadTree::collide_mult(collide_func f, int deep)
{
    if (_is_leaf)
    {
        _collide_leaf(f);
    }
    else
    {
        vector<thread> thread_arr;
        thread_arr.reserve(3);
        for (size_t i=1; i<4; i++)
            thread_arr.push_back(thread(thread_collide_balls, _leaf_arr[i], f, deep-1));
        thread_collide_balls(_leaf_arr[0], f, deep-1);
        for (size_t i=0; i<3; i++)
            thread_arr[i].join();
    }
}


void QuadTree::_add_ball_leaves(Ball* ball, bool is_threading)
{
    for (size_t i=0; i<4; i++)
    {
        if (is_threading)
            _leaf_arr[i]->add_ball_mult(ball);
        else
            _leaf_arr[i]->add_ball(ball);
    }
}

*/

void QuadTree::_init_leaves()
{
    this->_is_leaf = false;

    _leaf_arr[0] = new QuadTree(_min_p, _center);

    Point2d p1, p2;
    p1 = _center, p2 = _center;
    p1.y = _min_p.y, p2.x = _max_p.x;
    _leaf_arr[1] = new QuadTree(p1, p2);

    p1 = _center, p2 = _center;
    p1.x = _min_p.x, p2.y = _max_p.y;
    _leaf_arr[2] = new QuadTree(p1, p2);

    _leaf_arr[3] = new QuadTree(_center, _max_p);
}

void Scene::_quad_tree(bool is_threading)
{
    _alg = shared_ptr<BaseTree>(new QuadTree(Point2d(0, 0), Point2d(_w, _h)));
    // QuadTree tree(Point2d(0, 0), Point2d(_w, _h));

    if (is_threading)
    {
        thread_add_balls(_alg.get(), _ball_arr);
        _alg->collide_mult(_collide_balls, 4);
    }
    else
    {
        for (size_t i=0; i<_ball_n; i++)
            _alg->add_ball(&_ball_arr[i]);

        _alg->collide(_collide_balls);
    }
}
