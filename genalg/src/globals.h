#ifndef GLOBALS_H
#define GLOBALS_H

#include "types.h"

enum Functions {
    GRIEWANK,
    ACKLEY,
    KWADRATX,
};

//globalne parametry
idx_t g_dim,
      g_maxGen,
      g_popSize;

real_t g_pCross,
       g_pMut;

enum Functions g_funcType;

//globalne parametry zależne
idx_t g_gen,
      g_bitvecPerChrom,
      g_bitPerChrom;

real_t g_funcLB,
       g_funcUB;


real_t (*g_evalFunct)    (const real_t*, idx_t),
       (*g_revalFunct)    (real_t eval),
       (*g_minimizeFunct)(const real_t*, idx_t);

#endif /* end of include guard: GLOBALS_H */
