#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"
#include "../mpi_barriers.h"
#include "timer.h"

#define BLOCK 10000
#define tries 100
#define NUM_ITERS 100

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
    for (int k = 0; k < NUM_ITERS; k++) {
      combined_barrier(&barrier, MPI_COMM_WORLD, 1234);
    }
    stop_watch(&after);
    results[j] = get_timer_diff(&before, &after);
  }

  combined_barrier_destroy(&barrier);
}

void tournament_timing() {
  int my_id, num_processes;
  MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

  for (int k = 0; k < NUM_ITERS; k++) {
    MPI_tournament_barrier(MPI_COMM_WORLD, 1234);
  }
  
  for (int j = 0; j < tries; j++) {
    start_watch(&before);
    for (int k = 0; k < NUM_ITERS; k++) {
      MPI_tournament_barrier(MPI_COMM_WORLD, 1234);
    }
    stop_watch(&after);
    results[j] = get_timer_diff(&before, &after);
  }
}

void dissemination_timing() {
  int my_id, num_processes;
  MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

  for (int k = 0; k < NUM_ITERS; k++) {
    MPI_dissemination_barrier(MPI_COMM_WORLD, 1234);
  }

  for (int j = 0; j < tries; j++) {
    start_watch(&before);
    for (int k = 0; k < NUM_ITERS; k++) {
      MPI_dissemination_barrier(MPI_COMM_WORLD, 1234);
    }
    stop_watch(&after);
    results[j] = get_timer_diff(&before, &after);
  }
}

void mcs_timing() {
  int my_id, num_processes;
  MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

  for (int k = 0; k < NUM_ITERS; k++) {
    MPI_mcs_barrier(MPI_COMM_WORLD, 1234);
  }

  for (int j = 0; j < tries; j++) {
    start_watch(&before);
    for (int k = 0; k < NUM_ITERS; k++) {
      MPI_mcs_barrier(MPI_COMM_WORLD, 1234);
    }
    stop_watch(&after);
    results[j] = get_timer_diff(&before, &after);
  }
}

void mpi_timing() {
  int my_id, num_processes;
  MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

  for (int k = 0; k < NUM_ITERS; k++) {
    MPI_Barrier(MPI_COMM_WORLD);
  }

  for (int j = 0; j < tries; j++) {
    start_watch(&before);
    for (int k = 0; k < NUM_ITERS; k++) {
      MPI_Barrier(MPI_COMM_WORLD);
    }
    stop_watch(&after);
    results[j] = get_timer_diff(&before, &after);
  }
}

void warmup()
{
  int array[10000];

  int my_id, num_processes;
  MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

  int block_size = BLOCK / num_processes;
  int tid = my_id;
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
    fprintf(stderr, "Invalid number of arguments. Usage: %s <barrier-type>", argv[0]);
    fprintf(stderr, "\tType 0: Tournament\n");
    fprintf(stderr, "\tType 1: Dissemination\n");
    fprintf(stderr, "\tType 2: MCS\n");
    fprintf(stderr, "\tType 3: MPI\n");
    fprintf(stderr, "\tType 4: Combined\n");
    exit(1);
  }

  int barrier_type = atoi(argv[1]);

  MPI_Init(&argc, &argv);

  warmup();

  switch (barrier_type) {
    case 0:
      printf("Tournament, ");
      tournament_timing();
      break;
    case 1:
      printf("Dissemination, ");
      dissemination_timing();
      break;
    case 2:
      printf("MCS, ");
      mcs_timing();
      break;
    case 3:
      printf("MPI, ");
      mpi_timing();
      break;
    case 4:
      assert(argc > 2);
      printf("Combined, ");
      combined_timing(atoi(argv[2]));
    default:
      printf("Invalid Barrier Type\n");
      return -1;
  }
  print_results();
  MPI_Finalize();
  return 0;
}
