#include <omp.h>
#include <stdio.h>
#include "../omp_barriers.h"

void centralized_test(int num_threads, int num_iters) {
  omp_set_num_threads(num_threads);

  omp_centralized_barrier_t barrier;
  omp_centralized_barrier_init(&barrier, num_threads);

  printf("=====Test Start: Centralized Barrier 1 Threads: %d Iters: %d=====\n", num_threads, num_iters);
#pragma omp parallel
  {
    int N = omp_get_num_threads();
    int thread_num = omp_get_thread_num();
    for (int i = 1; i <= num_iters; i++) {
      printf("Process %d/%d: Iteration %d\n", thread_num, N, i);
      omp_centralized_barrier(&barrier);
    }
  }

  printf("=====Test End: Centralized Barrier 1====\n");
  omp_centralized_barrier_destroy(&barrier);
}

void centralized2_test(int num_threads, int num_iters) {
  omp_set_num_threads(num_threads);

  omp_centralized_barrier2_t barrier;
  omp_centralized_barrier2_init(&barrier, num_threads);

  printf("=====Test Start: Centralized Barrier 2 Threads: %d Iters: %d=====\n", num_threads, num_iters);
#pragma omp parallel
  {
    int N = omp_get_num_threads();
    int thread_num = omp_get_thread_num();
    for (int i = 1; i <= num_iters; i++) {
      printf("Process %d/%d: Iteration %d\n", thread_num, N, i);
      omp_centralized_barrier2(&barrier);
    }
  }
  printf("=====Test End: Centralized Barrier 2====\n");

  omp_centralized_barrier2_destroy(&barrier);
}

void tree_test(int num_threads, int num_iters) {
  omp_set_num_threads(num_threads);

  omp_tree_barrier_t barrier;
  omp_tree_barrier_init(&barrier, num_threads);

  printf("=====Test Start: Tree Barrier Threads: %d Iters: %d=====\n", num_threads, num_iters);
#pragma omp parallel
  {
    int N = omp_get_num_threads();
    int thread_num = omp_get_thread_num();
    for (int i = 1; i <= num_iters; i++) {
      printf("Process %d/%d: Iteration %d\n", thread_num, N, i);
      omp_tree_barrier(&barrier);
    }
  }
  printf("=====Test End: Tree Barrier====\n");

  omp_tree_barrier_destroy(&barrier);
}

void tournament_test(int num_threads, int num_iters) {
  omp_set_num_threads(num_threads);

  omp_tournament_barrier_t barrier;
  omp_tournament_barrier_init(&barrier, num_threads);

  printf("=====Test Start: Tournament Barrier Threads: %d Iters: %d=====\n", num_threads, num_iters);
#pragma omp parallel
  {
    int N = omp_get_num_threads();
    int thread_num = omp_get_thread_num();
    for (int i = 1; i <= num_iters; i++) {
      printf("Process %d/%d: Iteration %d\n", thread_num, N, i);
      omp_tournament_barrier(&barrier);
    }
  }
  printf("=====Test End: Tournament Barrier====\n");

  omp_tournament_barrier_destroy(&barrier);
}

void dissemination_test(int num_threads, int num_iters) {
  omp_set_num_threads(num_threads);

  omp_dissemination_barrier_t barrier;
  omp_dissemination_barrier_init(&barrier, num_threads);

  printf("=====Test Start: Dissemination Barrier Threads: %d Iters: %d=====\n", num_threads, num_iters);
#pragma omp parallel
  {
    int N = omp_get_num_threads();
    int thread_num = omp_get_thread_num();
    for (int i = 1; i <= num_iters; i++) {
      printf("Process %d/%d: Iteration %d\n", thread_num, N, i);
      omp_dissemination_barrier(&barrier);
    }
  }
  printf("=====Test End: Dissemination Barrier====\n");

  omp_dissemination_barrier_destroy(&barrier);
}

void mcs_test(int num_threads, int num_iters) {
  omp_set_num_threads(num_threads);

  omp_mcs_barrier_t barrier;
  omp_mcs_barrier_init(&barrier, num_threads);

  printf("=====Test Start: MCS Barrier Threads: %d Iters: %d=====\n", num_threads, num_iters);
#pragma omp parallel
  {
    int N = omp_get_num_threads();
    int thread_num = omp_get_thread_num();
    for (int i = 1; i <= num_iters; i++) {
      printf("Process %d/%d: Iteration %d\n", thread_num, N, i);
      omp_mcs_barrier(&barrier);
    }
  }
  printf("=====Test End: MCS Barrier====\n");

  omp_mcs_barrier_destroy(&barrier);
}

void omp_test(int num_threads, int num_iters) {
  omp_set_num_threads(num_threads);

  printf("=====Test Start: OMP Barrier Threads: %d Iters: %d=====\n", num_threads, num_iters);
#pragma omp parallel
  {
    int N = omp_get_num_threads();
    int thread_num = omp_get_thread_num();
    for (int i = 1; i <= num_iters; i++) {
      printf("Process %d/%d: Iteration %d\n", thread_num, N, i);
#pragma omp barrier
    }
  }
  printf("=====Test End: OMP Barrier====\n");
}

int main(int argc, char** argv) {
  int barrier_type, num_threads, num_iters;
  if (argc != 4) {
    printf("Usage: ./%s <barrier-type> <num-threads> <num-iters>\n", argv[0]);
    return -1;
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
      break;
    default:
      printf("Invalid Barrier Type\n");
      return -1;
  }
  return 0;
}



