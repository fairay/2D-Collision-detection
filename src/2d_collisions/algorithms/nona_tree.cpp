#include "rect_tree.h"
#include <iostream>
#define SPLIT_N 10 // !!! 100 !!!

using namespace std;


class NonaTree: public RectTree
{
public:
    NonaTree(Point2d min_p, Point2d max_p);
    ~NonaTree();

    virtual void show(shared_ptr<QGraphicsScene> &_qscene);
private:
    Point2d _cnt0, _cnt1, _cnt2, _cnt3;

    void _init_leaves();
};


NonaTree::NonaTree(Point2d min_p, Point2d max_p): RectTree(min_p, max_p)
{
    _split_n = SPLIT_N;
    _leaf_n = 9;

    _cnt0.x = max_p.x / 3 + 2.0/3 * min_p.x;
    _cnt0.y = max_p.y / 3 + 2.0/3 * min_p.y;

    _cnt3.x = 2.0/3 * max_p.x + min_p.x / 3;
    _cnt3.y = 2.0/3 * max_p.y + min_p.y / 3;

    _cnt1.x = 2.0/3 * max_p.x + min_p.x / 3;
    _cnt1.y = max_p.y / 3 + 2.0/3 * min_p.y;

    _cnt2.x = max_p.x / 3 + 2.0/3 * min_p.x;
    _cnt2.y = 2.0/3 * max_p.y + min_p.y / 3;
}
NonaTree::~NonaTree()
{
    if (!_is_leaf)
        for (size_t i=0; i<9; i++)
            delete _leaf_arr[i];
}

void NonaTree::show(shared_ptr<QGraphicsScene> &_qscene)
{
    if (_is_leaf) return;
    _qscene->addLine(_min_p.x, _cnt0.y,
                     _max_p.x, _cnt0.y,
                     QPen(MASH_COLOR));
    _qscene->addLine(_min_p.x, _cnt2.y,
                     _max_p.x, _cnt2.y,
                     QPen(MASH_COLOR));

    _qscene->addLine(_cnt0.x, _min_p.y,
                     _cnt0.x, _max_p.y,
                     QPen(MASH_COLOR));
    _qscene->addLine(_cnt1.x, _min_p.y,
                     _cnt1.x, _max_p.y,
                     QPen(MASH_COLOR));

    for (size_t i = 0; i<_leaf_n; i++)
        _leaf_arr[i]->show(_qscene);
}


void NonaTree::_init_leaves()
{
    this->_is_leaf = false;

    _leaf_arr[0] = new NonaTree(_min_p, _cnt0);

    Point2d p1, p2;

    p1 = _cnt0, p2 = _cnt1;
    p1.y = _min_p.y;
    _leaf_arr[1] = new NonaTree(p1, p2);

    p1 = _cnt1, p2 = _cnt1;
    p1.y = _min_p.y, p2.x = _max_p.x;
    _leaf_arr[2] = new NonaTree(p1, p2);


    p1 = _cnt0, p2 = _cnt2;
    p1.x = _min_p.x;
    _leaf_arr[3] = new NonaTree(p1, p2);

    _leaf_arr[4] = new NonaTree(_cnt0, _cnt3);


    p1 = _cnt1, p2 = _cnt3;
    p2.x = _max_p.x;
    _leaf_arr[5] = new NonaTree(p1, p2);

    p1 = _cnt2, p2 = _cnt2;
    p1.x = _min_p.x, p2.y = _max_p.y;
    _leaf_arr[6] = new NonaTree(p1, p2);

    p1 = _cnt2, p2 = _cnt3;
    p2.y = _max_p.y;
    _leaf_arr[7] = new NonaTree(p1, p2);

    _leaf_arr[8] = new NonaTree(_cnt3, _max_p);
}

void Scene::_nona_tree(bool is_threading)
{
    // HexaTree tree(Point2d(0, 0), Point2d(_w, _h));
    _alg = shared_ptr<BaseTree>(new NonaTree(Point2d(0, 0), Point2d(_w, _h)));
    if (is_threading)
    {
        for (size_t i=0; i<_ball_n; i++)
            _alg->add_ball(&_ball_arr[i]);

        int thread_n = 9;
        vector<BaseTree*> v;
        v.assign(thread_n, nullptr);
        _alg->select_nodes(v, thread_n);

        #pragma omp parallel for num_threads(9)
        for (int i=0; i<thread_n; i++)
            if (v[i])
                v[i]->collide(_collide_balls);
    }
    else
    {
        for (size_t i=0; i<_ball_n; i++)
            _alg->add_ball(&_ball_arr[i]);

        _alg->collide(_collide_balls);
    }
}