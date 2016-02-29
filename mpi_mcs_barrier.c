#include "mpi_mcs_barrier.h"
#include <assert.h>

/**
 * Implementation of the tournament barrier
 */
void MPI_mcs_barrier(MPI_Comm comm, int tag) {
  int my_id, num_processes;

  MPI_Comm_size(comm, &num_processes);
  MPI_Comm_rank(comm, &my_id);

  if (num_processes == 1)
    return;

  // Identify the number of children I am waiting for
  int wait_count = 0;
  int first_child = 4 * my_id + 1;
  if (first_child < num_processes) {
    wait_count = num_processes - first_child;
    if (wait_count > 4) {
      wait_count = 4;
    }
  }

  // Wait till all my children have arrived
  int num_arrived = 0;
  while (num_arrived != wait_count) {
    MPI_Status status;
    MPI_Recv(NULL, 0, MPI_DATATYPE_NULL, MPI_ANY_SOURCE, tag, comm, &status);
    assert((status.MPI_SOURCE >= (4 * my_id + 1) && (status.MPI_SOURCE < (4 * my_id + wait_count))));
    num_arrived++;
  }

  if (my_id != 0) {
    // Notify parent that all of my children have arrived
    int arrive_parent = (my_id - 1) / 4;
    MPI_Isend(NULL, 0, MPI_DATATYPE_NULL, arrive_parent, tag, comm, NULL);

    // Wait till we get wakeup message from wakeup parent
    int wakeup_parent = (my_id - 1) / 2;
    MPI_Recv(NULL, 0, MPI_DATATYPE_NULL, wakeup_parent, tag, comm, NULL);
  }

  // Send wakeup notification to children
  int child_id = 2 * my_id + 1;
  if (child_id < num_processes) {
    MPI_Isend(NULL, 0, MPI_DATATYPE_NULL, child_id, tag, comm, NULL);
    if ((child_id + 1) < num_processes) {
      MPI_Isend(NULL, 0, MPI_DATATYPE_NULL, child_id + 1, tag, comm, NULL);
    }
  }
}