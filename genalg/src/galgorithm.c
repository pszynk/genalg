#include "galgorithm.h"

void print_gen_info(
        idx_t gen,
        real_t fitSum,
        real_t bestFitAll,
        real_t bestFitCurr);

real_t galgorithm(grstate_t *grstate)
{
    idx_t gen, nsel, i, send_process_id, receive_process_id;
    real_t bestFval, f = 0, val;
    idx_t selection[g_popSize];
    indiv_t *bestOfAll, *bestOfPop, *tmpIndiv;
    popul_t *oldPop, *newPop, *tmpPop;

    bestOfAll = indiv_create(1);
    bestOfPop = indiv_create(1);
    oldPop = pop_create(g_popSize);
    newPop = pop_create(g_popSize);
    pop_rand(grstate, oldPop);
    pop_eval(bestOfAll, oldPop);

    bestFval = indiv_eval(bestOfAll);
    for (gen = 1; gen <= g_maxGen; ++gen) {
        val = g_revalFunct(bestFval);
        /* jezeli jestesmy blisko minimum, przerwij algorytm */
        if ((val >= -EPSILON) && (val <= EPSILON)) {
            break;
        }
        /* selection */
        nsel = g_selFunct(grstate, oldPop, selection, oldPop->popSize);
        /* generate new pop */
        pop_generate(newPop, oldPop, selection, nsel);
        newPop->genIdx=gen;
        /* crossover */
        pop_cross(grstate, newPop);
        /* mutation */
        pop_mut(grstate, newPop);
        /* mix populations with other processes */
        if (g_mpiVer == MPI3 && gen == g_maxGen/2) {
            // pass it to the right...
            if (g_mpiProcId == g_mpiNumProcs - 1) {
                send_process_id = 0;
            } else {
                send_process_id = g_mpiProcId + 1;
            }
            if (g_mpiProcId == 0) {
                receive_process_id = g_mpiNumProcs - 1;
            } else {
                receive_process_id = g_mpiProcId - 1;
            }
            
            if (g_VERBOSELVL > 0) {
                printf("[%d] send: %d  | receive %d\n", g_mpiProcId, send_process_id, receive_process_id);
            
            // send some of your population to the next process
                printf("[%d] BEFORE:\n", g_mpiProcId);
            }
            for(i = 0; i < g_popSize/10; ++i) { 
                if (g_VERBOSELVL > 0) {
                    printf("[%d] %x   %f\n", g_mpiProcId, *(newPop->indivs[i].genotype), newPop->indivs[i].fitness);
                }
                MPI_Send(&*(newPop->indivs[i].genotype), sizeof(chrom_t), MPI_BYTE, send_process_id, 9998, MPI_COMM_WORLD);
                MPI_Send(&newPop->indivs[i].fitness, sizeof(real_t), MPI_BYTE, send_process_id, 9999, MPI_COMM_WORLD);
            }
            
            if (g_VERBOSELVL > 0) {
                printf("[%d] AFTER:\n", g_mpiProcId);
            }
            // receive and store some of other processes population
            for(i = 0; i < g_popSize/10; ++i) {
                MPI_Recv(&*(newPop->indivs[i].genotype), sizeof(chrom_t), MPI_BYTE, receive_process_id, 9998, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Recv(&newPop->indivs[i].fitness, sizeof(real_t), MPI_BYTE, receive_process_id, 9999, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                if (g_VERBOSELVL > 0) {
                    printf("[%d] %x   %f\n", g_mpiProcId, *(newPop->indivs[i].genotype), newPop->indivs[i].fitness);
                }
            }
        }
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

    return bestFval;
}

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


