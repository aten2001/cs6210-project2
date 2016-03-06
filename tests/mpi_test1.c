#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include "../mpi_barriers.h"

void tournament_test(int skip_thread) {
  int my_id, num_processes;
  MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

  printf("Process %d/%d: Before Barrier\n", my_id, num_processes);
  if (my_id != skip_thread) {
    MPI_tournament_barrier(MPI_COMM_WORLD, 1234);
  }
  printf("Process %d/%d: After Barrier\n", my_id, num_processes);

}

void dissemination_test(int skip_thread) {
  int my_id, num_processes;
  MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

  printf("Process %d/%d: Before Barrier\n", my_id, num_processes);
  if (my_id != skip_thread) {
    MPI_dissemination_barrier(MPI_COMM_WORLD, 1234);
  }
  printf("Process %d/%d: After Barrier\n", my_id, num_processes);

}

void mcs_test(int skip_thread) {
  int my_id, num_processes;
  MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

  printf("Process %d/%d: Before Barrier\n", my_id, num_processes);
  if (my_id != skip_thread) {
    MPI_mcs_barrier(MPI_COMM_WORLD, 1234);
  }
  printf("Process %d/%d: After Barrier\n", my_id, num_processes);
}

int main(int argc, char** argv) {
  int barrier_type, skip_thread;
  if (argc != 3) {
    printf("Usage: ./%s <barrier-type> <skip-thread>", argv[0]);
  }

  barrier_type = atoi(argv[2]);
  skip_thread = atoi(argv[2]);

  MPI_Init(&argc, &argv);
  switch (barrier_type) {
    case 0:
      tournament_test(skip_thread);
      break;
    case 1:
      dissemination_test(skip_thread);
      break;
    case 2:
      mcs_test(skip_thread);
      break;
    default:
      printf("Invalid Barrier Type\n");
      return -1;
  }

  MPI_Finalize();
  return 0;
}

