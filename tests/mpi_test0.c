#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include "../mpi_barriers.h"

void tournament_test(int num_iters) {
  int my_id, num_processes;
  MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

  printf("Process %d/%d: Tournament Barrier Test Start\n", my_id, num_processes);
  for (int i = 1; i <= num_iters; i++) {
    printf("Process %d/%d: Iteration %d\n", my_id, num_processes, i);
    MPI_tournament_barrier(MPI_COMM_WORLD, 1234);
  }
  printf("Process %d/%d: Tournament Barrier Test End\n", my_id, num_processes);
}

void dissemination_test(int num_iters) {
  int my_id, num_processes;
  MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

  printf("Process %d/%d: Dissemination Barrier Test Start\n", my_id, num_processes);
  for (int i = 1; i <= num_iters; i++) {
    printf("Process %d/%d: Iteration %d\n", my_id, num_processes, i);
    MPI_dissemination_barrier(MPI_COMM_WORLD, 1234);
  }
  printf("Process %d/%d: Dissemination Barrier Test End\n", my_id, num_processes);
}

void mcs_test(int num_iters) {
  int my_id, num_processes;
  MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

  printf("Process %d/%d: MCS Barrier Test Start\n", my_id, num_processes);
  for (int i = 1; i <= num_iters; i++) {
    printf("Process %d/%d: Iteration %d\n", my_id, num_processes, i);
    MPI_mcs_barrier(MPI_COMM_WORLD, 1234);
  }
  printf("Process %d/%d: MCS Barrier Test End\n", my_id, num_processes);
}

void mpi_test(int num_iters) {
  int my_id, num_processes;
  MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

  printf("Process %d/%d: MPI Barrier Test Start\n", my_id, num_processes);
  for (int i = 1; i <= num_iters; i++) {
    printf("Process %d/%d: Iteration %d\n", my_id, num_processes, i);
    MPI_Barrier(MPI_COMM_WORLD);
  }
  printf("Process %d/%d: MPI Barrier Test End\n", my_id, num_processes);
}

int main(int argc, char** argv) {
  int barrier_type, num_iters;
  if (argc != 3) {
    printf("Usage: ./%s <barrier-type> <num-iters>\n", argv[0]);
    return -1;
  }

  barrier_type = atoi(argv[1]);
  num_iters = atoi(argv[2]);

  MPI_Init(&argc, &argv);
  switch (barrier_type) {
    case 0:
      tournament_test(num_iters);
      break;
    case 1:
      dissemination_test(num_iters);
      break;
    case 2:
      mcs_test(num_iters);
      break;
    case 3:
      mpi_test(num_iters);
      break;
    default:
      printf("Invalid Barrier Type\n");
      return -1;
  }

  MPI_Finalize();
  return 0;
}



