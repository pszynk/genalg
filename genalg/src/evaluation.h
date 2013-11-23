#ifndef EVALUATION_H
#define EVALUATION_H

#include "types.h"
#include "globals.h"
#include "myerr.h"


static inline real_t eval_funct(const real_t *args, idx_t nargs) {
    return (*g_evalFunct)(args, nargs);
};

static inline real_t minmized_funct(const real_t *args, idx_t nargs) {
    return (*g_minimizeFunct)(args, nargs);
};

static inline real_t reval_funct(real_t arg) {
    return (*g_revalFunct)(arg);
};
#endif /* end of include guard: EVALUATION_H */
