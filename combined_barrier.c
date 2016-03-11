#include "combined_barrier.h"
#include "helpers.h"

void combined_barrier_init(combined_barrier_t* barrier, int num_threads) {
  barrier->count = barrier->N = num_threads;
  barrier->sense = true;
}

void combined_barrier_destroy(combined_barrier_t* barrier) {

}

void combined_barrier(combined_barrier_t* barrier, MPI_Comm comm, int tag) {
  int my_id, num_processes;

  MPI_Comm_size(comm, &num_processes);
  MPI_Comm_rank(comm, &my_id);

  int local_sense = !barrier->sense;
  if (__sync_fetch_and_sub(&barrier->count, 1) == 1) {
    if (num_processes > 1) {
      for (int offset = 1; offset < num_processes; offset *= 2) {
        MPI_Request req;
        // Send non-blocking message to my_id + 2^k
        MPI_Isend(NULL, 0, MPI_INT, mod(my_id + offset, num_processes), tag, comm, &req);
        // Wait till we receive message from process my_id - 2^k
        MPI_Recv(NULL, 0, MPI_INT, mod(my_id - offset, num_processes), tag, comm, NULL);
      }
    }
    barrier->count = barrier->N;
    barrier->sense = local_sense;
  } else {
    while (barrier->sense != local_sense);
  }
}
