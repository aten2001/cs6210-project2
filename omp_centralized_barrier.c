#include <stdio.h>
#include "omp_centralized_barrier.h"

void omp_centralized_barrier_init(omp_centralized_barrier_t *barrier, int num_threads) {
  barrier->count = barrier->N = num_threads;
  barrier->sense = true;
}

void omp_centralized_barrier_destroy(omp_centralized_barrier_t *barrier) {

}

void omp_centralized_barrier(omp_centralized_barrier_t *barrier) {
  int local_sense = !barrier->sense;
  if (__sync_fetch_and_sub(&barrier->count, 1) == 1) {
    barrier->count = barrier->N;
    barrier->sense = local_sense;
  } else {
    while (barrier->sense != local_sense);
  }
}

void omp_centralized_barrier2_init(omp_centralized_barrier2_t *barrier, int num_threads) {
  barrier->count = barrier->N = num_threads;
  barrier->sense = true;

  posix_memalign((void**)&barrier->threads, 64, sizeof(centralized_barrier_thread_data_t) * barrier->N);

  if (barrier->threads == NULL) {
    perror("malloc failed");
    exit(EXIT_FAILURE);
  }

#ifdef CACHE_PADDING
  assert((barrier->threads % 64) == 0)
  assert(sizeof(centralized_barrier_thread_data_t) == 64);
#endif
  for (int i = 0; i < barrier->N; i++) {
    barrier->threads[i].sense = true;
  }
}

void omp_centralized_barrier2_destroy(omp_centralized_barrier2_t *barrier) {
  free(barrier->threads);
}

void omp_centralized_barrier2(omp_centralized_barrier2_t *barrier) {
  int* local_sense = &(barrier->threads[omp_get_thread_num()].sense);
  *local_sense = !*local_sense;
  if (__sync_fetch_and_sub(&barrier->count, 1) == 1) {
    barrier->count = barrier->N;
    barrier->sense = *local_sense;
  } else {
    while (barrier->sense != *local_sense);
  }
}
