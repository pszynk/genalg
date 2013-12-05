#include "griewank.h"

real_t griew_eval(const real_t *args, idx_t nargs)
{
    return ((40.0 * g_dim + 2.0) - griewank(args, nargs));
}

real_t griew_reval(real_t eval)
{
    return ((40.0 * g_dim  + 2.0) - eval);
}

real_t griewank(const real_t *args, idx_t nargs)
{
    real_t sum = 0, prod = 1, x;
    idx_t i;
    // TODO OMP tylko dim razy, czy warto?
    for (i = 0; i < nargs; ++i) {
        x = args[i];
        if ((x > GRIEW_UB) || (x < GRIEW_LB)) {
            MYERR_ERR(-1, "%s: x[%d] = %f ::  %f <= x <= %f",
                    "Blad ograniczen w funckji Griewanka",
                    i, x, GRIEW_LB, GRIEW_UB);
        }
        sum += x * x;
        prod *= cos(x / (i + 1));
    }
    return (sum / 40.0) + (1 - prod);
}
