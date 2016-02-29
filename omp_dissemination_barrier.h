//
// Created by manatunga on 2/29/16.
//

#ifndef PROJECT2_OMP_DISSEMINATION_BARRIER_H
#define PROJECT2_OMP_DISSEMINATION_BARRIER_H

#include <stdlib.h>
#include "omp.h"
#include "helpers.h"

#define true 1
#define false 0

typedef struct flags {
  volatile int32_t* myflags[2];
  volatile int32_t** partnerflags[2];
} flags_t;


typedef struct dissemination_barrier_thread_data {
  int32_t sense;
  int32_t parity;
  flags_t flags;
} dissemination_barrier_thread_data_t;

typedef struct omp_dissemination_barrier {
  int N;
  int num_rounds;
  dissemination_barrier_thread_data_t* threads;
} omp_dissemination_barrier_t;

void omp_dissemination_barrier_init(omp_dissemination_barrier_t* barrier, int num_threads);

void omp_dissemination_barrier_destroy(omp_dissemination_barrier_t* barrier);

void omp_dissemination_barrier(omp_dissemination_barrier_t* barrier);

#endif //PROJECT2_OMP_DISSEMINATION_BARRIER_H
