OMPFLAGS = -fopenmp
OMPLIBS = -lgomp

CC = gcc
CPPFLAGS = -g -Wall -std=gnu99 -O3

MPICC = /opt/openmpi-1.4.3-gcc44/bin/mpicc 
MPIFLAGS = -I/opt/openmpi-1.4.3-gcc44/include

PROJECT_ROOT_DIR = ..

MPI_BARRIER_SOURCES = $(PROJECT_ROOT_DIR)/mpi_dissemination_barrier.c \
                      $(PROJECT_ROOT_DIR)/mpi_tournament_barrier.c \
                      $(PROJECT_ROOT_DIR)/mpi_mcs_barrier.c

OMP_BARRIER_SOURCES = $(PROJECT_ROOT_DIR)/omp_centralized_barrier.c \
                      $(PROJECT_ROOT_DIR)/omp_tree_barrier.c \
                      $(PROJECT_ROOT_DIR)/omp_tournament_barrier.c \
                      $(PROJECT_ROOT_DIR)/omp_dissemination_barrier.c \
                      $(PROJECT_ROOT_DIR)/omp_mcs_barrier.c

COMBINED_BARRIER_SOURCES = $(PROJECT_ROOT_DIR)/combined_barrier.c

MPI_BARRIER_OBJECTS = $(patsubst $(PROJECT_ROOT_DIR)/%, %, $(MPI_BARRIER_SOURCES:.c=.o))
OMP_BARRIER_OBJECTS = $(patsubst $(PROJECT_ROOT_DIR)/%, %, $(OMP_BARRIER_SOURCES:.c=.o))
COMBINED_BARRIER_OBJECTS = $(patsubst $(PROJECT_ROOT_DIR)/%, %, $(COMBINED_BARRIER_SOURCES:.c=.o))

OMP_TEST_SOURCES = omp_test0.c omp_test1.c
OMP_TEST_OBJECTS = $(OMP_TEST_SOURCES:.c=.o)
OMP_TESTS = omp_test0 omp_test1
OMP_TESTS_PADDED = $(OMP_TESTS:%=%_padded)

MPI_TEST_SOURCES = mpi_test0.c mpi_test1.c
MPI_TEST_OBJECTS = $(MPI_TEST_SOURCES:.c=.o)
MPI_TESTS = mpi_test0 mpi_test1

COMBINED_TEST_SOURCES = combined_test0.c combined_test1.c
COMBINED_TEST_OBJECTS = $(COMBINED_TEST_SOURCES:.c=.o)
COMBINED_TESTS = combined_test0 combined_test1

TARGETS = $(OMP_TESTS) $(OMP_TESTS_PADDED) $(MPI_TESTS) $(COMBINED_TESTS)

all: $(TARGETS)

$(OMP_TESTS): % : %.o $(OMP_BARRIER_OBJECTS)
	$(CC) -o $@ $(OMPLIBS) $^

$(OMP_TESTS_PADDED): %_padded : %.o $(OMP_BARRIER_OBJECTS:.o=_padded.o)
	$(CC) -o $@ $(OMPLIBS) $^

$(OMP_TEST_OBJECTS): %.o : %.c
	$(CC) -c -o $@ $(CPPFLAGS) $(OMPFLAGS) $^

$(OMP_BARRIER_OBJECTS): %.o : $(PROJECT_ROOT_DIR)/%.c
	$(CC) -c -o $@ $(CPPFLAGS)  $(OMPFLAGS) $^

$(OMP_BARRIER_OBJECTS:%.o=%_padded.o): %_padded.o : $(PROJECT_ROOT_DIR)/%.c
	$(CC) -c -o $@ $(CPPFLAGS) -DCACHE_PADDING $(OMPFLAGS) $^

$(MPI_TESTS): % : %.o $(MPI_BARRIER_OBJECTS)
	$(MPICC) -o $@ $(CPPFLAGS) $(MPIFLAGS) $^

$(MPI_TEST_OBJECTS) : %.o : %.c
	$(MPICC) -c -o $@ $(CPPFLAGS) $(MPIFLAGS) $^

$(MPI_BARRIER_OBJECTS): %.o : $(PROJECT_ROOT_DIR)/%.c
	$(MPICC) -c -o $@ $(CPPFLAGS) $(MPIFLAGS) $^

$(COMBINED_TESTS): % : %.o $(COMBINED_BARRIER_OBJECTS)
	$(MPICC) -o $@ $(CPPFLAGS) $(MPIFLAGS) $(OMPLIBS) $^

$(COMBINED_TEST_OBJECTS) : %.o : %.c
	$(MPICC) -c -o $@ $(CPPFLAGS) $(MPIFLAGS) $(OMPFLAGS) $^

$(COMBINED_BARRIER_OBJECTS): %.o : $(PROJECT_ROOT_DIR)/%.c
	$(MPICC) -c -o $@ $(CPPFLAGS) $(MPIFLAGS) $(OMPFLAGS) $^

clean:
	@echo " Cleaning..."; rm -f *.o $(TARGETS)
