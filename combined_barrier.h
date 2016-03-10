#ifndef PROJECT2_COMBINED_BARRIER_H
#define PROJECT2_COMBINED_BARRIER_H

#include "mpi.h"

typedef struct combined_barrier {

} combined_barrier_t;

void combined_barrier_init(combined_barrier_t* barrier, int num_threads);

void combined_barrier_destroy(combined_barrier_t* barrier);

void combined_barrier(combined_barrier_t* barrier, MPI_Comm comm, int tag);

#endif //PROJECT2_COMBINED_BARRIER_H
