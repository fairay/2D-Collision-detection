#ifndef CONFIG_H
#define CONFIG_H
#include "scene/alg.h"

#define         MEASURE

#ifdef MEASURE
#define BALL_N          10000
#define IS_THREADING    true
const upd_t ALG =       BIN_TREE;
#else
#define BALL_N          100
#endif // MEASURE

#endif // CONFIG_H
