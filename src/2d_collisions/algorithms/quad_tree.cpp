#include "base_tree.h"

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

class QuadTree: public BaseTree
{
public:
    QuadTree(Point2d min_p, Point2d max_p);
    ~QuadTree();

    void add_ball(Ball* ball);
    void collide(collide_func f);
private:
    size_t _split_n = 100;
    Point2d _min_p, _max_p;
    Point2d _center;

    bool _is_leaf = true;
    vector<Ball*> _ball_arr;
    QuadTree* _leaf_arr[4];

    void _add_ball_leafs(Ball* ball);
    void _init_leafs();
};


QuadTree::QuadTree(Point2d min_p, Point2d max_p): _min_p(min_p), _max_p(max_p)
{
    _center.x = (min_p.x + max_p.x) / 2;
    _center.y = (min_p.y + max_p.y) / 2;
}
QuadTree::~QuadTree()
{
    if (!_is_leaf)
        for (size_t i=0; i<4; i++)
            delete _leaf_arr[i];
}

void QuadTree::add_ball(Ball* ball)
{
    if (_min_p.x - ball->r < ball->pos.x && ball->pos.x < _max_p.x + ball->r &&
        _min_p.y - ball->r < ball->pos.y && ball->pos.y < _max_p.y + ball->r)
    {
        if (_is_leaf)
        {
            _ball_arr.push_back(ball);
            if (_ball_arr.size() >= _split_n)
            {
                _init_leafs();
                for (size_t i=0; i<_ball_arr.size(); i++)
                    _add_ball_leafs(_ball_arr[i]);
                _ball_arr.clear();
            }
        }
        else
            _add_ball_leafs(ball);
    }
}

void QuadTree::collide(collide_func f)
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

void QuadTree::_add_ball_leafs(Ball* ball)
{
    for (size_t i=0; i<4; i++)
        _leaf_arr[i]->add_ball(ball);
}

void QuadTree::_init_leafs()
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


void Scene::_quad_tree()
{
    QuadTree tree(Point2d(0, 0), Point2d(_w, _h));
    for (size_t i=0; i<_ball_n; i++)
        tree.add_ball(&_ball_arr[i]);

    tree.collide(_collide_balls);
}
