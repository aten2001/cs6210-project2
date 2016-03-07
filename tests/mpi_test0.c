#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "mpi.h"
#include "../mpi_barriers.h"

void tournament_test(int num_iters) {
  int my_id, num_processes;
  MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

  char* curtime;
  time_t rawtime;
  time(&rawtime);
  curtime = ctime(&rawtime);
  curtime[strlen(curtime)-1] = ' ';
  printf("Process %d/%d: %s- Tournament Barrier Test Start\n", my_id, num_processes, curtime);
  for (int i = 1; i <= num_iters; i++) {
    time(&rawtime);
    curtime = ctime(&rawtime);
    curtime[strlen(curtime)-1] = ' ';
    printf("Process %d/%d: %s- Iteration %d\n", my_id, num_processes, curtime, i);
    sleep(my_id + 1);
    MPI_tournament_barrier(MPI_COMM_WORLD, 1234);
  }
  time(&rawtime);
  curtime = ctime(&rawtime);
  curtime[strlen(curtime)-1] = ' ';
  printf("Process %d/%d: %s- Tournament Barrier Test End\n", my_id, num_processes, curtime);
}

void dissemination_test(int num_iters) {
  int my_id, num_processes;
  MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

  char* curtime;
  time_t rawtime;
  time(&rawtime);
  curtime = ctime(&rawtime);
  curtime[strlen(curtime)-1] = ' ';
  printf("Process %d/%d: %s- Dissemination Barrier Test Start\n", my_id, num_processes, curtime);
  for (int i = 1; i <= num_iters; i++) {
    time(&rawtime);
    curtime = ctime(&rawtime);
    curtime[strlen(curtime)-1] = ' ';
    printf("Process %d/%d: %s- Iteration %d\n", my_id, num_processes, curtime, i);
    sleep(my_id + 1);
    MPI_dissemination_barrier(MPI_COMM_WORLD, 1234);
  }
  time(&rawtime);
  curtime = ctime(&rawtime);
  curtime[strlen(curtime)-1] = ' ';
  printf("Process %d/%d: %s- Dissemination Barrier Test End\n", my_id, num_processes, curtime);
}

void mcs_test(int num_iters) {
  int my_id, num_processes;
  MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

  char* curtime;
  time_t rawtime;
  time(&rawtime);
  curtime = ctime(&rawtime);
  curtime[strlen(curtime)-1] = ' ';
  printf("Process %d/%d: %s- MCS Barrier Test Start\n", my_id, num_processes, curtime);
  for (int i = 1; i <= num_iters; i++) {
    time(&rawtime);
    curtime = ctime(&rawtime);
    curtime[strlen(curtime)-1] = ' ';
    printf("Process %d/%d: %s- Iteration %d\n", my_id, num_processes, curtime, i);
    sleep(my_id + 1);
    MPI_mcs_barrier(MPI_COMM_WORLD, 1234);
  }
  time(&rawtime);
  curtime = ctime(&rawtime);
  curtime[strlen(curtime)-1] = ' ';
  printf("Process %d/%d: %s- MCS Barrier Test End\n", my_id, num_processes, curtime);
}

void mpi_test(int num_iters) {
  int my_id, num_processes;
  MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

  char* curtime;
  time_t rawtime;
  time(&rawtime);
  curtime = ctime(&rawtime);
  curtime[strlen(curtime)-1] = ' ';
  printf("Process %d/%d: %s- MPI Barrier Test Start\n", my_id, num_processes, curtime);
  for (int i = 1; i <= num_iters; i++) {
    time(&rawtime);
    curtime = ctime(&rawtime);
    curtime[strlen(curtime)-1] = ' ';
    printf("Process %d/%d: %s- Iteration %d\n", my_id, num_processes, curtime, i);
    sleep(my_id + 1);
    MPI_Barrier(MPI_COMM_WORLD);
  }
  time(&rawtime);
  curtime = ctime(&rawtime);
  curtime[strlen(curtime)-1] = ' ';
  printf("Process %d/%d: %s- MPI Barrier Test End\n", my_id, num_processes, curtime);
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



