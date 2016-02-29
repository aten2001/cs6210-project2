#include <stdio.h>
#include "omp_dissemination_barrier.h"


void omp_dissemination_barrier_init(omp_dissemination_barrier_t* barrier, int num_threads) {
  barrier->N = num_threads;
  barrier->num_rounds = log2ceil(num_threads);
  barrier->threads = (dissemination_barrier_thread_data_t*) malloc(
          sizeof(dissemination_barrier_thread_data_t) * num_threads);

  if (barrier->threads == NULL) {
    perror("malloc failed");
    exit(EXIT_FAILURE);
  }

  // Initialize thread data
  for (int i = 0; i < barrier->N; i++) {
    dissemination_barrier_thread_data_t* thread_data = &barrier->threads[i];
    thread_data->parity = 0;
    thread_data->sense = true;
    thread_data->flags.myflags[0] = (volatile int32_t*) malloc(sizeof(int32_t) * barrier->num_rounds);
    thread_data->flags.myflags[1] = (volatile int32_t*) malloc(sizeof(int32_t) * barrier->num_rounds);
    thread_data->flags.partnerflags[0] = (volatile int32_t**) malloc(sizeof(int32_t*) * barrier->num_rounds);
    thread_data->flags.partnerflags[1] = (volatile int32_t**) malloc(sizeof(int32_t*) * barrier->num_rounds);

    if ((thread_data->flags.myflags[0] == NULL) ||
        (thread_data->flags.myflags[1] == NULL) ||
        (thread_data->flags.partnerflags[0] == NULL) ||
        (thread_data->flags.partnerflags[1] == NULL)) {
      perror("malloc failed");
      exit(EXIT_FAILURE);
    }
  }

  for (int i = 0; i < barrier->N; i++) {
    dissemination_barrier_thread_data_t* thread_data = &barrier->threads[i];
    for (int j = 0; j < barrier->N; j++) {
      thread_data->flags.myflags[0][j] = false;
      thread_data->flags.myflags[1][j] = false;

      int partner = (i + (1 << j)) % barrier->N;
      dissemination_barrier_thread_data_t* partner_thread = &barrier->threads[partner];
      thread_data->flags.partnerflags[0][j] = &partner_thread->flags.myflags[0][j];
    }
  }
}

void omp_dissemination_barrier_destroy(omp_dissemination_barrier_t* barrier) {
  for (int i = 0; i < barrier->N; i++) {
    dissemination_barrier_thread_data_t* thread_data = &barrier->threads[i];
    free(thread_data->flags.myflags[0]);
    free(thread_data->flags.myflags[1]);
    free(thread_data->flags.partnerflags[0]);
    free(thread_data->flags.partnerflags[1]);
  }
  free(barrier->threads);
}

void omp_dissemination_barrier(omp_dissemination_barrier_t* barrier) {
  int32_t id = omp_get_thread_num();
  dissemination_barrier_thread_data_t* thread_data = &barrier->threads[id];

  for (int i = 0; i < barrier->num_rounds; i++) {
    *(thread_data->flags.partnerflags[thread_data->parity][i]) = thread_data->sense;
    while (thread_data->flags.myflags[thread_data->parity][i] != thread_data->sense);
  }
  if (thread_data->parity) {
    thread_data->sense = !thread_data->sense;
  }
  thread_data->parity = 1 - thread_data->parity;
}