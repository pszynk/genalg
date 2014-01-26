#include "galgorithm.h"

void print_gen_info(
        idx_t gen,
        real_t fitSum,
        real_t bestFitAll,
        real_t bestFitCurr);

real_t galgorithm(grstate_t *grstate)
{
    idx_t gen, nsel, i;
    real_t bestFval, f = 0;
    idx_t selection[g_popSize];
    indiv_t *bestOfAll, *bestOfPop, *tmpIndiv;
    popul_t *oldPop, *newPop, *tmpPop;

    /* MPI begin */
    unsigned int _mpi_sendID,
                 _mpi_recID;

    // pass it to the right...
    _mpi_sendID = (g_mpiProcId + 1) % g_mpiNumProcs;
    if (g_mpiProcId == 0) {
        _mpi_recID = g_mpiNumProcs - 1;
    } else {
        _mpi_recID = g_mpiProcId - 1;
    }

    chrom_t _mpi_recGen;

    idx_t _mpi_outSelection[g_mpi3MigSize],
          _mpi_inSelection[g_mpi3MigSize];

    if (g_mpiVer == VMPI2) {
        _mpi_recGen = chrom_create();
    }
    /* MPI end */


    bestOfAll = indiv_create(1);
    bestOfPop = indiv_create(1);
    oldPop = pop_create(g_popSize);
    newPop = pop_create(g_popSize);
    pop_rand(grstate, oldPop);
    pop_eval(bestOfAll, oldPop);

    bestFval = indiv_eval(bestOfAll);

    for (gen = 1; gen <= g_maxGen; ++gen) {

        /* selection */
        nsel = g_selFunct(grstate, oldPop, selection, oldPop->popSize);

        /* generate new pop */
        pop_generate(newPop, oldPop, selection, nsel);
        newPop->genIdx=gen;

        /* mix populations with other processes */
        if (g_mpiVer == VMPI2
                && !(gen % g_mpi3Interval)) {


            if (g_VERBOSELVL > 0) {
                printf("[%d] send: %d  | receive %d\n", g_mpiProcId,
                        _mpi_sendID, _mpi_recID);
            }

            pop_find_best_or_worst(g_mpi3MigSize, 1, newPop, _mpi_outSelection, g_mpi3MigSize);
            pop_find_best_or_worst(g_mpi3MigSize, 0, newPop, _mpi_inSelection, g_mpi3MigSize);

            for (i = 0; i < g_mpi3MigSize; ++i) {
                if (g_mpiProcId % 2) {
                    // send some of your population to the next process
                    MPI_Send((void*)newPop->indivs[_mpi_outSelection[i]].genotype,
                            g_bitPerChrom/BIT_PER_BYTE, MPI_BYTE, _mpi_sendID,
                            1, MPI_COMM_WORLD);

                    // receive and store some of other processes populations better individuals
                    MPI_Recv((void*)_mpi_recGen, g_bitPerChrom/BIT_PER_BYTE,
                            MPI_BYTE, _mpi_recID, 1,
                            MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                    chrom_copy(newPop->indivs[_mpi_inSelection[i]].genotype, _mpi_recGen);

                } else {
                    // receive and store some of other processes populations better individuals
                    MPI_Recv((void*)_mpi_recGen, g_bitPerChrom/BIT_PER_BYTE,
                            MPI_BYTE, _mpi_recID, 1,
                            MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                    chrom_copy(newPop->indivs[_mpi_inSelection[i]].genotype, _mpi_recGen);

                    // send some of your population to the next process
                    MPI_Send((void*)newPop->indivs[_mpi_outSelection[i]].genotype,
                            g_bitPerChrom/BIT_PER_BYTE, MPI_BYTE, _mpi_sendID,
                            1, MPI_COMM_WORLD);
                }
            }
        }

        /* crossover */
        pop_cross(grstate, newPop);
        /* mutation */
        pop_mut(grstate, newPop);
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

    indiv_destory(bestOfAll);
    free(bestOfAll);
    indiv_destory(bestOfPop);
    free(bestOfPop);
    pop_destroy(oldPop);
    pop_destroy(newPop);

    /* MPI */
    if (g_mpiVer == VMPI2) {
        chrom_destroy(_mpi_recGen);
    }

    return bestFval;
}

void print_gen_info(
        idx_t gen,
        real_t fitSum,
        real_t bestFitAll,
        real_t bestFitCurr)
{
    printf("[%d] generacja: %d\n", g_mpiProcId, gen);
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


