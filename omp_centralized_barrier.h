#ifndef PROJECT2_OMP_CENTRALIZED_BARRIER_H
#define PROJECT2_OMP_CENTRALIZED_BARRIER_H

#include <stdlib.h>
#include <stdint.h>
#include "omp.h"

#define true 1
#define false 0
#define CENTRALIZED_BARRIER_THREAD_DATA_PADDING 60

typedef struct omp_centralized_barrier {
  int32_t N;
  volatile int32_t count;
  volatile int32_t sense;
} omp_centralized_barrier_t;

void omp_centralized_barrier_init(omp_centralized_barrier_t* barrier, int num_threads);

void omp_centralized_barrier_destroy(omp_centralized_barrier_t* barrier);

void omp_centralized_barrier(omp_centralized_barrier_t* barrier);


typedef struct centralized_barrier_thread_data {
  int32_t sense;
#ifdef CACHE_PADDING
  int8_t padding[CENTRALIZED_BARRIER_THREAD_DATA_PADDING];
#endif
} centralized_barrier_thread_data_t;

typedef struct omp_centralized_barrier2 {
  int32_t N;
  volatile int32_t count;
  volatile int32_t sense;
  centralized_barrier_thread_data_t* threads;
} omp_centralized_barrier2_t;


void omp_centralized_barrier2_init(omp_centralized_barrier2_t* barrier, int num_threads);

void omp_centralized_barrier2_destroy(omp_centralized_barrier2_t* barrier);

void omp_centralized_barrier2(omp_centralized_barrier2_t* barrier);


#endif //PROJECT2_OMP_CENTRALIZED_BARRIER_H
