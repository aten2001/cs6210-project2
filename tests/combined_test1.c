#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "mpi.h"
#include <omp.h>
#include "../combined_barrier.h"

void combined_test(int num_threads, int skip_process, int skip_thread) {
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
     time(&rawtime);
    curtime = ctime(&rawtime);
    curtime[strlen(curtime)-1] = ' ';
    printf("Process %d.%d/(%d, %d): %s - Before Barrier\n", my_id, tid, num_processes, omp_get_num_threads(), curtime);
    if (my_id != skip_process || tid != skip_thread) {
      combined_barrier(&barrier, MPI_COMM_WORLD, 1234);
    }
    printf("Process %d.%d/(%d, %d): %s - After Barrier\n", my_id, tid, num_processes, omp_get_num_threads(), curtime);
  }
  
  combined_barrier_destroy(&barrier);
  time(&rawtime);
  curtime = ctime(&rawtime);
  curtime[strlen(curtime)-1] = ' ';
  printf("Process %d/%d: %s- Combined Barrier Test End\n", my_id, num_processes, curtime);
}

void omp_mpi_test(int num_threads, int skip_process, int skip_thread) {
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
    time(&rawtime);
    curtime = ctime(&rawtime);
    curtime[strlen(curtime)-1] = ' ';
    printf("Process %d.%d/(%d, %d): %s - Before Barrier\n", my_id, tid, num_processes, omp_get_num_threads(), curtime);
    if (my_id != skip_process || tid != skip_thread) {
      #pragma omp barrier
      #pragma omp single
      {
        MPI_Barrier(MPI_COMM_WORLD);
      }
    }
    printf("Process %d.%d/(%d, %d): %s - After Barrier\n", my_id, tid, num_processes, omp_get_num_threads(), curtime);
  }
  time(&rawtime);
  curtime = ctime(&rawtime);
  curtime[strlen(curtime)-1] = ' ';
  printf("Process %d/%d: %s- MPI Barrier Test End\n", my_id, num_processes, curtime);
}

int main(int argc, char** argv) {
  int barrier_type, num_threads, skip_process, skip_thread;
  if (argc != 5) {
    printf("Usage: ./%s <barrier-type> <num-threads> <skip-process> <skip-thread>\n", argv[0]);
    return -1;
  }

  barrier_type = atoi(argv[1]);
  num_threads = atoi(argv[2]);
  skip_process = atoi(argv[3]);
  skip_thread = atoi(argv[4]);

  MPI_Init(&argc, &argv);
  switch (barrier_type) {
    case 0:
      combined_test(num_threads, skip_process, skip_thread);
      break;
    case 1:
      omp_mpi_test(num_threads, skip_process, skip_thread);
      break;
    default:
      printf("Invalid Barrier Type\n");
      return -1;
  }

  MPI_Finalize();
  return 0;
}



