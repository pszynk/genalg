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
    for (i = 0; i < pop->popSize; ++i) {
        indiv_destory(pop->indivs + i);
    }
    free(pop->indivs);
    free(pop);
    pop = NULL;
}

void pop_rand(
        grstate_t *grstate,
        popul_t *pop)
{
    idx_t i;
    grstate_t _thread_grstate;
    grstate_shift(&_thread_grstate, grstate, g_mpiProcId);
    for (i = 0; i < pop->popSize; ++i) {
        indiv_rand(grstate, &(pop->indivs[i]));
    }
    grstate_copy(grstate, &_thread_grstate);
}


void pop_eval(indiv_t *bestOne, popul_t *pop)
{
    real_t bestFit = 0, fitSum = 0;
    idx_t bestIdx = 0;
    idx_t i;
    for (i = 0; i < pop->popSize; ++i) {
        real_t fit = indiv_eval(&(pop->indivs[i]));
        fitSum += fit;
        if (fit > bestFit) {
            bestFit = fit;
            bestIdx = i;
        }
    }
    pop->fitSum = fitSum;
    indiv_copy(bestOne, &(pop->indivs[bestIdx]));
}

idx_t pop_find_best_or_worst(
        idx_t bwkcount,
        char  best,
        const popul_t *pop,
        idx_t *selection,
        idx_t size)
{
    idx_t idxs[pop->popSize];
    int i, kcount, from = 0, to = pop->popSize - 1,
          tmp;
    int right, left;
    real_t split, fit;

    bwkcount = fmin(bwkcount, pop->popSize);
    kcount = bwkcount;
    if (!best) {
        kcount = pop->popSize - kcount;
    }

    if (kcount == 0) return 0;

    for (i = 0; i < pop->popSize; ++i) {
        idxs[i] = i;
    }

    while(from < to) {
        left = from;
        right = to;
        split = pop->indivs[idxs[(left + right) / 2]].fitness;

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
            --left;
        }

        if (kcount <= left) {
            to = left;
        } else {
            from = left + 1;
        }
    }

    if (best) {
        for (i = 0; i < size; ++i) {
            selection[i] = idxs[i % bwkcount];
        }
    } else {
        for (i = 0; i < size; ++i) {
            selection[i] = idxs[(i % bwkcount) + kcount - 1];
        }
    }

    return size;
}

idx_t pop_select_rulette(
        grstate_t *grstate,
        const popul_t *pop,
        idx_t *selection,
        idx_t size)
{
    idx_t i, j;
    real_t psum = 0, rot;
    real_t sprob[pop->popSize];

    for (i = 0; i < pop->popSize; ++i) {
        psum += (pop->indivs[i].fitness / pop->fitSum);
        sprob[i] = psum;
    }

    grstate_t _thread_grstate;
    grstate_shift(&_thread_grstate, grstate, g_mpiProcId);
    for (i = 0; i < size; ++i) {
        rot = RANDOM_0_TO_1(_thread_grstate.xsubi);
        j = 0;
        while(sprob[j] < rot) {
            ++j;
        }
        selection[i] = j;
    }
    grstate_copy(grstate, &_thread_grstate);
    return size;
}

idx_t pop_select_best(
        grstate_t *grstate,
        const popul_t *pop,
        idx_t *selection,
        idx_t size)
{
    idx_t kbest = g_selParam;
    return pop_find_best_or_worst(kbest, 1, pop, selection, size);
    /*idx_t idxs[pop->popSize];*/
    /*int i, from = 0, to = pop->popSize - 1,*/
          /*tmp;*/
    /*int right, left;*/
    /*real_t split, fit;*/
    /*kbest = fmin(kbest, pop->popSize);*/

    /*for (i = 0; i < pop->popSize; ++i) {*/
        /*idxs[i] = i;*/
    /*}*/

    /*while(from < to) {*/
        /*left = from;*/
        /*right = to;*/
        /*split = pop->indivs[idxs[(left + right) / 2]].fitness;*/

        /*while (left < right) {*/
            /*fit = pop->indivs[idxs[left]].fitness;*/
            /*if (fit <= split) {*/
                /*tmp = idxs[right];*/
                /*idxs[right] = idxs[left];*/
                /*idxs[left] = tmp;*/
                /*--right;*/
            /*} else {*/
                /*++left;*/
            /*}*/
        /*}*/

        /*if (pop->indivs[idxs[left]].fitness < split) {*/
            /*--left;*/
        /*}*/

        /*if (kbest <= left) {*/
            /*to = left;*/
        /*} else {*/
            /*from = left + 1;*/
        /*}*/
    /*}*/

    /*for (i = 0; i < size; ++i) {*/
        /*selection[i] = idxs[i % kbest];*/
    /*}*/

    /*return size;*/
}


