#ifndef GALGORITHM_H
#define GALGORITHM_H

/*
 * Plik z algorytmem genetycznym
 */
#include "types.h"
#include "population.h"
#include "individual.h"

#define MAX_GENERATIONS (100 * 1000)

//typedef struct gaparams_t {
    //idx_t MAX_POP_SIZE;
    //idx_t MAX_GENERS;
    //real_t PMUT;
    //real_t PCROSS;
//} gaparams_t;

typedef struct stats_t {

} stats_t;

/*
 * Funkcja - algorytm genetyczny
 * argumenty -> paczka parametrow (ile generacji)
 * populacja startowa
 * paczka wynikow
 *
 */
real_t galgorithm(stats_t *stats);



#endif /* end of include guard: GALGORITHM_H */
