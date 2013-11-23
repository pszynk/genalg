#include "kwadratx.h"

real_t kwx_eval(const real_t *args, idx_t nargs)
{
    return ((40.0 * 40.0 * g_dim) - kwadratx(args, nargs)); ///g_dim;
}

real_t kwx_reval(real_t eval)
{
    return ((40.0 * 40.0 * g_dim) - eval); // * g_dim;
}

real_t kwadratx(const real_t *args, idx_t nargs)
{
    //printf("<<<");
    //int ii;
    //for (ii = 0; ii < nargs; ++ii) {
        //printf("%f, ", args[ii]);
    //}
    //printf(">>>\n");
    real_t sum = 0, x;
    idx_t i;
    for (i = 0; i < nargs; ++i) {
        x = args[i];
        if ((x > KWX_UB) || (x < KWX_LB)) {
            MYERR_ERR(-1, "%s: x[%d] = %f ::  %f <= x <= %f",
                    "Blad ograniczen w funckji kwadratxa",
                    i, x, KWX_LB, KWX_UB);
        }
        sum += x * x;
    }
    return sum;
}
