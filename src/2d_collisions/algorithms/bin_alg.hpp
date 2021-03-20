#ifndef BIN_ALG_HPP
#define BIN_ALG_HPP

#include "bin_alg.h"
#include <iostream>

template <int cell_n>
BinAlg<cell_n>::BinAlg(double w, double h, int ball_n): _w(w), _h(h)
{
    _w_step = w / cell_n;
    _h_step = h / cell_n;

    for (int i=0; i<cell_n; i++)
        for (int j=0; j<cell_n; j++)
            n_matr[i][j] = 0;
    _is_selective = false;

//    for (int i=1; i<cell_n; i++)
//        memcpy(&n_matr[i][0], &n_matr[0][0], cell_n);
}
template <int cell_n>
BinAlg<cell_n>::~BinAlg() {}


template <int cell_n>
void BinAlg<cell_n>::add_ball(Ball* ball)
{
    int i_min = (fmax(0, ball->pos.x - ball->r)) / _w_step;
    int i_max = (fmin(_w - 1e-5, ball->pos.x + ball->r)) / _w_step;
    int j_min = (fmax(0 , ball->pos.y - ball->r)) / _h_step;
    int j_max = (fmin(_h - 1e-5, ball->pos.y - ball->r)) / _h_step;

    for (int i=i_min; i<=i_max; i++)
        for (int j=j_min; j<=j_max; j++)
        {
            cell_matr[i][j][n_matr[i][j]++] = ball;
            if (_is_selective && n_matr[i][j] == 2)
                used_cells.push_front(std::pair<int, int>(i, j));
        }
}


template <int cell_n>
void BinAlg<cell_n>::collide(collide_func f)
{
    if (_is_selective)
    {
        for (auto a : used_cells)
            _collide_cell(f, a.first, a.second);
    }
    else
    {
        for (int i=0; i<cell_n; i++)
            for (int j=0; j<cell_n; j++)
                _collide_cell(f, i, j);
    }

}

template <int cell_n>
void BinAlg<cell_n>::_collide_cell(collide_func f, int i, int j)
{
    Ball** arr = cell_matr[i][j];
    size_t size = n_matr[i][j];
    if (size < 2) return;
    else if (size == 2)
    {
        collide_balls(arr[0], arr[1], f);
//        double dist = sqrt(pow(arr[0]->pos.x - arr[1]->pos.x, 2) +
//                           pow(arr[0]->pos.y - arr[1]->pos.y, 2));
//        if (dist <= arr[0]->r + arr[1]->r)
//            f(*arr[0], *arr[1]);
    }
    else
    {
        for (size_t l=0; l<size; l++)
            for (size_t k=l+1; k<size; k++)
            {
                collide_balls(arr[l], arr[k], f);
    //            double dist = sqrt(pow(arr[l]->pos.x - arr[k]->pos.x, 2) +
    //                               pow(arr[l]->pos.y - arr[k]->pos.y, 2));
    //            if (dist <= arr[l]->r + arr[k]->r)
    //                f(*arr[l], *arr[k]);
            }
    }

    n_matr[i][j] = 0;
}

#endif // BIN_ALG_HPP
