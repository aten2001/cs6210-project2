#include "mpi_tournament_barrier.h"

/**
 * Implementation of the tournament barrier
 */
void MPI_tournament_barrier(MPI_Comm comm, int tag) {
  int my_id, num_processes;

  MPI_Comm_size(comm, &num_processes);
  MPI_Comm_rank(comm, &my_id);

  if (num_processes == 1)
    return;

  if (my_id == 0) {
    // For simplicity, we will use different code for node 0,
    // since it is designated as the overall tournament winner.
    int i;
    // Wait for loser from each my_level
    for (i = 1; i < num_processes; i <<= 1) {
      MPI_Recv(NULL, 0, MPI_INT, i, tag, comm, NULL);
    }
    // All losers have sent message, so now wakeup losers
    for (i >>= 1; i > 0; i >>= 1) {
      MPI_Request req;
      MPI_Isend(NULL, 0, MPI_INT, i, tag, comm, &req);
    }
  } else {
    int win_stride = 1;
    // Winner wait loop
    while ((my_id % (2 * win_stride)) == 0) {
      // Wait for message from loser
      if ((my_id + win_stride) < num_processes) {
        MPI_Recv(NULL, 0, MPI_INT, my_id + win_stride, tag, comm, NULL);
      }
      win_stride *= 2;
    }

    // Send message to winner
    MPI_Send(NULL, 0, MPI_INT, my_id - win_stride, tag, comm);
    // Wait for wakeup from winner
    MPI_Recv(NULL, 0, MPI_INT, my_id - win_stride, tag, comm, NULL);

    // Send messages to my losers
    while (win_stride > 1) {
      MPI_Request req;
      win_stride /= 2;
      if ((my_id + win_stride) < num_processes) {
        MPI_Isend(NULL, 0, MPI_INT, my_id + win_stride, tag, comm, &req);
      }
    }
  }
}
