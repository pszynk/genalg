#ifndef POPULATION_H
#define POPULATION_H

/*
 * typy danych i funkcje zwiazane z populacja
 */
#include <stdlib.h>
#include <math.h>

#include "types.h"
#include "individual.h"
#include "evaluation.h"


#define MAX_POPULATION 100000

typedef struct _popul_t {
    idx_t    popSize;
    idx_t    genIdx;
    real_t   fitSum;
    indiv_t *indivs;
} popul_t;

typedef struct popStats_t {
    idx_t   popSize;
    idx_t   genIdx;

    real_t  fitSum;

    real_t  fitMean;
    real_t  fitSD;

    real_t  objFuncMean;
    real_t  objFuncSD;

    real_t *evalResultMean;
    real_t *evalResultSD;

    real_t *objResultsMean;
    real_t *objResultsSD;
} popStats_t;

popul_t *pop_create(
        idx_t size);

void pop_destroy(
        popul_t *pop);

void pop_eval(
        indiv_t *best_one,
        popul_t *pop);

void pop_rand(
        grstate_t *grstate,
        popul_t *pop);

void pop_generate(
        popul_t *newPop,
        const popul_t *oldPop,
        idx_t *selection,
        idx_t nsel);

void pop_cross(
        grstate_t *grstate,
        popul_t *pop);

void pop_mut(
        grstate_t *grstate,
        popul_t *pop);

idx_t pop_find_best_or_worst(
        idx_t bwkcount,
        char  best,
        const popul_t *pop,
        idx_t *selection,
        idx_t size);

idx_t pop_select_rulette(
        grstate_t *grstate,
        const popul_t *pop,
        idx_t *selection,
        idx_t size);

idx_t pop_select_best(
        grstate_t *grstate,
        const popul_t *pop,
        idx_t *selection,
        idx_t size);

idx_t pop_select_tournament(
        grstate_t *grstate,
        const popul_t *pop,
        idx_t *selection,
        idx_t size);

void sortTab(const popul_t *pop, idx_t *tab, idx_t size);

char *pop_to_string(
        const popul_t *pop);

/* STATS */

popStats_t *popStats_create();

void popStats_destroy(
        popStats_t *popstat);

popStats_t *popStats_generate(
        popStats_t *popstat,
        const popul_t *pop);

char *popStat_to_string(
        popStats_t *popstat);

#endif /* end of include guard: POPULATION_H */
