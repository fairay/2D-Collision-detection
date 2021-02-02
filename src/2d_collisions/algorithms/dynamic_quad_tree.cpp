#include "base_tree.h"
#define SPLIT_N 100

using namespace std;


class DynamicQuadTree: public BaseTree
{
public:
    DynamicQuadTree(Point2d min_p, Point2d max_p);
    ~DynamicQuadTree();

    void count_center();
    void add_ball(Ball* ball);
    void split_space();
    void collide(collide_func f);
private:
    Point2d _min_p, _max_p;
    Point2d _center;

    DynamicQuadTree* _leaf_arr[4];

    void _init_leaves();
    void _add_ball_leaves(Ball* ball);
};

DynamicQuadTree::DynamicQuadTree(Point2d min_p, Point2d max_p):
    _min_p(min_p), _max_p(max_p)
{
    _split_n = SPLIT_N;
}

DynamicQuadTree::~DynamicQuadTree()
{
    if (!_is_leaf)
        for (size_t i=0; i<4; i++)
            delete _leaf_arr[i];
}

void DynamicQuadTree::count_center()
{
    double sum_x = 0, sum_y = 0;
    size_t n = _ball_arr.size();

    for (size_t i=0; i<n; i++)
    {
        sum_x += _ball_arr[i]->pos.x;
        sum_y += _ball_arr[i]->pos.y;
    }
    sum_x /= n;
    sum_y /= n;

    _center.x = sum_x;
    _center.y = sum_y;
}

void DynamicQuadTree::add_ball(Ball* ball)
{
    if (_min_p.x - ball->r < ball->pos.x && ball->pos.x < _max_p.x + ball->r &&
        _min_p.y - ball->r < ball->pos.y && ball->pos.y < _max_p.y + ball->r)
            _ball_arr.push_back(ball);
}

void DynamicQuadTree::split_space()
{
    if (_ball_arr.size() >= _split_n)
    {
        count_center();

        _init_leaves();
        for (size_t i=0; i<_ball_arr.size(); i++)
            _add_ball_leaves(_ball_arr[i]);
        _ball_arr.clear();

        for (size_t i=0; i<4; i++)
            _leaf_arr[i]->split_space();
    }
}

void DynamicQuadTree::_init_leaves()
{
    this->_is_leaf = false;

    _leaf_arr[0] = new DynamicQuadTree(_min_p, _center);

    Point2d p1, p2;
    p1 = _center, p2 = _center;
    p1.y = _min_p.y, p2.x = _max_p.x;
    _leaf_arr[1] = new DynamicQuadTree(p1, p2);

    p1 = _center, p2 = _center;
    p1.x = _min_p.x, p2.y = _max_p.y;
    _leaf_arr[2] = new DynamicQuadTree(p1, p2);

    _leaf_arr[3] = new DynamicQuadTree(_center, _max_p);
}

void DynamicQuadTree::_add_ball_leaves(Ball* ball)
{
    for (size_t i=0; i<4; i++)
        _leaf_arr[i]->add_ball(ball);
}

void DynamicQuadTree::collide(collide_func f)
{
    if (_is_leaf)
    {
        if (_ball_arr.size() < 2) return;

        for (size_t i=0; i<_ball_arr.size(); i++)
            for (size_t j=i+1; j<_ball_arr.size(); j++)
            {
                double dist = sqrt(pow(_ball_arr[i]->pos.x - _ball_arr[j]->pos.x, 2) +
                                   pow(_ball_arr[i]->pos.y - _ball_arr[j]->pos.y, 2));
                if (dist <= _ball_arr[i]->r + _ball_arr[j]->r)
                    f(*_ball_arr[i], *_ball_arr[j]);
            }
    }
    else
    {
        for (size_t i=0; i<4; i++)
            _leaf_arr[i]->collide(f);
    }
}

void Scene::_dynamic_quad_tree()
{
    DynamicQuadTree tree(Point2d(0, 0), Point2d(_w, _h));
    for (size_t i=0; i<_ball_n; i++)
        tree.add_ball(&_ball_arr[i]);

    tree.split_space();

    tree.collide(_collide_balls);
}
