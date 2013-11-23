/**
 *      @file  test.c
 *     @brief
 *
 * Detailed description starts here.
 *
 *    @author  Paweł Szynkiewicz (PSz), pszynk@gmail.com
 *
 *  @internal
 *    Created  01.11.2013 14:46:24
 *   Revision  $REVISION$
 *   Compiler  gcc
 *    Company  Politechnika Warszawska
 *  Copyright  Copyright (c) 2013, Paweł Szynkiewicz
 *
 *=====================================================================================
 */
#include <err.h>
#include <stdio.h>
#include <time.h>
#include "myerr.h"
#include <unistd.h>
#include "types.h"
#include "griewank.h"
#include "bitvec.h"
#include "chromosome.h"
#include "population.h"
#define RED "\\\[\\033[31m\\]"

int rsum(int argc) {
    int r[argc];
    int i;
    for (i = 0; i < argc; ++i) {
        r[i] = rand();
    }
    for (i = 0; i < argc; ++i) {
        printf("%d [%d]", r[i], i);
    }
    return 0;
}

int main_t(int argc, const char *argv[])
{
    g_dim = 3;
    g_bitvecPerChrom = g_dim;
    g_bitPerChrom = g_bitvecPerChrom  * 32;
    g_evalFunct = &griew_eval;
    g_minimizeFunct = &griewank;
    g_funcLB = -40;
    g_funcUB = 40;
    real_t args[] = {0, 0, 0};
    printf("%f griew\n", griew_eval(args, 3));


    /*srand(time(NULL));*/
    srand(13);
    rsum(3);
    return 0;
    printf("%d \n",
            RANDOM_BITVEC
            );
    idx_t ii;
    for (ii = 0; ii < 200; ++ii) {
        /*printf("%f\n", (double)((bitvec_t)(rand()<< 16) + (bitvec_t)rand())/(double)MAX_BITVEC_VALUE);*/
        printf("%u\n", RANDOM_BITVEC);
    }
    popul_t *pop = pop_create(100);
    bitvec_t bv = MAX_BITVEC_VALUE;
    char *str = bitvec_to_string(&bv);
    printf("<%s>\n", str);
    chrom_t chrom = chrom_create();
    chrom[0] = 2;
    chrom[1] = MAX_BITVEC_VALUE;
    char* strchrom = chrom_to_string(chrom);
    printf("<%s>\n", strchrom);
    indiv_t indiv;
    indiv.fitness = -111239.3211231;
    indiv.genotype = chrom;
    char* indstr = indiv_to_string(&indiv);
    printf("->> %s\n", indstr);
    free(indstr);
    pop_rand(pop);
    indiv_t *indivBest = indiv_create(1);
    pop_eval(indivBest, pop);
    char *popstr = pop_to_string(pop);
    indstr = indiv_to_string(indivBest);
    printf("->> %s\n", indstr);
    printf("->> %s\n", popstr);
    free(popstr);
    free(indstr);
    free(strchrom);
    free(str);
    free(pop);
    return 0;
}

