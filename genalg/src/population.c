#include "population.h"


popul_t *pop_create(
        idx_t size)
{
    popul_t *pop = (popul_t*)malloc(sizeof(*pop) * size);
    pop->fitSum = 0;
    pop->genIdx = 0;
    pop->popSize = size;
    pop->indivs = indiv_create(pop->popSize);
    return pop;
}

void pop_destroy(
        popul_t *pop)
{
    idx_t i;
    // TODO OMP tylko finalziacja
    for (i = 0; i < pop->popSize; ++i) {
        /*indiv_destory(&(pop->indivs[i]));*/
        indiv_destory(pop->indivs + i);
    }
    free(pop->indivs);
    free(pop);
    pop = NULL;
}

void pop_eval(indiv_t *bestOne, popul_t *pop)
{
    real_t bestFit = 0, fitSum = 0;
    idx_t bestIdx = 0, i;
    // TODO OMP wykonywane popSize razy -> dobre miejsce na omp
#pragma omp parallel for schedule(dynamic,2) reduction(+: fitSum)
        /*num_threads(GENALG_OMP_NUM_THREADS)*/
    for (idx_t i = 0; i < pop->popSize; ++i) {
        real_t fit = indiv_eval(&(pop->indivs[i]));
    /*real_t feno[g_dim];*/
    /*chrom_to_real(pop->indivs[i].genotype, feno);*/
    /*real_t fit = pop->indivs[i].fitness = eval_funct(feno, g_dim);*/
        fitSum += fit;
#pragma omp critical
        {
        if (fit > bestFit) {
            bestFit = fit;
            bestIdx = i;
        }
        }
    }
    pop->fitSum = fitSum;
    indiv_copy(bestOne, &(pop->indivs[bestIdx]));
}

void pop_rand(popul_t *pop)
{
    idx_t i;
    // TODO OMP tylko inicjalziacja
#pragma omp parallel for schedule(dynamic,2)
    for (i = 0; i < pop->popSize; ++i) {
        indiv_rand(&(pop->indivs[i]));
    }
}


idx_t pop_select_rulette(
        const popul_t *pop,
        idx_t *selection,
        idx_t size)
{
    idx_t i, j;
    real_t psum = 0, rot;
    real_t sprob[pop->popSize];
    // TODO OMP dobre
    for (i = 0; i < pop->popSize; ++i) {
        psum += (pop->indivs[i].fitness / pop->fitSum);
        sprob[i] = psum;
    }
    // TODO OMP tez dobre
    for (i = 0; i < size; ++i) {
        rot = RANDOM_0_TO_1;
        j = 0;
        while(sprob[j] < rot) {
            ++j;
        }
        selection[i] = j;
    }
    return size;
}

#include <assert.h>
idx_t pop_select_best(
        const popul_t *pop,
        idx_t *selection,
        idx_t size)
{
    idx_t kbest = g_selParam;
    idx_t idxs[pop->popSize];
    int i, from = 0, to = pop->popSize - 1,
          tmp;
    int right, left;
    real_t split, fit;
    kbest = fmin(kbest, pop->popSize);

    // TODO taka inicjalizacja tablcy pojawia się też w turniejach
    // może oddzielna funkcja?
    // TODO OMP za proste na wątki?
    for (i = 0; i < pop->popSize; ++i) {
        idxs[i] = i;
    }

    while(from < to) {
        left = from;
        right = to;
        split = pop->indivs[idxs[(left + right) / 2]].fitness;
        /*
         *printf("[%d   %d]\n", from, to);
         *printf("split po: %f [%d]\n", split, (left+right)/2);
         */

        while (left < right) {
            fit = pop->indivs[idxs[left]].fitness;
            if (fit <= split) {
                tmp = idxs[right];
                idxs[right] = idxs[left];
                idxs[left] = tmp;
                --right;
            } else {
                ++left;
            }
        }

        if (pop->indivs[idxs[left]].fitness < split) {
            /*assert(left != 0);*/
            --left;
        }

        if (kbest <= left) {
            to = left;
        } else {
            from = left + 1;
        }
    }

    for (i = 0; i < size; ++i) {
        selection[i] = idxs[i % kbest];
    }

    return size;
}

