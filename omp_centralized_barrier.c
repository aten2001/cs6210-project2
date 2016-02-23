#include <stdlib.h>
#include <stdio.h>
#include "omp.h"

#define true 1
#define false 0

typedef struct omp_centralized_barrier {
  int N;
  int sense;
} omp_centralized_barrier_t;

void omp_centralized_barrier_init(omp_centralized_barrier_t *barrier, int num_threads) {
  barrier->N = num_threads;
  barrier->sense = true;
}

void omp_centralized_barrier_destroy(omp_centralized_barrier_t *barrier) {

}

void omp_centralized_barrier(omp_centralized_barrier_t *barrier) {
  int local_sense = !barrier->sense;
#pragma omp atomic
  barrier->N -= 1;
  if (barrier->N == 0) {
    barrier->N = omp_get_num_threads();
    barrier->sense = local_sense;
  } else {
    while (barrier->sense != local_sense);
  }
}

typedef struct omp_centralized_barrier2 {
  int N;
  int sense;
  int *local_senses;
} omp_centralized_barrier2_t;

void omp_centralized_barrier2_init(omp_centralized_barrier2_t *barrier, int num_threads) {
  barrier->N = num_threads;
  barrier->sense = true;
  barrier->local_senses = (int *) malloc(sizeof(int) * barrier->N);
  if (barrier->local_senses == NULL) {
    perror("malloc failed");
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < barrier->N; i++) {
    barrier->local_senses[i] = true;
  }
}

void omp_centralized_barrier2_destroy(omp_centralized_barrier2_t *barrier) {
  free(barrier->local_senses);
}

void omp_centralized_barrier2(omp_centralized_barrier2_t *barrier) {
  int *local_sense = &barrier->local_senses[omp_get_thread_num()];
  *local_sense = !*local_sense;
#pragma omp atomic
  barrier->N -= 1;
  if (barrier->N == 0) {
    barrier->N = omp_get_num_threads();
    barrier->sense = *local_sense;
  } else {
    while (barrier->sense != *local_sense);
  }
}

typedef struct sense {
  int32_t value;
  int32_t padding[15];
} sense_t;
typedef struct omp_centralized_barrier3 {
  int N;
  int sense;
  sense_t *local_senses;
} omp_centralized_barrier3_t;

void omp_centralized_barrier3_init(omp_centralized_barrier3_t *barrier, int num_threads) {
  barrier->N = num_threads;
  barrier->sense = true;
  barrier->local_senses = (sense_t *) malloc(sizeof(sense_t) * barrier->N);
  if (barrier->local_senses == NULL) {
    perror("malloc failed");
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < barrier->N; i++) {
    barrier->local_senses[i].value = true;
  }
}

void omp_centralized_barrier3_destroy(omp_centralized_barrier3_t *barrier) {
  free(barrier->local_senses);
}

void omp_centralized_barrier3(omp_centralized_barrier3_t *barrier) {
  sense_t *local_sense = &barrier->local_senses[omp_get_thread_num()];
  local_sense->value = !local_sense->value;
#pragma omp atomic
  barrier->N -= 1;
  if (barrier->N == 0) {
    barrier->N = omp_get_num_threads();
    barrier->sense = local_sense->value;
  } else {
    while (barrier->sense != local_sense->value);
  }
}