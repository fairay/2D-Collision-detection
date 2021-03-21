#include "bin_alg.h"
#include <iostream>

using namespace std;

void Scene::_bin_alg(bool is_threading)
{
    // BinAlg<75> field(_w, _h, _ball_n);
    _alg = shared_ptr<BaseTree>(new BinAlg<40>(_w, _h, _ball_n));

    for (size_t i=0; i<_ball_n; i++)
        _alg->add_ball(&_ball_arr[i]);

    if (is_threading)
        _alg->collide(_collide_balls);
    else
        _alg->collide_mult(_collide_balls, 8);
}
