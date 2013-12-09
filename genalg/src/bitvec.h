#ifndef BITVEC_H
#define BITVEC_H

#include "types.h"
#include "locales.h"
#include "globals.h"
#include "grandom.h"
#include "myerr.h"
#include <stdio.h>

#define MAX_BITVEC_VALUE ((bitvec_t) ~(0))
#define BIT_PER_BITVEC (sizeof(bitvec_t) * BIT_PER_BYTE)
//#define RANDOM_BITVEC(xsubi) ((((bitvec_t)erand48(xsubi))<<15) + (bitvec_t)rand())

/*typedef uint32_t bitvec_t;*/
#define RANDOM_BITVEC(xsubi) (((bitvec_t)nrand48(xsubi)))
typedef uint16_t bitvec_t;

bitvec_t binary_to_gray(bitvec_t num);
bitvec_t gray_to_binary(bitvec_t num);

real_t binary_to_real(
        bitvec_t binary);

void bitvec_xcross(
        idx_t nbit,
        bitvec_t *bv1,
        bitvec_t *bv2);

void bitvec_flip(
        idx_t nbit,
        bitvec_t *bv);

char* bitvec_to_string(
        const bitvec_t *bitvec);


#endif /* end of include guard: BITVEC_H */
