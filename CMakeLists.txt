cmake_minimum_required(VERSION 3.3)
project(project2)

find_package(MPI REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES mpi_dissemination_barrier.c omp_tree_barrier.c omp_centralized_barrier.c omp_mcs_barrier.c mpi_mcs_barrier.c mpi_tournament_barrier.c omp_dissemination_barrier.c omp_tournament_barrier.c)
add_executable(project2 ${SOURCE_FILES})