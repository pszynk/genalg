#ifndef KWADRATX_H
#define KWADRATX_H

#include <math.h>

#include "globals.h"
#include "types.h"
#include "myerr.h"

#define KWX_LB -40.0
#define KWX_UB  40.0

real_t kwx_eval(const real_t *args, idx_t nargs);
real_t kwx_reval(real_t eval);

real_t kwadratx(const real_t *args, idx_t nargs);

#endif /* end of include guard: KWADRATX_H */
