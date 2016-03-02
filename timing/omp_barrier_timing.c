#include <omp.h>
#include <stdio.h>
#include "../omp_barriers.h"


void centralized_timing(int num_threads) {
  omp_set_num_threads(num_threads);

  omp_centralized_barrier_t barrier;
  omp_centralized_barrier_init(&barrier, num_threads);

#pragma omp parallel
  {
    for (int i = 0; i < 100; i++) {
      omp_centralized_barrier(&barrier);
    }
  }
  omp_centralized_barrier_destroy(&barrier);
}

void centralized2_timing(int num_threads) {
  omp_set_num_threads(num_threads);

  omp_centralized_barrier2_t barrier;
  omp_centralized_barrier2_init(&barrier, num_threads);

#pragma omp parallel
  {
    for (int i = 0; i < 100; i++) {
      omp_centralized_barrier2(&barrier);
    }
  }
  omp_centralized_barrier2_destroy(&barrier);
}

void tree_timing(int num_threads) {
  omp_set_num_threads(num_threads);

  omp_tree_barrier_t barrier;
  omp_tree_barrier_init(&barrier, num_threads);

#pragma omp parallel
  {
    for (int i = 0; i < 100; i++) {
      omp_tree_barrier(&barrier);
    }
  }
  omp_tree_barrier_destroy(&barrier);
}

void tournament_timing(int num_threads) {
  omp_set_num_threads(num_threads);

  omp_tournament_barrier_t barrier;
  omp_tournament_barrier_init(&barrier, num_threads);

#pragma omp parallel
  {
    for (int i = 0; i < 100; i++) {
      omp_tournament_barrier(&barrier);
    }
  }
  omp_tournament_barrier_destroy(&barrier);
}

void dissemination_timing(int num_threads) {
  omp_set_num_threads(num_threads);

  omp_dissemination_barrier_t barrier;
  omp_dissemination_barrier_init(&barrier, num_threads);

#pragma omp parallel
  {
    for (int i = 0; i < 100; i++) {
      omp_dissemination_barrier(&barrier);
    }
  }
  omp_dissemination_barrier_destroy(&barrier);
}

void mcs_timing(int num_threads) {
  omp_set_num_threads(num_threads);

  omp_mcs_barrier_t barrier;
  omp_mcs_barrier_init(&barrier, num_threads);

#pragma omp parallel
  {
    for (int i = 0; i < 100; i++) {
      omp_mcs_barrier(&barrier);
    }
  }
  omp_mcs_barrier_destroy(&barrier);
}

void omp_timing(int num_threads) {
  omp_set_num_threads(num_threads);

  omp_mcs_barrier_t barrier;
  omp_mcs_barrier_init(&barrier, num_threads);

#pragma omp parallel
  {
    for (int i = 0; i < 100; i++) {
#pragma omp barrier
    }
  }
  omp_mcs_barrier_destroy(&barrier);
}

int main(int argc, char** argv) {
  if (argc != 3) {
    printf(stderr, "Invalid number of arguments. Usage: %s <barrier-type> <num-threads>", argv[0]);
  }

  int barrier_type = atoi(argv[1]);
  int num_threads = atoi(argv[2]);

  switch (barrier_type) {
    case 0:
      centralized_timing(num_threads);
      break;
    case 1:
      centralized2_timing(num_threads);
      break;
    case 2:
      tree_timing(num_threads);
      break;
    case 3:
      tournament_timing(num_threads);
      break;
    case 4:
      dissemination_timing(num_threads);
      break;
    case 5:
      mcs_timing(num_threads);
      break;
    case 6:
      omp_timing(num_threads);
    default:
      printf("Invalid Barrier Type\n");
      return -1;
  }
}