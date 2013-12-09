#ifndef GRANDOM_H
#define GRANDOM_H

#include <stdlib.h>
#include <limits.h>
#include "utils.h"
#include "locales.h"
#include "types.h"

#define __NRAND_MAX 2147483648

#define RANDOM_0_TO_1(xsubi) ((real_t)erand48(xsubi))

#define RANDOM_FROM(xsubi, a, b) \
    (idx_t)(MIN(((a) + nrand48(xsubi)/(__NRAND_MAX/((b) - (a)))), ((b)-1)))

typedef struct _grstate {
    unsigned short xsubi[3];
} grstate_t;

void grstate_copy(
        grstate_t *to,
        const grstate_t *from);

void grstate_seed(
        grstate_t *grstate,
        unsigned int seed);

void grstate_shift(
        grstate_t *to,
        const grstate_t *from,
        unsigned shift);

void shuffle_array(
        grstate_t *grstate,
        idx_t *array,
        idx_t n);

#endif /* end of include guard: GRANDOM_H */
