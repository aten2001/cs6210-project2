OMPFLAGS = -fopenmp
OMPLIBS = -lgomp

CC = gcc
CPPFLAGS = -g -Wall -std=c99 -O3

MPICC = /opt/openmpi-1.4.3-gcc44/bin/mpicc 
MPIFLAGS = -I/opt/openmpi-1.4.3-gcc44/include

PROJECT_ROOT_DIR = ..

HELPER_SOURCES = $(PROJECT_ROOT_DIR)/helpers.c
MPI_BARRIER_SOURCES = $(PROJECT_ROOT_DIR)/mpi_dissemination_barrier.c \
                      $(PROJECT_ROOT_DIR)/mpi_tournament_barrier.c \
                      $(PROJECT_ROOT_DIR)/mpi_mcs_barrier.c

OMP_BARRIER_SOURCES = $(PROJECT_ROOT_DIR)/omp_centralized_barrier.c \
                      $(PROJECT_ROOT_DIR)/omp_tree_barrier.c \
                      $(PROJECT_ROOT_DIR)/omp_tournament_barrier.c \
                      $(PROJECT_ROOT_DIR)/omp_dissemination_barrier.c \
                      $(PROJECT_ROOT_DIR)/omp_mcs_barrier.c

HELPER_OBJECTS = $(patsubst $(PROJECT_ROOT_DIR)/%, %, $(HELPER_SOURCES:.c=.o))
MPI_BARRIER_OBJECTS = $(patsubst $(PROJECT_ROOT_DIR)/%, %, $(MPI_BARRIER_SOURCES:.c=.o))
OMP_BARRIER_OBJECTS = $(patsubst $(PROJECT_ROOT_DIR)/%, %, $(OMP_BARRIER_SOURCES:.c=.o))

OMP_TEST_SOURCES = omp_test0.c omp_test1.c
OMP_TEST_OBJECTS = $(OMP_TEST_SOURCES:.c=.o)
OMP_TESTS = omp_test0 omp_test1

MPI_TEST_SOURCES = mpi_test0.c mpi_test1.c
MPI_TEST_OBJECTS = $(MPI_TEST_SOURCES:.c=.o)
MPI_TESTS = mpi_test0 mpi_test1

TARGETS = $(OMP_TESTS) $(OMP_TESTS:%=%_padded) $(MPI_TESTS)

all: $(TARGETS)

$(OMP_TESTS): % : %.o $(OMP_BARRIER_OBJECTS) $(HELPER_OBJECTS)
	$(CC) -o $@ $(OMPLIBS) $^

$(OMP_PADDED_TESTS): %_padded : %.o $(OMP_BARRIER_OBJECTS:.o=_padded.o) $(HELPER_OBJECTS)
    $(CC) -o $@ $(OMPLIBS) $^

$(OMP_TEST_OBJECTS): %.o : %.c
	$(CC) -c -o $@ $(CPPFLAGS) $(OMPFLAGS) $^

$(OMP_BARRIER_OBJECTS): %.o : $(PROJECT_ROOT_DIR)/%.c
	$(CC) -c -o $@ $(CPPFLAGS)  $(OMPFLAGS) $^

$(OMP_BARRIER_OBJECTS): %_padded.o : $(PROJECT_ROOT_DIR)/%.c
	$(CC) -c -o $@ $(CPPFLAGS) -DCACHE_PADDING $(OMPFLAGS) $^

$(MPI_TESTS): % : %.o $(MPI_BARRIER_OBJECTS) $(HELPER_OBJECTS)
	$(MPICC) -o $@ $(CPPFLAGS) $(MPIFLAGS) $^

$(MPI_TEST_OBJECTS) : %.o : %.c
	$(MPICC) -c -o $@ $(CPPFLAGS) $(MPIFLAGS) $^

$(MPI_BARRIER_OBJECTS): %.o : $(PROJECT_ROOT_DIR)/%.c
	$(MPICC) -c -o $@ $(CPPFLAGS) $(MPIFLAGS) $^

$(HELPER_OBJECTS): %.o : $(PROJECT_ROOT_DIR)/%.c
	$(CC) -c -o $@ $(CPPFLAGS) $^

clean:
	@echo " Cleaning..."; rm -f *.o $(TARGETS)