idx_t pop_select_tournament(
        const popul_t *pop,
        idx_t *selection,
        idx_t size)
{
    idx_t i, s;
    idx_t toursize = g_selParam;
    idx_t idxs[pop->popSize];

    if (toursize < 1) {
        MYERR_ERR(-2, "Rozmar turnieju jest mniejszy od 1!");
    }

    //init idxs table
    // TODO też w best, oddzielna funckja?
#pragma omp parallel for
    for (i = 0; i < pop->popSize; ++i) {
        idxs[i] = i;
    }
    // przeprowadź size turnieji
    // TODO OMP dobre UWAGA na zmienne last (może deklaracja w pętli?)
#pragma omp parallel for schedule(dynamic,1) firstprivate(idxs)
    for (s = 0; s < size; ++s) {
        // ile zostało możliwych indeksów do wyboru
        idx_t last = pop->popSize - 1;
        // sprawdzaj kolejnych uczesników
        // TODO OMP NIE MOŻNA ZRÓWNOLEGLIC, zależy od poprzedniej iteracji
        idx_t t, rnd, rndidx,
              bestIdx = 0;
        real_t currFit, bestFit = -1;
        for (t = 0; t < toursize; ++t) {
            rnd = RANDOM_FROM(0, last);
            rndidx = idxs[rnd];
            currFit = pop->indivs[idxs[rndidx]].fitness;
            // obecny zwycięzca
            if (currFit > bestFit) {
                bestFit = currFit;
                bestIdx = rndidx;
            }
            // wybrany indeks przerzuć na koniec
            idxs[rnd] = idxs[last-1];
            idxs[last-1] = rndidx;
            --last;
        }
        selection[s] = bestIdx;
    }
    return size;
}

void pop_generate(
        popul_t *newPop,
        const popul_t *oldPop,
        idx_t *selection,
        idx_t nsel)
{
    idx_t i;
    // TODO OMP niezłe miejsce na zrównoleglenie
/*#pragma omp parallel for num_threads(GENALG_OMP_NUM_THREADS)*/
    for (i = 0; i < nsel; ++i) {
        /*printf("i %d sel[i] %d\n", i, selection[i]);*/
        indiv_copy(&(newPop->indivs[i]), &(oldPop->indivs[selection[i]]));
    }
    newPop->popSize = oldPop->popSize; // population size doesn't change in new population
}

void pop_cross(
        popul_t *pop)
{
    idx_t i, ncross = 0;
    idx_t icross[pop->popSize];
    // TODO OMP niezłe
/*#pragma omp parallel for num_threads(GENALG_OMP_NUM_THREADS)*/
    for (i = 0; i < pop->popSize; ++i) {
        if (RANDOM_0_TO_1 <= g_pCross) {
/*#pragma omp critical*/
            {
            icross[ncross++] = i;
            }
        }
    }
    shuffle_array(icross, ncross);

    ncross &= (idx_t) (~1);
    // TODO OMP nie wiem czy warto, ale może być  ich sporo
/*#pragma omp parallel for num_threads(GENALG_OMP_NUM_THREADS)*/
#pragma omp for schedule(dynamic, 2)
    for (i = 0; i < ncross; i+=2) {
        indiv_xcross(&(pop->indivs[icross[i]]), &(pop->indivs[icross[i+1]]));
    }
}

void pop_mut(
        popul_t *pop)
{
    idx_t i;
    // TODO OMP chyba warto
/*#pragma omp parallel for num_threads(GENALG_OMP_NUM_THREADS)*/
#pragma omp parallel for schedule(dynamic,5)
    for (i = 0; i < pop->popSize; ++i) {
        indiv_mut(&(pop->indivs[i]));
    }
}

