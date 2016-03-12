#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include "mpi.h"
#include "../mpi_barriers.h"
#include "timer.h"

#define BLOCK 10000
#define tries 100
#define NUM_ITERS 1000

int array[BLOCK];
long results[tries];

struct timespec before, after;

void combined_timing(int num_threads)
{
  int my_id, num_processes;

  MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

  combined_barrier_t barrier;
  combined_barrier_init(&barrier, num_threads);

  for (int j = 0; j < tries; j++) {
    start_watch(&before);

    #pragma omp parallel
    {
      for (int k = 0; k < NUM_ITERS; k++) {
        combined_barrier(&barrier, MPI_COMM_WORLD, 1234);
      }
    }

    stop_watch(&after);
    results[j] = get_timer_diff(&before, &after);
  }

  combined_barrier_destroy(&barrier);
}

void baseline(int num_threads)
{
  int my_id, num_processes;
  MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

  for (int j = 0; j < tries; j++) {
    start_watch(&before);

    #pragma omp parallel
    {
      for (int i = 1; i <= NUM_ITERS; i++) {
        #pragma omp barrier
        #pragma omp single
        {
          MPI_Barrier(MPI_COMM_WORLD);
        }
      }
    }

    stop_watch(&after);
    results[j] = get_timer_diff(&before, &after);
  }
}

void warmup(int num_threads)
{
  volatile int array[10000];

  int my_id, num_processes;
  MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

  omp_set_dynamic(0);
  omp_set_num_threads(num_threads);

  int block_size = BLOCK / num_processes;
  int tid = my_id;
#pragma omp for
  for (int i = tid * 100; i < tid*100+block_size; i++)
    array[i] = tid;
  return;
}

void print_results()
{
  /*
  for (int i = 0; i < tries; i++) {
    printf("%ld, ", results[i]);
  }
  */
  int my_id, num_processes;
  MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
  printf("Process %d/%d, ", my_id, num_processes);
  double m = get_mean(results, tries);
  printf("%0.3f, ", m);
  printf("%0.3f, ", get_stddev(results, tries, m));
  printf("\n");
}

int main(int argc, char** argv) {
  if (argc != 2) {
    fprintf(stderr, "Invalid number of arguments. Usage: %s <num_threads>", argv[0]);
    exit(1);
  }

  int num_threads = atoi(argv[1]);

  MPI_Init(&argc, &argv);

  warmup(num_threads);

  baseline(num_threads);
  print_results();

  combined_timing(num_threads);
  print_results();

  MPI_Finalize();
  return 0;
}
