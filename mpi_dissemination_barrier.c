#include "mpi_dissemination_barrier.h"
#include <stdio.h>

int mod(int a, int b) {
  int r = a % b;
  return r < 0 ? r + b : r;
}

/**
 * Implementation of the dissemination barrier
 */
void MPI_dissemination_barrier(MPI_Comm comm, int tag) {
  int my_id, num_processes;

  MPI_Comm_size(comm, &num_processes);
  MPI_Comm_rank(comm, &my_id);

  if (num_processes == 1)
    return;

  for (int offset = 1; offset < num_processes; offset *= 2) {
    MPI_Request req;
    // Send non-blocking message to my_id + 2^k
    MPI_Isend(NULL, 0, MPI_INT, mod(my_id + offset, num_processes), tag, comm, &req);
    // Wait till we receive message from process my_id - 2^k
    MPI_Recv(NULL, 0, MPI_INT, mod(my_id - offset, num_processes), tag, comm, NULL);
  }
}
