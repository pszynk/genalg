#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

/*
 * Typy i funkcje na osobnikach
 */
#include <math.h>

#include "types.h"
#include "evaluation.h"
#include "chromosome.h"

typedef struct indiv_t {
    chrom_t genotype;
    real_t fitness;
} indiv_t;

indiv_t* indiv_create(
        idx_t count);

void indiv_destory(
        indiv_t *indiv);

void indiv_copy(
        indiv_t *to,
        const indiv_t *from);

void indiv_rand(
        grstate_t *grstate,
        indiv_t *indiv);

real_t indiv_eval(
        indiv_t *indiv);

void indiv_xcross(
        grstate_t *grstate,
        indiv_t *indiv1,
        indiv_t *indiv2);

void indiv_mut(
        grstate_t *grstate,
        indiv_t *indiv);

char* indiv_to_string(
        indiv_t *indiv);


#endif /* end of include guard: INDIVIDUAL_H */
