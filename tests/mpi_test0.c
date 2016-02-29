#include <stdio.h>
#include <mpi/mpi.h>
#include <stdlib.h>
#include "../mpi_barriers.h"

void tournament_test(int num_iters) {
  int my_id, num_processes;
  MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

  for (int i = 0; i < num_iters; i++) {
    printf("Process %d/%d: Iteration %d\n", my_id, num_processes, i);
    MPI_tournament_barrier(MPI_COMM_WORLD, 1234);
  }
}

void dissemination_test(int num_iters) {
  int my_id, num_processes;
  MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

  for (int i = 0; i < num_iters; i++) {
    printf("Process %d/%d: Iteration %d\n", my_id, num_processes, i);
    MPI_dissemination_barrier(MPI_COMM_WORLD, 1234);
  }
}

void mcs_test(int num_iters) {
  int my_id, num_processes;
  MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

  for (int i = 0; i < num_iters; i++) {
    printf("Process %d/%d: Iteration %d\n", my_id, num_processes, i);
    MPI_mcs_barrier(MPI_COMM_WORLD, 1234);
  }
}

int main(int argc, char** argv) {
  int barrier_type, num_iters;
  if (argc != 3) {
    printf("Usage: ./%s <barrier-type> <num-iters>", argv[0]);
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
    default:
      printf("Invalid Barrier Type\n");
      return -1;
  }

  MPI_Finalize();
  return 0;
}



