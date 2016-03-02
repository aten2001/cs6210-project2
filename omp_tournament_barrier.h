//
// Created by manatunga on 2/29/16.
//

#ifndef PROJECT2_OMP_TOURNAMENT_BARRIER_H
#define PROJECT2_OMP_TOURNAMENT_BARRIER_H

#include <stdlib.h>
#include <stdint.h>
#include "omp.h"
#include "helpers.h"

#define true 1
#define false 0


typedef enum {
  UNKNOWN, WINNER, LOSER, BYE, CHAMPION, DROPOUT
} role_t;

typedef struct round {
  role_t role;
  volatile int32_t* opponent;
  volatile int32_t flag;
} round_t;

typedef struct tournament_barrier_thread_data {
  int32_t sense;
  round_t* rounds;
} tournament_barrier_thread_data_t;

typedef struct omp_tournament_barrier {
  int32_t N;
  int32_t num_rounds;
  tournament_barrier_thread_data_t* threads;
} omp_tournament_barrier_t;

void omp_tournament_barrier_init(omp_tournament_barrier_t* barrier, int num_threads);

void omp_tournament_barrier_destroy(omp_tournament_barrier_t* barrier);

void omp_tournament_barrier(omp_tournament_barrier_t* barrier);

#endif //PROJECT2_OMP_TOURNAMENT_BARRIER_H
