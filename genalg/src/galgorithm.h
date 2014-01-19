#ifndef GALGORITHM_H
#define GALGORITHM_H

/*
 * Plik z algorytmem genetycznym
 */
#include "types.h"
#include "population.h"
#include "individual.h"
#include <mpi.h>
#include "chromosome.h"

#define MAX_GENERATIONS (100 * 1000)

/*
 * Funkcja - algorytm genetyczny
 * argumenty -> paczka parametrow (ile generacji)
 * populacja startowa
 * paczka wynikow
 *
 */
real_t galgorithm(grstate_t *grstate);



#endif /* end of include guard: GALGORITHM_H */
