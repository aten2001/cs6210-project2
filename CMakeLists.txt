cmake_minimum_required(VERSION 3.3)
project(project2)

find_package(MPI REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        omp_centralized_barrier.h
        omp_centralized_barrier.c

        omp_tree_barrier.h
        omp_tree_barrier.c

        omp_tournament_barrier.h
        omp_tournament_barrier.c

        omp_dissemination_barrier.h
        omp_dissemination_barrier.c

        omp_mcs_barrier.h
        omp_mcs_barrier.c

        mpi_dissemination_barrier.h
        mpi_dissemination_barrier.c

        mpi_tournament_barrier.h
        mpi_tournament_barrier.c

        mpi_mcs_barrier.h
        mpi_mcs_barrier.c

        helpers.h
        helpers.c
        omp_barriers.h
        mpi_barriers.h

        tests/omp_test0.c
        tests/omp_test1.c
        tests/mpi_test0.c
        tests/mpi_test1.c

        )
add_executable(project2 ${SOURCE_FILES})