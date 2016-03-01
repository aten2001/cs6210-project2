#include <omp.h>
#include <stdio.h>
#include "../omp_barriers.h"

void centralized_test(int num_threads, int num_iters) {
  omp_set_num_threads(num_threads);

  omp_centralized_barrier_t barrier;
  omp_centralized_barrier_init(&barrier, num_threads);

#pragma omp parallel
  {
    int N = omp_get_num_threads();
    int thread_num = omp_get_thread_num();
    for (int i = 0; i < num_iters; i++) {
      printf("Process %d/%d: Iteration %d\n", thread_num, N, i);
      omp_centralized_barrier(&barrier);
    }
  }

  omp_centralized_barrier_destroy(&barrier);
}

void centralized2_test(int num_threads, int num_iters) {
  omp_set_num_threads(num_threads);

  omp_centralized_barrier2_t barrier;
  omp_centralized_barrier2_init(&barrier, num_threads);

#pragma omp parallel
  {
    int N = omp_get_num_threads();
    int thread_num = omp_get_thread_num();
    for (int i = 0; i < num_iters; i++) {
      printf("Process %d/%d: Iteration %d\n", thread_num, N, i);
      omp_centralized_barrier2(&barrier);
    }
  }

  omp_centralized_barrier2_destroy(&barrier);
}

void tree_test(int num_threads, int num_iters) {
  omp_set_num_threads(num_threads);

  omp_tree_barrier_t barrier;
  omp_tree_barrier_init(&barrier, num_threads);

#pragma omp parallel
  {
    int N = omp_get_num_threads();
    int thread_num = omp_get_thread_num();
    for (int i = 0; i < num_iters; i++) {
      printf("Process %d/%d: Iteration %d\n", thread_num, N, i);
      omp_tree_barrier(&barrier);
    }
  }

  omp_tree_barrier_destroy(&barrier);
}

void tournament_test(int num_threads, int num_iters) {
  omp_set_num_threads(num_threads);

  omp_tournament_barrier_t barrier;
  omp_tournament_barrier_init(&barrier, num_threads);

#pragma omp parallel
  {
    int N = omp_get_num_threads();
    int thread_num = omp_get_thread_num();
    for (int i = 0; i < num_iters; i++) {
      printf("Process %d/%d: Iteration %d\n", thread_num, N, i);
      omp_tournament_barrier(&barrier);
    }
  }

  omp_tournament_barrier_destroy(&barrier);
}

void dissemination_test(int num_threads, int num_iters) {
  omp_set_num_threads(num_threads);

  omp_dissemination_barrier_t barrier;
  omp_dissemination_barrier_init(&barrier, num_threads);

#pragma omp parallel
  {
    int N = omp_get_num_threads();
    int thread_num = omp_get_thread_num();
    for (int i = 0; i < num_iters; i++) {
      printf("Process %d/%d: Iteration %d\n", thread_num, N, i);
      omp_dissemination_barrier(&barrier);
    }
  }

  omp_dissemination_barrier_destroy(&barrier);
}

void mcs_test(int num_threads, int num_iters) {
  omp_set_num_threads(num_threads);

  omp_mcs_barrier_t barrier;
  omp_mcs_barrier_init(&barrier, num_threads);

#pragma omp parallel
  {
    int N = omp_get_num_threads();
    int thread_num = omp_get_thread_num();
    for (int i = 0; i < num_iters; i++) {
      printf("Process %d/%d: Iteration %d\n", thread_num, N, i);
      omp_mcs_barrier(&barrier);
    }
  }

  omp_mcs_barrier_destroy(&barrier);
}

void omp_test(int num_threads, int num_iters) {
  omp_set_num_threads(num_threads);

#pragma omp parallel
  {
    int N = omp_get_num_threads();
    int thread_num = omp_get_thread_num();
    for (int i = 0; i < num_iters; i++) {
      printf("Process %d/%d: Iteration %d\n", thread_num, N, i);
#pragma omp barrier
    }
  }

}

int main(int argc, char** argv) {
  int barrier_type, num_threads, num_iters;
  if (argc != 4) {
    printf("Usage: ./%s <barrier-type> <num-threads> <num-iters>", argv[0]);
  }

  barrier_type = atoi(argv[1]);
  num_threads = atoi(argv[2]);
  num_iters = atoi(argv[3]);

  switch (barrier_type) {
    case 0:
      centralized_test(num_threads, num_iters);
      break;
    case 1:
      centralized2_test(num_threads, num_iters);
      break;
    case 2:
      tree_test(num_threads, num_iters);
      break;
    case 3:
      tournament_test(num_threads, num_iters);
      break;
    case 4:
      dissemination_test(num_threads, num_iters);
      break;
    case 5:
      mcs_test(num_threads, num_iters);
      break;
    case 6:
      omp_test(num_threads, num_iters);
    default:
      printf("Invalid Barrier Type\n");
      return -1;
  }
  return 0;
}



