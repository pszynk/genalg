#include "ackley.h"

real_t ackley_eval(const real_t *args, idx_t nargs)
{
    return (ACKLEY_MAX_VAL - ackley(args, nargs));
}

real_t ackley_reval(real_t eval)
{
    return (ACKLEY_MAX_VAL - eval);
}

real_t ackley(const real_t *args, idx_t nargs)
{
    real_t sum1 = 0.0, sum2 = 0.0, x;
    idx_t i;
    for (i = 0; i < nargs; ++i) {
        x = args[i];
        if ((x > ACKLEY_UB) || (x < ACKLEY_LB)) {
            MYERR_ERR(-1, "%s: x[%d] = %f ::  %f <= x <= %f",
                    "Blad ograniczen w funckji Ackleya",
                    i, x, ACKLEY_LB, ACKLEY_UB);
        }
        sum1 += x * x;
        sum2 += cos(x*2*M_PI);
    }
    return -20.0*exp(-0.2*sqrt(sum1/nargs))-exp(sum2/nargs)+20.0+exp(1);
}
