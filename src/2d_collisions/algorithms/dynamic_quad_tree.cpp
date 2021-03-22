#include "rect_tree.h"
#define SPLIT_N 100 // !!! 70 !!!

using namespace std;


class DynamicQuadTree: public RectTree
{
public:
    DynamicQuadTree(Point2d min_p, Point2d max_p);
    ~DynamicQuadTree();

    void add_ball(Ball* ball);
    virtual void show(shared_ptr<QGraphicsScene> &_qscene);

    void count_center();
    void split_space();
private:
    Point2d _center;

    void _init_leaves();
};

DynamicQuadTree::DynamicQuadTree(Point2d min_p, Point2d max_p):
    RectTree(min_p, max_p)
{
    _split_n = SPLIT_N;
    _leaf_n = 4;
}
DynamicQuadTree::~DynamicQuadTree()
{
    if (!_is_leaf)
        for (size_t i=0; i<_leaf_n; i++)
            delete _leaf_arr[i];
}

void DynamicQuadTree::show(shared_ptr<QGraphicsScene> &_qscene)
{
    if (_is_leaf) return;
    _qscene->addLine(_min_p.x, _center.y,
                     _max_p.x, _center.y,
                     QPen(MASH_COLOR));
    _qscene->addLine(_center.x, _min_p.y,
                     _center.x, _max_p.y,
                     QPen(MASH_COLOR));

    for (size_t i = 0; i<_leaf_n; i++)
        _leaf_arr[i]->show(_qscene);
}


void DynamicQuadTree::count_center()
{
    double sum_x = 0, sum_y = 0;

    for (size_t i=0; i<_ball_n; i++)
    {
        sum_x += _ball_arr[i]->pos.x;
        sum_y += _ball_arr[i]->pos.y;
    }
    sum_x /= _ball_n;
    sum_y /= _ball_n;

    _center.x = sum_x;
    _center.y = sum_y;
}

//void DynamicQuadTree::add_ball(Ball* ball)
//{
//    if (is_ball_in(ball))
//        _ball_arr.push_back(ball);
//}


void DynamicQuadTree::add_ball(Ball* ball)
{
    if (!is_ball_in(ball)) return;

    if (_is_leaf)
    {
        _ball_arr[_ball_n++] = ball;
        split_space();
//        if (_ball_arr.size() >= _split_n)
//        {
//            _init_leaves();
//            for (size_t i=0; i<_ball_arr.size(); i++)
//                _add_ball_leaves(_ball_arr[i]);
//            _ball_arr.clear();
//        }
    }
    else
        _add_ball_leaves(ball);
}

void DynamicQuadTree::split_space()
{
    if (_ball_n < _split_n) return;

    count_center();

    _init_leaves();
    for (size_t i=0; i<_ball_n; i++)
        _add_ball_leaves(_ball_arr[i]);
    _ball_n = 0;

    //for (size_t i=0; i<4; i++)
    //    _leaf_arr[i]->split_space();
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


void Scene::_dynamic_quad_tree(bool is_threading)
{
    DynamicQuadTree* tree = new DynamicQuadTree(Point2d(0, 0), Point2d(_w, _h));
    _alg = shared_ptr<BaseTree>(tree);
    // DynamicQuadTree tree(Point2d(0, 0), Point2d(_w, _h));

    for (size_t i=0; i<_ball_n; i++)
        tree->add_ball(&_ball_arr[i]);
    //tree->split_space();

    if (is_threading)
    {
        int thread_n = 16;
        vector<BaseTree*> v;
        v.assign(thread_n, nullptr);
        _alg->select_nodes(v, thread_n);

        #pragma omp parallel for num_threads(16)
        for (int i=0; i<thread_n; i++)
            if (v[i])
                v[i]->collide(_collide_balls);
    }
    else
        tree->collide(_collide_balls);
}
