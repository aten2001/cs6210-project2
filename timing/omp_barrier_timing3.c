#include <omp.h>
#include <stdio.h>
#include "../omp_barriers.h"
#include "timer.h"

#define BLOCK 10000
#define NUM_ITERS 100
#define tries 100

int array[BLOCK];
long results[tries];

struct timeval before, after;

void centralized_timing(int num_threads) {
  omp_centralized_barrier_t barrier;

  omp_centralized_barrier_init(&barrier, num_threads);

#pragma omp parallel
  {
    for (int i = 0; i < NUM_ITERS; i++) {
      omp_centralized_barrier(&barrier);
    }
  }

  for (int j = 0; j < tries; j++) {
    start_watch_usec(&before);
#pragma omp parallel
    {
      for (int i = 0; i < NUM_ITERS; i++) {
        omp_centralized_barrier(&barrier);
      }
    }
    stop_watch_usec(&after);
    results[j] = get_timer_diff_usec(&before, &after);
  }
  omp_centralized_barrier_destroy(&barrier);

  return;
}

void centralized2_timing(int num_threads) {
  omp_centralized_barrier2_t barrier;
  omp_centralized_barrier2_init(&barrier, num_threads);

#pragma omp parallel
  {
    for (int i = 0; i < NUM_ITERS; i++) {
      omp_centralized_barrier2(&barrier);
    }
  }

  start_watch_usec(&before);
  for (int j = 0; j < tries; j++) {
    start_watch_usec(&before);
#pragma omp parallel
    {
      for (int i = 0; i < NUM_ITERS; i++) {
        omp_centralized_barrier2(&barrier);
      }
    }
    stop_watch_usec(&after);
    results[j] = get_timer_diff_usec(&before, &after);
  }

  omp_centralized_barrier2_destroy(&barrier);
}

void tree_timing(int num_threads) {
  omp_tree_barrier_t barrier;
  omp_tree_barrier_init(&barrier, num_threads);

#pragma omp parallel
  {
    for (int i = 0; i < NUM_ITERS; i++) {
      omp_tree_barrier(&barrier);
    }
  }

  for (int j = 0; j < tries; j++) {
    start_watch_usec(&before);
#pragma omp parallel
    {
      for (int i = 0; i < NUM_ITERS; i++) {
        omp_tree_barrier(&barrier);
      }
    }
    stop_watch_usec(&after);
    results[j] = get_timer_diff_usec(&before, &after);
  }

  omp_tree_barrier_destroy(&barrier);
}

void tournament_timing(int num_threads) {
  omp_tournament_barrier_t barrier;
  omp_tournament_barrier_init(&barrier, num_threads);

#pragma omp parallel
  {
    for (int i = 0; i < NUM_ITERS; i++) {
      omp_tournament_barrier(&barrier);
    }
  }

  for (int j = 0; j < tries; j++) {
    start_watch_usec(&before);
#pragma omp parallel
    {
      for (int i = 0; i < NUM_ITERS; i++) {
        omp_tournament_barrier(&barrier);
      }
    }
    stop_watch_usec(&after);
    results[j] = get_timer_diff_usec(&before, &after);
  }

  omp_tournament_barrier_destroy(&barrier);
}

void dissemination_timing(int num_threads) {
  omp_dissemination_barrier_t barrier;
  omp_dissemination_barrier_init(&barrier, num_threads);

#pragma omp parallel
  {
    for (int i = 0; i < NUM_ITERS; i++) {
      omp_dissemination_barrier(&barrier);
    }
  }

  for (int j = 0; j < tries; j++) {
    start_watch_usec(&before);
#pragma omp parallel
    {
      for (int i = 0; i < NUM_ITERS; i++) {
        omp_dissemination_barrier(&barrier);
      }
    }
    stop_watch_usec(&after);
    results[j] = get_timer_diff_usec(&before, &after);
  }

  omp_dissemination_barrier_destroy(&barrier);
}

void mcs_timing(int num_threads) {
  omp_mcs_barrier_t barrier;
  omp_mcs_barrier_init(&barrier, num_threads);

#pragma omp parallel
  {
    for (int i = 0; i < NUM_ITERS; i++) {
      omp_mcs_barrier(&barrier);
    }
  }

  for (int j = 0; j < tries; j++) {
    start_watch_usec(&before);
#pragma omp parallel
    {
      for (int i = 0; i < NUM_ITERS; i++) {
        omp_mcs_barrier(&barrier);
      }
    }
    stop_watch_usec(&after);
    results[j] = get_timer_diff_usec(&before, &after);
  }

  omp_mcs_barrier_destroy(&barrier);
}

void omp_timing(int num_threads) {

#pragma omp parallel
  {
    for (int i = 0; i < NUM_ITERS; i++) {
#pragma omp barrier
    }
  }

  for (int j = 0; j < tries; j++) {
    start_watch_usec(&before);
#pragma omp parallel
    {
      for (int i = 0; i < NUM_ITERS; i++) {
#pragma omp barrier
      }
    }
    stop_watch_usec(&after);
    results[j] = get_timer_diff_usec(&before, &after);
  }

}

void warmup(int num_threads) {
  int array[10000];

  int block_size = BLOCK / num_threads;
  omp_set_num_threads(num_threads);
#pragma omp parallel
  {
    int tid = omp_get_thread_num();
    for (int i = tid * 100; i < tid * 100 + block_size; i++)
      array[i] = tid;
  }

  return;
}

void print_results(int num_threads) {
  /*
  for (int i = 0; i < tries; i++) {
    printf("%ld, ", results[i]);
  }
  */
  printf("%d, ", num_threads);
  double m = get_mean(results, tries);
  printf("%0.3f, ", m);
  printf("%0.3f, ", get_stddev(results, tries, m));
  printf("\n");
}

int main(int argc, char** argv) {
  if (argc != 3) {
    fprintf(stderr, "Invalid number of arguments. Usage: %s <barrier-type> <num-threads>", argv[0]);
    fprintf(stderr, "\tType 0: Centralized 1\n");
    fprintf(stderr, "\tType 1: Centralized 2\n");
    fprintf(stderr, "\tType 2: Tree\n");
    fprintf(stderr, "\tType 3: Tournament\n");
    fprintf(stderr, "\tType 4: Dissemination\n");
    fprintf(stderr, "\tType 5: MCS\n");
    fprintf(stderr, "\tType 6: OMP\n");
    exit(1);
  }

  int barrier_type = atoi(argv[1]);
  int num_threads = atoi(argv[2]);

  warmup(num_threads);

  switch (barrier_type) {
    case 0:
      printf("Centralized1, ");
      centralized_timing(num_threads);
      break;
    case 1:
      printf("Centralized2, ");
      centralized2_timing(num_threads);
      break;
    case 2:
      printf("Tree, ");
      tree_timing(num_threads);
      break;
    case 3:
      printf("Tournament, ");
      tournament_timing(num_threads);
      break;
    case 4:
      printf("Dissemination, ");
      dissemination_timing(num_threads);
      break;
    case 5:
      printf("MCS, ");
      mcs_timing(num_threads);
      break;
    case 6:
      printf("OMP, ");
      omp_timing(num_threads);
      break;
    default:
      printf("Invalid Barrier Type\n");
      return -1;
  }

  print_results(num_threads);

  return 0;
}