idx_t pop_select_tournament(
        grstate_t *grstate,
        const popul_t *pop,
        idx_t *selection,
        idx_t size)
{
    idx_t toursize = g_selParam;

    if (toursize < 1) {
        MYERR_ERR(-2, "Rozmar turnieju jest mniejszy od 1!");
    }
    idx_t bestIdx = 0;
    real_t bestFit = -1;

    idx_t i, s;
    idx_t idxs[pop->popSize];
    for (i = 0; i < pop->popSize; ++i) {
        idxs[i] = i;
    }
    grstate_t _thread_grstate;
    grstate_shift(&_thread_grstate, grstate, g_mpiProcId);

    // przeprowadź size turniejów
    for (s = 0; s < size; ++s) {
        // ile zostało możliwych indeksów do wyboru
        idx_t last = pop->popSize - 1;

        idx_t t, rnd, rndidx;
        //idx_t bestIdx = 0;

        real_t currFit;
        //real_t bestFit = -1;

        // sprawdzaj kolejnych uczesników
        for (t = 0; t < toursize; ++t) {
            rnd = RANDOM_FROM(_thread_grstate.xsubi, 0, last);
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
    grstate_copy(grstate, &_thread_grstate);
    return size;
}

void pop_generate(
        popul_t *newPop,
        const popul_t *oldPop,
        idx_t *selection,
        idx_t nsel)
{
    idx_t i;
    for (i = 0; i < nsel; ++i) {
        indiv_copy(&(newPop->indivs[i]), &(oldPop->indivs[selection[i]]));
    }
    newPop->popSize = oldPop->popSize; // population size doesn't change in new population
}

void pop_cross(
        grstate_t *grstate,
        popul_t *pop)
{
    idx_t i, ncross = 0;
    idx_t icross[pop->popSize];
    grstate_t _thread_grstate;
    grstate_shift(&_thread_grstate, grstate, g_mpiProcId);
    for (i = 0; i < pop->popSize; ++i) {
        if (RANDOM_0_TO_1(_thread_grstate.xsubi) <= g_pCross) {
            icross[ncross++] = i;
        }
    }
    grstate_copy(grstate, &_thread_grstate);
    shuffle_array(grstate, icross, ncross);

    ncross &= (idx_t) (~1);
    for (i = 0; i < ncross; i+=2) {
        indiv_xcross(grstate, &(pop->indivs[icross[i]]), &(pop->indivs[icross[i+1]]));
    }
}

void pop_mut(
        grstate_t *grstate,
        popul_t *pop)
{
    idx_t i;
    grstate_t _thread_grstate;
    grstate_shift(&_thread_grstate, grstate, g_mpiProcId);
    for (i = 0; i < pop->popSize; ++i) {
        indiv_mut(&_thread_grstate, &(pop->indivs[i]));
    }
    grstate_copy(grstate, &_thread_grstate);
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
              "[%d] Population:\n %s = %d\n %s = %d\n %s = %f\n\t",
              g_mpiProcId,
              "size         ", pop->popSize,
              "generation   ", pop->genIdx,
              "total fitness", pop->fitSum
                )
          ) <= 0) {
        MYERR_ERR(-3, "Blad w population -> string");
    }

    strcat(str, chromStr);
    free(chromStr);
    idx_t i;
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

    for (i = 0; i < pop->popSize; ++i) {
        tmpFS += pop->indivs[i].fitness;
        tmpOF += reval_funct(pop->indivs[i].fitness);

        chrom_to_real(pop->indivs[i].genotype, tmpRealChrom);
        for (j = 0; j < g_dim; ++j) {
            tmpEval[j] += pop->indivs[i].genotype[j];
            tmpRes[j] += tmpRealChrom[j];
        }
    }

    popstat->fitMean = tmpFS / pop->popSize;
    popstat->objFuncMean = tmpOF / pop->popSize;

    for (i = 0; i < g_dim; ++i) {
        popstat->evalResultMean[i] = tmpEval[i] / pop->popSize;
        popstat->objResultsMean[i] = tmpRes[i] / pop->popSize;
    }

    /* VARS */
    tmpFS = 0, tmpOF = 0;
    memset(tmpRes, 0, g_dim * sizeof(real_t));
    memset(tmpEval, 0, g_dim * sizeof(real_t));

    for (i = 0; i < pop->popSize; ++i) {
        tmpFS += pow(pop->indivs[i].fitness - popstat->fitMean, 2.0);
        tmpOF += pow(reval_funct(pop->indivs[i].fitness) - popstat->objFuncMean, 2.0);

        chrom_to_real(pop->indivs[i].genotype, tmpRealChrom);
        for (j = 0; j < g_dim; ++j) {
            tmpEval[j] += pow(pop->indivs[i].genotype[j] - popstat->evalResultMean[j], 2.0);
            tmpRes[j] += pow(tmpRealChrom[j] - popstat->objResultsMean[j], 2.0);
        }
    }

    popstat->fitSD = sqrt(tmpFS / pop->popSize);
    popstat->objFuncSD = sqrt(tmpOF / pop->popSize);

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
