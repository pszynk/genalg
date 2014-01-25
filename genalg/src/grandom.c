#include "grandom.h"

#define __MAGIC_NUMBER1 4253
#define __MAGIC_NUMBER2 44497
#define __MAGIC_NUMBER3 60649

void grstate_copy(
        grstate_t *to,
        const grstate_t *from)
{
    to->xsubi[0] = from->xsubi[0];
    to->xsubi[1] = from->xsubi[1];
    to->xsubi[2] = from->xsubi[2];
}


void grstate_seed(
        grstate_t *grstate,
        unsigned int seed)
{
    unsigned short xsubi[3];
    xsubi[0] = seed;
    xsubi[1] = seed >> (CHAR_BIT * (sizeof(unsigned short)));
    xsubi[2] = __MAGIC_NUMBER3;
    grstate->xsubi[0] = RANDOM_FROM(xsubi, 1, USHRT_MAX);
    grstate->xsubi[1] = RANDOM_FROM(xsubi, 1, USHRT_MAX);
    grstate->xsubi[2] = RANDOM_FROM(xsubi, 1, USHRT_MAX);
}

void grstate_shift(
        grstate_t *to,
        const grstate_t *from,
        unsigned shift)
{
    to->xsubi[0] += __MAGIC_NUMBER1 * shift;
    to->xsubi[1] += __MAGIC_NUMBER2 * shift;
    to->xsubi[2] += __MAGIC_NUMBER3 * shift;
}


void shuffle_array(
        grstate_t *grstate,
        idx_t *array,
        idx_t n)
{
    if (n > 1)
    {
        idx_t i;
        for (i = 0; i < n - 1; i++)
        {
          idx_t j = RANDOM_FROM(grstate->xsubi, i, n-1);
          idx_t t = array[j];
          array[j] = array[i];
          array[i] = t;
        }
    }
}
