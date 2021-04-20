#ifndef CONFIG_H
#define CONFIG_H
#include "scene/alg.h"
#include <QColor>

// #define         MEASURE
#define         BALL_R  5.0

#ifdef          MEASURE
#define BALL_N          100
#define IS_THREADING    false
const upd_t ALG =       BIN_ALG;
#else
#define BALL_N          100
#endif // MEASURE

const Qt::GlobalColor BALL_COLOR =  Qt::blue;
const Qt::GlobalColor SPEED_COLOR = Qt::red;
const Qt::GlobalColor MASH_COLOR =  Qt::black;

#endif // CONFIG_H
