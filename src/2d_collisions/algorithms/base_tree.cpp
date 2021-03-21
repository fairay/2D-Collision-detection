#include "base_tree.h"
#include <thread>
#include <iostream>

using namespace std;

BaseTree::BaseTree() {}
BaseTree::~BaseTree() {}

bool BaseTree::is_void()
{
    //return _ball_arr.size() < 2; //&& _is_leaf;
    return _is_leaf && (_ball_arr.size() < 2);
}

int BaseTree::deep() {return 0;}
bool BaseTree::is_leaf() { return _is_leaf; }

void BaseTree::collide_mult(collide_func, int) {}
void BaseTree::add_ball_mult(Ball*) {}
void BaseTree::_add_ball_leaves(Ball*, bool) {}
void BaseTree::_init_leaves() {}

void BaseTree::_collide_leaf(collide_func f)
{
    for (size_t i=0; i<_ball_arr.size(); i++)
        for (size_t j=i+1; j<_ball_arr.size(); j++)
            collide_balls(_ball_arr[i], _ball_arr[j], f);
}

void BaseTree::select_nodes(std::vector<BaseTree*>&, int, int) {}

void BaseTree::show(std::shared_ptr<QGraphicsScene> &_qscene) {}


void _thread_add(BaseTree* tree, vector<Ball>* ball_arr, int from_, int to_)
{
    for (int i=from_; i<to_; i++)
        tree->add_ball_mult(&(*ball_arr)[i]);
}

void thread_add_balls(BaseTree* tree, vector<Ball>& ball_arr)
{
    int thread_n = ADD_THREAD_N;
    vector<thread> thread_arr;
    int ball_n = ball_arr.size();
    thread_arr.reserve(thread_n);

    for (int i = 0; i < thread_n; i++)
       thread_arr.push_back(thread(_thread_add, tree, &ball_arr,
                                   (ball_n * i) / thread_n,
                                   (ball_n * (i+1)) / thread_n));
    for (int i = 0; i < thread_n; i++)
       thread_arr[i].join();
}

void thread_collide_balls(BaseTree* tree, collide_func f, int deep)
{
    if (tree->is_void()) return;
    if (deep >= 0)
        tree->collide_mult(f, deep);
    else
        tree->collide(f);
}



void collide_balls(Ball* b1, Ball* b2, collide_func f)
{
    double dist = sqrt(pow(b1->pos.x - b2->pos.x, 2) +
                       pow(b1->pos.y - b2->pos.y, 2));
    if (dist <= b1->r + b2->r)
        f(*b1, *b2);
}


