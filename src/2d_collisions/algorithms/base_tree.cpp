#include "base_tree.h"

BaseTree::BaseTree() {}
BaseTree::~BaseTree() {}

bool BaseTree::is_void()
{
    return _ball_arr.size() < 2;
}

int BaseTree::deep() {return 0;}

void BaseTree::collide_mult(collide_func f, size_t deep) {}

void BaseTree::_collide_leaf(collide_func f)
{
    for (size_t i=0; i<_ball_arr.size(); i++)
        for (size_t j=i+1; j<_ball_arr.size(); j++)
        {
            double dist = sqrt(pow(_ball_arr[i]->pos.x - _ball_arr[j]->pos.x, 2) +
                               pow(_ball_arr[i]->pos.y - _ball_arr[j]->pos.y, 2));
            if (dist <= _ball_arr[i]->r + _ball_arr[j]->r)
                f(*_ball_arr[i], *_ball_arr[j]);
        }
}
