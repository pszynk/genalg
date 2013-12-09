#ifndef ACKLEY_H
#define ACKLEY_H

#include <math.h>

#include "globals.h"
#include "types.h"
#include "myerr.h"

#define ACKLEY_LB -40.0
#define ACKLEY_UB  40.0
#define ACKLEY_MAX_VAL 24

real_t ackley_eval(const real_t *args, idx_t nargs);
real_t ackley_reval(real_t eval);

real_t ackley(const real_t *args, idx_t nargs);

#endif /* end of include guard: ACKLEY_H */
