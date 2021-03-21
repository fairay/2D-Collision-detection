#ifndef BIN_ALG_HPP
#define BIN_ALG_HPP

#include "bin_alg.h"
#include <iostream>

#define MAX_OPTIMIZE_N 1200


template <int cell_n>
BinAlg<cell_n>::BinAlg(double w, double h, int ball_n): _w(w), _h(h)
{
    _w_step = w / cell_n;
    _h_step = h / cell_n;

    for (int i=0; i<cell_n; i++)
        for (int j=0; j<cell_n; j++)
            n_matr[i][j] = 0;
    _is_selective = (ball_n < 1200);

//    for (int i=1; i<cell_n; i++)
//        memcpy(&n_matr[i][0], &n_matr[0][0], cell_n);
}
template <int cell_n>
BinAlg<cell_n>::~BinAlg() {}


template <int cell_n>
void BinAlg<cell_n>::show(std::shared_ptr<QGraphicsScene> &_qscene)
{
    for (double x=0; x + 0.5 < _w; x+=_h_step)
        _qscene->addLine(x, 0,
                         x, _h,
                         QPen(Qt::blue));

    for (double y=0; y + 0.5 < _h; y+=_w_step)
        _qscene->addLine(0, y,
                         _w, y,
                         QPen(Qt::blue));
}


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
void BinAlg<cell_n>::collide_mult(collide_func f, int)
{
    float thread_step = (float)cell_n / THREAD_N;
    #pragma omp parallel for num_threads(THREAD_N)
    for (int k=0; k<THREAD_N; k++)
    {
        int beg = thread_step * k;
        int end = thread_step * (k + 1);
        for (int i=beg; i<end; i++)
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
    }
    else
    {
        for (size_t l=0; l<size; l++)
            for (size_t k=l+1; k<size; k++)
                collide_balls(arr[l], arr[k], f);
    }

    n_matr[i][j] = 0;
}

#endif // BIN_ALG_HPP