char* pop_to_string(
        const popul_t *pop)
{
    char *chromStr = indiv_to_string(pop->indivs);
    char *str = (char*)malloc(sizeof(char) * (strlen(chromStr) * (pop->popSize + 10) +  pop->popSize * 5 + 200));
    idx_t slen = 0;
    str[0] = '\0';
    if ((slen =
          sprintf(
              str,
              "Population:\n %s = %d\n %s = %d\n %s = %f\n\t",
              "size         ", pop->popSize,
              "generation   ", pop->genIdx,
              "total fitness", pop->fitSum)
          ) <= 0) {
        MYERR_ERR(-3, "Blad w population -> string");
    }

    strcat(str, chromStr);
    free(chromStr);
    idx_t i;
    // TODO OMP tylko wypisywanie
    for (i = 1; i < pop->popSize; ++i) {
        strcat(str, "\n\t");
        chromStr = indiv_to_string(pop->indivs + i);
        strcat(str, chromStr);
        free(chromStr);
    }
    return str;
}


popStats_t *popStats_create()
{
    popStats_t *popstat     = (popStats_t*)malloc(sizeof(*popstat));
    popstat->evalResultMean = (real_t*)malloc(sizeof(*(popstat->evalResultMean)) * g_dim);
    popstat->evalResultSD  = (real_t*)malloc(sizeof(*(popstat->evalResultSD)) * g_dim);
    popstat->objResultsMean  = (real_t*)malloc(sizeof(*(popstat->objResultsMean)) * g_dim);
    popstat->objResultsSD   = (real_t*)malloc(sizeof(*(popstat->objResultsSD)) * g_dim);
    return popstat;
}

void popStats_destroy(
        popStats_t *popstat)
{
    free(popstat->evalResultMean);
    free(popstat->evalResultSD);
    free(popstat->objResultsMean);
    free(popstat->objResultsSD);
    free(popstat);
    popstat = NULL;
}

popStats_t *popStats_generate(
        popStats_t *popstat,
        const popul_t *pop)
{
    idx_t i, j;
    real_t tmpFS, tmpOF,
           tmpEval[g_dim], tmpRes[g_dim],
           tmpRealChrom[g_dim];

    if (popstat == NULL) {
        popstat = popStats_create();
    }

    popstat->popSize = pop->popSize;
    popstat->genIdx  = pop->genIdx;

    popstat->fitSum = pop->fitSum;

    /* MEANS */
    tmpFS = 0, tmpOF = 0;
    memset(tmpRes, 0, g_dim * sizeof(real_t));
    memset(tmpEval, 0, g_dim * sizeof(real_t));

    // TODO OMP nieważna funkcja
    for (i = 0; i < pop->popSize; ++i) {
        tmpFS += pop->indivs[i].fitness;
        tmpOF += reval_funct(pop->indivs[i].fitness);

        chrom_to_real(pop->indivs[i].genotype, tmpRealChrom);
        // TODO OMP nieważne
        for (j = 0; j < g_dim; ++j) {
            tmpEval[j] += pop->indivs[i].genotype[j];
            tmpRes[j] += tmpRealChrom[j];
        }
    }

    popstat->fitMean = tmpFS / pop->popSize;
    popstat->objFuncMean = tmpOF / pop->popSize;

    // TODO OMP nieważne
    for (i = 0; i < g_dim; ++i) {
        popstat->evalResultMean[i] = tmpEval[i] / pop->popSize;
        popstat->objResultsMean[i] = tmpRes[i] / pop->popSize;
    }

    /* VARS */
    tmpFS = 0, tmpOF = 0;
    memset(tmpRes, 0, g_dim * sizeof(real_t));
    memset(tmpEval, 0, g_dim * sizeof(real_t));

    // TODO OMP nieważne
    for (i = 0; i < pop->popSize; ++i) {
        tmpFS += pow(pop->indivs[i].fitness - popstat->fitMean, 2.0);
        tmpOF += pow(reval_funct(pop->indivs[i].fitness) - popstat->objFuncMean, 2.0);

        chrom_to_real(pop->indivs[i].genotype, tmpRealChrom);
        // TODO OMP nieważne
        for (j = 0; j < g_dim; ++j) {
            tmpEval[j] += pow(pop->indivs[i].genotype[j] - popstat->evalResultMean[j], 2.0);
            tmpRes[j] += pow(tmpRealChrom[j] - popstat->objResultsMean[j], 2.0);
        }
    }

    popstat->fitSD = sqrt(tmpFS / pop->popSize);
    popstat->objFuncSD = sqrt(tmpOF / pop->popSize);

    // TODO OMP nieważne
    for (i = 0; i < g_dim; ++i) {
        popstat->evalResultSD[i] = sqrt(tmpEval[i] / pop->popSize);
        popstat->objResultsSD[i] = sqrt(tmpRes[i] / pop->popSize);
    }
    return popstat;
}

