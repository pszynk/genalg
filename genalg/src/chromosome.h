/*
 * Typ chromosom i funkcje pomocnicze
 */

#ifndef CHROMOSOME_H
#define CHROMOSOME_H

#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "globals.h"
#include "bitvec.h"
#include "evaluation.h"
#include "myerr.h"


typedef bitvec_t *chrom_t;

chrom_t chrom_create();

void chrom_destroy(
        chrom_t chrom);

void chrom_copy(
        chrom_t to,
        const chrom_t from);

void chrom_rand(
        grstate_t *grstate,
        chrom_t chrom);

idx_t chrom_xcross(
        grstate_t *grstate,
        chrom_t chrom1,
        chrom_t chrom2);

void chrom_mut(
        grstate_t *grstate,
        chrom_t chrom);

void chrom_to_real(
        const chrom_t chrom,
        real_t *value);

char* chrom_to_string(
        const chrom_t chrom);

#endif /* end of include guard: CHROMOSOME_H */
