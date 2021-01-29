#include "scene/scene.h"

void Scene::_brute_force()
{
    for (size_t i=0; i<_ball_n; i++)
        for (size_t j=i+1; j<_ball_n; j++)
        {
            double dist = sqrt(pow(_ball_arr[i].pos.x - _ball_arr[j].pos.x, 2) +
                               pow(_ball_arr[i].pos.y - _ball_arr[j].pos.y, 2));
            if (dist <= _ball_r*2)
                _collide_balls(_ball_arr[i], _ball_arr[j]);
        }
}
