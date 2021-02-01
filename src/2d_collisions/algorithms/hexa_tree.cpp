#include "scene/scene.h"
#include <iostream>

using namespace std;


class HexaTree
{
public:
    HexaTree(Point2d min_p, Point2d max_p);
    ~HexaTree();

    void add_ball(Ball* ball);
    void collide(collide_func f);
private:
    size_t _split_n = 20;
    Point2d _min_p, _max_p;
    Point2d _cnt0, _cnt1, _cnt2, _cnt3;

    bool _is_leaf = true;
    vector<Ball*> _ball_arr;
    HexaTree* _leaf_arr[9];

    void _add_ball_leaves(Ball* ball);
    void _init_leafs();
};


HexaTree::HexaTree(Point2d min_p, Point2d max_p): _min_p(min_p), _max_p(max_p)
{
    _cnt0.x = max_p.x / 3 + 2.0/3 * min_p.x;
    _cnt0.y = max_p.y / 3 + 2.0/3 * min_p.y;

    _cnt3.x = 2.0/3 * max_p.x + min_p.x / 3;
    _cnt3.y = 2.0/3 * max_p.y + min_p.y / 3;

    _cnt1.x = 2.0/3 * max_p.x + min_p.x / 3;
    _cnt1.y = max_p.y / 3 + 2.0/3 * min_p.y;

    _cnt2.x = max_p.x / 3 + 2.0/3 * min_p.x;
    _cnt2.y = 2.0/3 * max_p.y + min_p.y / 3;
}
HexaTree::~HexaTree()
{
    if (!_is_leaf)
        for (size_t i=0; i<9; i++)
            delete _leaf_arr[i];
}

void HexaTree::add_ball(Ball* ball)
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
                    _add_ball_leaves(_ball_arr[i]);
                _ball_arr.clear();
            }
        }
        else
            _add_ball_leaves(ball);
    }
}

void HexaTree::collide(collide_func f)
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
        for (size_t i=0; i<9; i++)
            _leaf_arr[i]->collide(f);
    }
}

void HexaTree::_add_ball_leaves(Ball* ball)
{
    for (size_t i=0; i<9; i++)
        _leaf_arr[i]->add_ball(ball);
}


void HexaTree::_init_leafs()
{
    this->_is_leaf = false;

    _leaf_arr[0] = new HexaTree(_min_p, _cnt0);

    Point2d p1, p2;

    p1 = _cnt0, p2 = _cnt1;
    p1.y = _min_p.y;
    _leaf_arr[1] = new HexaTree(p1, p2);

    p1 = _cnt1, p2 = _cnt1;
    p1.y = _min_p.y, p2.x = _max_p.x;
    _leaf_arr[2] = new HexaTree(p1, p2);


    p1 = _cnt0, p2 = _cnt2;
    p1.x = _min_p.x;
    _leaf_arr[3] = new HexaTree(p1, p2);

    _leaf_arr[4] = new HexaTree(_cnt0, _cnt3);


    p1 = _cnt1, p2 = _cnt3;
    p2.x = _max_p.x;
    _leaf_arr[5] = new HexaTree(p1, p2);

    p1 = _cnt2, p2 = _cnt2;
    p1.x = _min_p.x, p2.y = _max_p.y;
    _leaf_arr[6] = new HexaTree(p1, p2);

    p1 = _cnt2, p2 = _cnt3;
    p2.y = _max_p.y;
    _leaf_arr[7] = new HexaTree(p1, p2);

    _leaf_arr[8] = new HexaTree(_cnt3, _max_p);
}

void Scene::_hexa_tree()
{
    HexaTree tree(Point2d(0, 0), Point2d(_w, _h));
    for (size_t i=0; i<_ball_n; i++)
        tree.add_ball(&_ball_arr[i]);

    tree.collide(_collide_balls);
}
