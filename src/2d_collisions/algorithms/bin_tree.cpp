#include "scene/scene.h"
#include <iostream>

using namespace std;
class BinTree
{
public:
    BinTree() {}
    BinTree(Point2d a, Point2d b, Point2d c);
    ~BinTree();

    void add_ball(Ball* ball);
    void collide(collide_func f);
protected:
    size_t _split_n = 10;
    Point2d _p_arr[3];

    Vector2d _dir;
    Vector2d _right_off, _left_off;

    bool _is_leaf = true;
    vector<Ball*> _ball_arr;
    BinTree* _left_leaf;
    BinTree* _right_leaf;

    virtual void _init_leavs();
    void _add_ball_leavs(Ball* ball);
};

BinTree::BinTree(Point2d a, Point2d b, Point2d c)
{
    double ad = dist(b, c);
    double bd = dist(a, c);
    double cd = dist(a, b);

    if (ad >= bd && ad >= cd)
        _p_arr[0] = a, _p_arr[1] = b, _p_arr[2] = c;
    else if (bd >= ad && bd >= cd)
        _p_arr[0] = b, _p_arr[1] = a, _p_arr[2] = c;
    else
        _p_arr[0] = c, _p_arr[1] = a, _p_arr[2] = b;
}
BinTree::~BinTree()
{
    if (!_is_leaf)
    {
        delete _left_leaf;
        delete  _right_leaf;
    }
}

void BinTree::add_ball(Ball* ball)
{
    if (_is_leaf)
    {
        _ball_arr.push_back(ball);
        if (_ball_arr.size() >= _split_n)
        {
            _init_leavs();
            for (size_t i=0; i<_split_n; i++)
                _add_ball_leavs(_ball_arr[i]);
            _ball_arr.clear();
        }
    }
    else
    {
        _add_ball_leavs(ball);
    }
}

void BinTree::collide(collide_func f)
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
        _left_leaf->collide(f);
        _right_leaf->collide(f);
    }
}



void BinTree::_init_leavs()
{
    _is_leaf = false;

    Point2d mid = mid_point(_p_arr[1], _p_arr[2]);
    _dir = Vector2d(_p_arr[0], mid);
    normalize(_dir);
    _right_off = Vector2d(-_dir.y, _dir.x);
    _left_off =  Vector2d(_dir.y, -_dir.x);

    if (vector_mult(_dir, Vector2d(_p_arr[0], _p_arr[1])) > 0)
    {
        _right_leaf = new BinTree(_p_arr[0], _p_arr[1], mid);
        _left_leaf =  new BinTree(_p_arr[0], _p_arr[2], mid);
    }
    else
    {
        _right_leaf = new BinTree(_p_arr[0], _p_arr[2], mid);
        _left_leaf =  new BinTree(_p_arr[0], _p_arr[1], mid);
    }
}

void BinTree::_add_ball_leavs(Ball* ball)
{
    Point2d p = ball->pos;
    p.x += _right_off.x, p.y += _right_off.y;
    if (vector_mult(_dir, Vector2d(_p_arr[0], p)) >= 0)
        _right_leaf->add_ball(ball);

    p = ball->pos;
    p.x += _left_off.x, p.y += _left_off.y;
    if (vector_mult(_dir, Vector2d(_p_arr[0], p)) <= 0)
        _left_leaf->add_ball(ball);
}


class MainBinTree: public BinTree
{
public:
    MainBinTree(const Point2d& min_p, const Point2d& max_p);
    ~MainBinTree();
private:
    Point2d _p_arr[4];

    virtual void _init_leavs();
};

MainBinTree::MainBinTree(const Point2d& min_p, const Point2d& max_p)
{
    _p_arr[0] = min_p;
    _p_arr[1] = Point2d(max_p.x, min_p.y);
    _p_arr[2] = Point2d(min_p.x, max_p.y);
    _p_arr[3] = max_p;
}
MainBinTree::~MainBinTree() {}

void MainBinTree::_init_leavs()
{
    _is_leaf = false;

    _dir = Vector2d(_p_arr[0], _p_arr[3]);
    normalize(_dir);
    _right_off = Vector2d(-_dir.y, _dir.x);
    _left_off =  Vector2d(_dir.y, -_dir.x);

    _right_leaf = new BinTree(_p_arr[0], _p_arr[2], _p_arr[3]);
    _left_leaf =  new BinTree(_p_arr[0], _p_arr[1], _p_arr[3]);
}

void Scene::_bin_tree()
{
    MainBinTree tree(Point2d(0,0), Point2d(_w, _h));
    for (size_t i=0; i<_ball_n; i++)
        tree.add_ball(&_ball_arr[i]);

    tree.collide(_collide_balls);
}