char *popStat_to_string(
        popStats_t *popstat)
{
    idx_t strl = (g_dim * 4 * 20 + 600);
    char *str = (char*)malloc(sizeof(char) * strl);
    char tmpstr[500];
    idx_t i, slen = 0;

    str[0] = '\0';
    if ((slen =
          sprintf(
              tmpstr,
              "POPULATION STATISTICS:\n"
              " %s = %d\n"
              " %s = %d\n"
              "*** FITNESS STATS ************************\n"
              " %s = %f\n"
              " %s = %f\n"
              " %s = %f\n"
              "*** OBJ FUNC STATS ***********************\n"
              " %s = %f\n"
              " %s = %f\n",
              "size          ", popstat->popSize,
              "generation    ", popstat->genIdx,

              "fitness sum   ", popstat->fitSum,
              "fitness mean  ", popstat->fitMean,
              "fitness sd    ", popstat->fitSD,

              "obj func mean ", popstat->objFuncMean,
              "obj func sd   ", popstat->objFuncSD
              )
          ) <= 0) {
        MYERR_ERR(-3, "Blad w population stats -> string");
    }
    strcat(str, tmpstr);
    strcat(str, "*** EVAL POINTS STATS ********************\n");
    if ((slen =
          sprintf(
              tmpstr,
              "\t%15s%15s\n", "MEAN:", "SD:"
              )
          ) <= 0) {
        MYERR_ERR(-3, "Blad w population stats -> string");
    }
    strcat(str, tmpstr);
    // TODO OMP nieważne
    for (i = 0; i < g_dim; ++i) {
        if ((slen =
              sprintf(
                  tmpstr,
                  "%d\t%15f%15f\n",
                  i+1,
                  popstat->evalResultMean[i],
                  popstat->evalResultSD[i]
                  )
              ) <= 0) {
            MYERR_ERR(-3, "Blad w population stats -> string");
        }
        strcat(str, tmpstr);
    }

    strcat(str, "*** OBJ POINTS STATS *********************\n");
    if ((slen =
          sprintf(
              tmpstr,
              "\t%15s%15s\n", "MEAN:", "SD:"
              )
          ) <= 0) {
        MYERR_ERR(-3, "Blad w population stats -> string");
    }
    strcat(str, tmpstr);
    // TODO OMP nieważne
    for (i = 0; i < g_dim; ++i) {
        if ((slen =
              sprintf(
                  tmpstr,
                  "%d\t%15f%15f\n",
                  i+1,
                  popstat->objResultsMean[i],
                  popstat->objResultsSD[i]
                  )
              ) <= 0) {
            MYERR_ERR(-3, "Blad w population stats -> string");
        }
        strcat(str, tmpstr);
    }
    return str;
}
