#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include "types.h"

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))

#define RANDOM_0_TO_1 ((real_t)rand()/(real_t)RAND_MAX)
#define RANDOM_FROM(a, b) \
    MIN(((a) + rand()/(RAND_MAX/((b) - (a)))), ((b)-1))


static inline void shuffle_array(idx_t *array, idx_t n)
{
    if (n > 1)
    {
        idx_t i;
        // TODO OMP może być warto
        for (i = 0; i < n - 1; i++)
        {
          idx_t j = i + rand() / (RAND_MAX / (n - i) + 1);
          idx_t t = array[j];
          array[j] = array[i];
          array[i] = t;
        }
    }
}
#endif /* end of include guard: UTILS_H */
