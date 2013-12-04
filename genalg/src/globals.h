#ifndef GLOBALS_H
#define GLOBALS_H

#include "types.h"

enum Functions {
    GRIEWANK,
    ACKLEY,
    KWADRATX,
};

enum Selections {
    BEST,
    RULETTE,
    TOURNAMENT,
};

int g_VERBOSELVL;

unsigned int g_seed;

//globalne parametry
idx_t g_dim,
      g_maxGen,
      g_popSize,
      g_selParam;

real_t g_pCross,
       g_pMut;

enum Functions g_funcType;

enum Selections g_selType;

//globalne parametry zależne
idx_t g_gen,
      g_bitvecPerChrom,
      g_bitPerChrom;

real_t g_funcLB,
       g_funcUB;


real_t (*g_evalFunct)    (const real_t*, idx_t),
       (*g_revalFunct)    (real_t eval),
       (*g_minimizeFunct)(const real_t*, idx_t);

typedef struct _popul_t popul_t;

idx_t (*g_selFunct) (
        const popul_t *pop,
        idx_t *selection,
        idx_t size);
#endif /* end of include guard: GLOBALS_H */
