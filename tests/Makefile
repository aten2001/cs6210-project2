OMPFLAGS = -fopenmp
OMPLIBS = -lgomp

CC = gcc
CPPFLAGS = -g -Wall -std=c99
LDFLAGS = -g -Wall
LDLIBS = $(OMPLIBS)

MPICC = mpicc
MPICH = /usr/lib64/openmpi/1.4-gcc
CFLAGS = -I$(MPICH)/include

PROJECT_ROOT_DIR = ..
MPI_BARRIER_SOURCES = $(PROJECT_ROOT_DIR)/mpi_dissemination_barrier.c \
                      $(PROJECT_ROOT_DIR)/mpi_tournament_barrier.c \
                      $(PROJECT_ROOT_DIR)/mpi_mcs_barrier.c \
                      $(PROJECT_ROOT_DIR)/helpers.c

OMP_BARRIER_SOURCES = $(PROJECT_ROOT_DIR)/omp_centralized_barrier.c \
                      $(PROJECT_ROOT_DIR)/omp_tree_barrier.c \
                      $(PROJECT_ROOT_DIR)/omp_tournament_barrier.c \
                      $(PROJECT_ROOT_DIR)/omp_dissemination_barrier.c \
                      $(PROJECT_ROOT_DIR)/omp_mcs_barrier.c \
                      $(PROJECT_ROOT_DIR)/helpers.c

MPI_BARRIER_OBJECTS = $(MPI_BARRIER_SOURCES:.c=.o)
OMP_BARRIER_OBJECTS = $(patsubst $(PROJECT_ROOT_DIR)/%, %, $(OMP_BARRIER_SOURCES:.c=.o))

OMP_TEST_SOURCES = omp_test0.c omp_test1.c
OMP_TEST_OBJECTS = $(OMP_TEST_SOURCES:.c=.o)

OMP_TESTS = omp_test0 omp_test1

TARGETS = mpi_test0 mpi_test1 $(OMP_TESTS)

all: $(OMP_TESTS)

$(OMP_TESTS): % : %.o $(OMP_BARRIER_OBJECTS)
	$(CC) -o $@ $(CPPFLAGS) $(OMPFLAGS) $^

$(OMP_TEST_OBJECTS): %.o : %.c
	$(CC) -c -o $@ $(CPPFLAGS) $(OMPFLAGS) $^

$(OMP_BARRIER_OBJECTS): %.o : $(PROJECT_ROOT_DIR)/%.c
	$(CC) -c -o $@ $(CPPFLAGS)  $(OMPFLAGS) $^

clean:
	@echo " Cleaning..."; rm -f *.o $(TARGETS)
