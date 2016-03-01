#include <omp.h>
#include <stdio.h>
#include "../omp_barriers.h"

void centralized_test(int num_threads, int skip_thread) {
  omp_set_num_threads(num_threads);

  omp_centralized_barrier_t barrier;
  omp_centralized_barrier_init(&barrier, num_threads);

#pragma omp parallel
  {
    int N = omp_get_num_threads();
    int thread_num = omp_get_thread_num();
    printf("Process %d/%d: Before Barrier\n", thread_num, N);
    if (thread_num != skip_thread) {
      omp_centralized_barrier(&barrier);
    }
    printf("Process %d/%d: After Barrier\n", thread_num, N);

  }

  omp_centralized_barrier_destroy(&barrier);
}

void centralized2_test(int num_threads, int skip_thread) {
  omp_set_num_threads(num_threads);

  omp_centralized_barrier2_t barrier;
  omp_centralized_barrier2_init(&barrier, num_threads);

#pragma omp parallel
  {
    int N = omp_get_num_threads();
    int thread_num = omp_get_thread_num();
    printf("Process %d/%d: Before Barrier\n", thread_num, N);
    if (thread_num != skip_thread) {
      omp_centralized_barrier2(&barrier);
    }
    printf("Process %d/%d: After Barrier\n", thread_num, N);
  }

  omp_centralized_barrier2_destroy(&barrier);
}

void tree_test(int num_threads, int skip_thread) {
  omp_set_num_threads(num_threads);

  omp_tree_barrier_t barrier;
  omp_tree_barrier_init(&barrier, num_threads);

#pragma omp parallel
  {
    int N = omp_get_num_threads();
    int thread_num = omp_get_thread_num();
    printf("Process %d/%d: Before Barrier\n", thread_num, N);
    if (thread_num != skip_thread) {
      omp_tree_barrier(&barrier);
    }
    printf("Process %d/%d: After Barrier\n", thread_num, N);

  }

  omp_tree_barrier_destroy(&barrier);
}

void tournament_test(int num_threads, int skip_thread) {
  omp_set_num_threads(num_threads);

  omp_tournament_barrier_t barrier;
  omp_tournament_barrier_init(&barrier, num_threads);

#pragma omp parallel
  {
    int N = omp_get_num_threads();
    int thread_num = omp_get_thread_num();
    printf("Process %d/%d: Before Barrier\n", thread_num, N);
    if (thread_num != skip_thread) {
      omp_tournament_barrier(&barrier);
    }
    printf("Process %d/%d: After Barrier\n", thread_num, N);

  }

  omp_tournament_barrier_destroy(&barrier);
}

void dissemination_test(int num_threads, int skip_thread) {
  omp_set_num_threads(num_threads);

  omp_dissemination_barrier_t barrier;
  omp_dissemination_barrier_init(&barrier, num_threads);

#pragma omp parallel
  {
    int N = omp_get_num_threads();
    int thread_num = omp_get_thread_num();
    printf("Process %d/%d: Before Barrier\n", thread_num, N);
    if (thread_num != skip_thread) {
      omp_dissemination_barrier(&barrier);
    }
    printf("Process %d/%d: After Barrier\n", thread_num, N);

  }

  omp_dissemination_barrier_destroy(&barrier);
}

void mcs_test(int num_threads, int skip_thread) {
  omp_set_num_threads(num_threads);

  omp_mcs_barrier_t barrier;
  omp_mcs_barrier_init(&barrier, num_threads);

#pragma omp parallel
  {
    int N = omp_get_num_threads();
    int thread_num = omp_get_thread_num();
    printf("Process %d/%d: Before Barrier\n", thread_num, N);
    if (thread_num != skip_thread) {
      omp_mcs_barrier(&barrier);
    }
    printf("Process %d/%d: After Barrier\n", thread_num, N);

  }

  omp_mcs_barrier_destroy(&barrier);
}

void omp_test(int num_threads, int skip_thread) {
  omp_set_num_threads(num_threads);

#pragma omp parallel
  {
    int N = omp_get_num_threads();
    int thread_num = omp_get_thread_num();
    printf("Process %d/%d: Before Barrier\n", thread_num, N);
    if (thread_num != skip_thread) {
#pragma omp barrier
    }
    printf("Process %d/%d: After Barrier\n", thread_num, N);

  }

}

int main(int argc, char** argv) {
  if (argc != 4) {
    printf("Usage: ./%s <barrier-type> <num-threads> <num-iters>", argv[0]);
  }

  int barrier_type, num_threads, skip_thread;
  barrier_type = atoi(argv[1]);
  num_threads = atoi(argv[2]);
  skip_thread = atoi(argv[3]);

  switch (barrier_type) {
    case 0:
      centralized_test(num_threads, skip_thread);
      break;
    case 1:
      centralized2_test(num_threads, skip_thread);
      break;
    case 2:
      tree_test(num_threads, skip_thread);
      break;
    case 3:
      tournament_test(num_threads, skip_thread);
      break;
    case 4:
      dissemination_test(num_threads, skip_thread);
      break;
    case 5:
      mcs_test(num_threads, skip_thread);
      break;
    case 6:
      omp_test(num_threads, skip_thread);
    default:
      printf("Invalid Barrier Type\n");
      return -1;
  }
  return 0;
}



