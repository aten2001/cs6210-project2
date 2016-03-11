#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "mpi.h"
#include <omp.h>
#include "../combined_barrier.h"

void combined_test(int num_threads, int num_iters) {
  omp_set_dynamic(0);
  omp_set_num_threads(num_threads);
  int my_id, num_processes;
  MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

  combined_barrier_t barrier;
  combined_barrier_init(&barrier, num_threads);
  char* curtime;
  time_t rawtime;
  time(&rawtime);
  curtime = ctime(&rawtime);
  curtime[strlen(curtime)-1] = ' ';
  printf("Process %d/%d: %s- Combined Barrier Test Start\n", my_id, num_processes, curtime);
  #pragma omp parallel
  {
    int tid = omp_get_thread_num();
    for (int i = 1; i <= num_iters; i++) {
      time(&rawtime);
      curtime = ctime(&rawtime);
      curtime[strlen(curtime)-1] = ' ';
      printf("Process %d.%d/(%d, %d): %s- Iteration %d\n", my_id, tid, num_processes, omp_get_num_threads(), curtime, i);
      sleep(my_id + tid + 1);
      combined_barrier(&barrier, MPI_COMM_WORLD, 1234);
    }
  }
  
  combined_barrier_destroy(&barrier);
  time(&rawtime);
  curtime = ctime(&rawtime);
  curtime[strlen(curtime)-1] = ' ';
  printf("Process %d/%d: %s- Combined Barrier Test End\n", my_id, num_processes, curtime);
}

void omp_mpi_test(int num_threads, int num_iters) {
  omp_set_dynamic(0);
  omp_set_num_threads(num_threads);
  int my_id, num_processes;
  MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

  char* curtime;
  time_t rawtime;
  time(&rawtime);
  curtime = ctime(&rawtime);
  curtime[strlen(curtime)-1] = ' ';
  printf("Process %d/%d: %s- MPI Barrier Test Start\n", my_id, num_processes, curtime);
  #pragma omp parallel
  {
    int tid = omp_get_thread_num();
    for (int i = 1; i <= num_iters; i++) {
      time(&rawtime);
      curtime = ctime(&rawtime);
      curtime[strlen(curtime)-1] = ' ';
      printf("Process %d.%d/(%d, %d): %s- Iteration %d\n", my_id, tid, num_processes, omp_get_num_threads(), curtime, i);
      sleep(my_id + tid + 1);
      #pragma omp barrier
      #pragma omp single
      {
        MPI_Barrier(MPI_COMM_WORLD);
      }
    }
  }
  time(&rawtime);
  curtime = ctime(&rawtime);
  curtime[strlen(curtime)-1] = ' ';
  printf("Process %d/%d: %s- MPI Barrier Test End\n", my_id, num_processes, curtime);
}

int main(int argc, char** argv) {
  int barrier_type, num_threads, num_iters;
  if (argc != 4) {
    printf("Usage: ./%s <barrier-type> <num-threads> <num-iters>\n", argv[0]);
    return -1;
  }

  barrier_type = atoi(argv[1]);
  num_threads = atoi(argv[2]);
  num_iters = atoi(argv[3]);

  MPI_Init(&argc, &argv);
  switch (barrier_type) {
    case 0:
      combined_test(num_threads, num_iters);
      break;
    case 1:
      omp_mpi_test(num_threads, num_iters);
      break;
    default:
      printf("Invalid Barrier Type\n");
      return -1;
  }

  MPI_Finalize();
  return 0;
}



