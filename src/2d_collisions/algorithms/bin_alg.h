#ifndef BIN_ALG_H
#define BIN_ALG_H

#include "base_tree.h"

template <int cell_n>
class BinAlg: public BaseTree
{
public:
    BinAlg(double w, double h, int ball_n);
    ~BinAlg();

    virtual void add_ball(Ball* ball);
    virtual void collide(collide_func f);
private:
    double _w, _h;
    double _w_step, _h_step;
    bool _is_selective;
    // std::vector<Ball*> cell_matr[cell_n][cell_n];
    Ball* cell_matr[cell_n][cell_n][1000/cell_n];
    int n_matr[cell_n][cell_n];

    std::list<std::pair<int, int>> used_cells;

    void _collide_cell(collide_func f, int i, int j);
};


#ifndef BIN_ALG_HPP
#include "bin_alg.hpp"
#endif

#endif // BIN_ALG_H
