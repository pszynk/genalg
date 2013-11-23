#ifndef GRIEWANK_H
#define GRIEWANK_H

#include <math.h>

#include "globals.h"
#include "types.h"
#include "myerr.h"

#define GRIEW_LB -40.0
#define GRIEW_UB  40.0
#define GRIEW_MAX_VAL(dim) (40.0 * (dim) + 1)

real_t griew_eval(const real_t *args, idx_t nargs);
real_t griew_reval(real_t eval);

real_t griewank(const real_t *args, idx_t nargs);

#endif /* end of include guard: GRIEWANK_H */
