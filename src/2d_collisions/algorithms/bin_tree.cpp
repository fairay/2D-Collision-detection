#include "base_tree.h"
#include <iostream>
#include <future>

// #include "omp.h"

#define SPLIT_N 70

using namespace std;

class BinTree: public BaseTree
{
public:
    BinTree() {_split_n = SPLIT_N;}
    BinTree(Point2d a, Point2d b, Point2d c);
    ~BinTree();

    void add_ball(Ball* ball);
    void collide(collide_func f);
    void add_ball_mult(Ball* ball);
    void collide_mult(collide_func f, int deep);

    void select_nodes(std::vector<BaseTree*>& v, int to, int from=0);

    int deep();
protected:
    Point2d _p_arr[3];

    Vector2d _dir;
    Vector2d _right_off, _left_off;

    BinTree* _left_leaf;
    BinTree* _right_leaf;

    virtual void _init_leaves();
    void _add_ball_leavs(Ball* ball, bool is_threading = false);
    void _tcb(collide_func f, int deep);
};

BinTree::BinTree(Point2d a, Point2d b, Point2d c)
{
    _split_n = SPLIT_N;
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
            _init_leaves();
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
void BinTree::add_ball_mult(Ball *ball)
{
    _m.lock();
    if (_is_leaf)
    {
        _ball_arr.push_back(ball);
        if (_ball_arr.size() >= _split_n)
        {
            _init_leaves();
            for (size_t i=0; i<_split_n; i++)
                _add_ball_leavs(_ball_arr[i], false);
            _ball_arr.clear();
        }
        _m.unlock();
    }
    else
    {
        _m.unlock();
        _add_ball_leavs(ball, true);
    }
}


void BinTree::collide(collide_func f)
{
    if (_is_leaf)
    {
        if (_ball_arr.size() < 2) return;
        _collide_leaf(f);
    }
    else
    {
        _left_leaf->collide(f);
        _right_leaf->collide(f);
    }
}

//void thread_act(BaseTree* tree, collide_func f, size_t deep)
//{
//    if (tree->is_void()) return;
//    if (deep >= 0)
//        tree->collide_mult(f, deep);
//    else
//        tree->collide(f);
//}
void BinTree::collide_mult(collide_func f, int deep)
{
    if (_is_leaf)
    {
        _collide_leaf(f);
    }
    else
    {
        thread t1(thread_collide_balls, _left_leaf,  f, deep-1);
        thread_collide_balls(_right_leaf, f, deep-1);
        t1.join();

//        thread t1(&BinTree::_tcb, _left_leaf,  f, deep-1);
//        _right_leaf->_tcb(f, deep-1);
//        t1.join();

//        auto as = async(launch::async, thread_collide_balls,
//                        _left_leaf,  f, deep-1);
//        thread_collide_balls(_right_leaf, f, deep-1);
//        as.get();

//         thread_collide_balls(_left_leaf, f, deep-1);
//         thread_collide_balls(_right_leaf, f, deep-1);
    }
}

void BinTree::select_nodes(std::vector<BaseTree*>& v, int to, int from)
{
    if (to - from == 1)
    {
        v[from] = this;
    }
    else
    {
        int border = (to + from) / 2;
        this->_left_leaf->select_nodes(v, border, from);
        this->_right_leaf->select_nodes(v, to, border);
    }
}

int BinTree::deep()
{
    if (_is_leaf)
        return 1;
    else
        return max(_left_leaf->deep(), _right_leaf->deep()) + 1;
}

void BinTree::_init_leaves()
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

void BinTree::_add_ball_leavs(Ball* ball, bool is_threading)
{
    Point2d p = ball->pos;
    p.x += _right_off.x, p.y += _right_off.y;
    if (vector_mult(_dir, Vector2d(_p_arr[0], p)) >= 0)
    {
        if (is_threading)
            _right_leaf->add_ball_mult(ball);
        else
            _right_leaf->add_ball(ball);
    }

    p = ball->pos;
    p.x += _left_off.x, p.y += _left_off.y;
    if (vector_mult(_dir, Vector2d(_p_arr[0], p)) <= 0)
    {
        if (is_threading)
            _left_leaf->add_ball_mult(ball);
        else
            _left_leaf->add_ball(ball);
    }
}

void BinTree::_tcb(collide_func f, int deep)
{
    if (this->is_void()) return;
    if (deep >= 0)
        this->collide_mult(f, deep);
    else
        this->collide(f);
}


class MainBinTree: public BinTree
{
public:
    MainBinTree(const Point2d& min_p, const Point2d& max_p);
    ~MainBinTree();
private:
    Point2d _p_arr[4];

    virtual void _init_leaves();
};

MainBinTree::MainBinTree(const Point2d& min_p, const Point2d& max_p)
{
    _p_arr[0] = min_p;
    _p_arr[1] = Point2d(max_p.x, min_p.y);
    _p_arr[2] = Point2d(min_p.x, max_p.y);
    _p_arr[3] = max_p;
}
MainBinTree::~MainBinTree() {}

void MainBinTree::_init_leaves()
{
    _is_leaf = false;

    _dir = Vector2d(_p_arr[0], _p_arr[3]);
    normalize(_dir);
    _right_off = Vector2d(-_dir.y, _dir.x);
    _left_off =  Vector2d(_dir.y, -_dir.x);

    _right_leaf = new BinTree(_p_arr[0], _p_arr[2], _p_arr[3]);
    _left_leaf =  new BinTree(_p_arr[0], _p_arr[1], _p_arr[3]);
}

void roflan_f(BaseTree* tree, collide_func f)
{
    tree->collide(f);
}

void Scene::_bin_tree(bool is_threading)
{
    MainBinTree tree(Point2d(0,0), Point2d(_w, _h));

    if (is_threading)
    {
        //thread_add_balls(&tree, _ball_arr);
        for (size_t i=0; i<_ball_n; i++)
            tree.add_ball(&_ball_arr[i]);

        // tree.collide_mult(_collide_balls, 2);

        int thread_n = 8;
        vector<BaseTree*> v;
        v.reserve(thread_n);
        tree.select_nodes(v, thread_n);

        vector<thread> thread_arr;
        thread_arr.reserve(thread_n);

        #pragma omp parallel for num_threads(8)
        for (int i=0; i<thread_n; i++)
        {
            //thread_arr.push_back(thread(roflan_f, v[i], _collide_balls));
            // roflan_f(v[i], _collide_balls);
            v[i]->collide(_collide_balls);
        }

//        for (int i=0; i<thread_n; i++)
//            thread_arr[i].join();
    }
    else
    {
        for (size_t i=0; i<_ball_n; i++)
            tree.add_ball(&_ball_arr[i]);

         tree.collide(_collide_balls);
    }
}
