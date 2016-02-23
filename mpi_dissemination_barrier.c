//
// Created by manatunga on 2/16/16.
//
#include <mpi/mpi.h>


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
    // Send non-blocking message to my_id + 2^k
    MPI_Isend(NULL, 0, MPI_DATATYPE_NULL, (my_id + offset) % num_processes, tag, comm, NULL);
    // Wait till we receive message from process my_id - 2^k
    MPI_Recv(NULL, 0, MPI_DATATYPE_NULL, (my_id - offset) % num_processes, tag, comm, NULL);
  }
}