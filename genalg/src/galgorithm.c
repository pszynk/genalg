#include "galgorithm.h"

void print_gen_info(
        idx_t gen,
        real_t fitSum,
        real_t bestFitAll,
        real_t bestFitCurr)
{
    printf("generacja: %d\n", gen);
    printf("\tsum fit:          %f\n"
           "\tbest eval:        %f\n"
           "\tbest in pop eval: %f\n"
           "\tbest val:         %f\n",
           fitSum,
           bestFitAll,
           bestFitCurr,
           g_revalFunct(bestFitAll)
          );
}
void print_populs(
        const popul_t *oldPop,
        const popul_t *newPop)
{
    char* ops = pop_to_string(oldPop);
    char* nps = pop_to_string(newPop);
    printf("****** OLD POP \n%s\n\n****** NEW POP \n%s\n\n",
            ops, nps);
    free(ops);
    free(nps);
}


real_t galgorithm(stats_t *stats)
{
    idx_t gen, nsel;
    real_t bestFval, f = 0, val;
    idx_t selection[g_popSize];
    indiv_t *bestOfAll, *bestOfPop, *tmpIndiv;
    popul_t *oldPop, *newPop, *tmpPop;

    bestOfAll = indiv_create(1);
    bestOfPop = indiv_create(1);
    oldPop = pop_create(g_popSize);
    newPop = pop_create(g_popSize);
    pop_rand(oldPop);
    pop_eval(bestOfAll, oldPop);

    bestFval = indiv_eval(bestOfAll);
    // TODO nie można zrównoleglić, zależy od poprzedniej iteracji
    for (gen = 1; gen <= g_maxGen; ++gen) {
        val = g_revalFunct(bestFval);
        /* jezeli jestesmy blisko minimum, przerwij algorytm */
        if ((val >= EPSILON) && (val <= EPSILON)) {
            break;
        }
        /* selection */
        nsel = g_selFunct(oldPop, selection, oldPop->popSize);
        /* generate new pop */
        pop_generate(newPop, oldPop, selection, nsel);
        newPop->genIdx=gen;
        /* crossover */
        pop_cross(newPop);
        /* mutation */
        pop_mut(newPop);
        /* evaluate */
        pop_eval(bestOfPop, newPop);
        f = indiv_eval(bestOfPop);
        if (f > bestFval) {
            bestFval = f;
            tmpIndiv = bestOfAll;
            bestOfAll = bestOfPop;
            bestOfPop = tmpIndiv;
        }
        tmpPop = oldPop;
        oldPop = newPop;
        newPop = tmpPop;
        if (g_VERBOSELVL > 0) {
            print_gen_info(
                    gen,
                    newPop->fitSum,
                    bestFval,
                    f);
        }
    }

    if (g_VERBOSELVL > 0) {
        popStats_t *popST = popStats_generate(NULL, oldPop);
        char *popSTstr = popStat_to_string(popST);
        printf("\nSTATISTICS:\n%s\n", popSTstr);
        free(popSTstr);
        popStats_destroy(popST);
    }

    if (g_VERBOSELVL > 0) {
        real_t xxx[g_dim];
        chrom_to_real(bestOfAll->genotype, xxx);
        idx_t i;
        for (i = 0; i < g_dim; ++i) {
            printf("%f, ", xxx[i]);
        }
        printf("\n");
    }

    pop_destroy(oldPop);
    pop_destroy(newPop);

    return bestFval;
}
