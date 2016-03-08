OMPFLAGS = -fopenmp
OMPLIBS = -lgomp

CC = gcc
CPPFLAGS = -g -Wall -std=gnu99 -O3
LDFLAGS = -g -Wall 
LDLIBS = -lm -lrt

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

OMP_TIMING_SOURCES = omp_barrier_timing.c
OMP_TIMING_OBJECTS = $(OMP_TIMING_SOURCES:.c=.o)
OMP_TIMING = omp_barrier_timing

MPI_TIMING_SOURCES = mpi_barrier_timing.c
MPI_TIMING_OBJECTS = $(MPI_TIMING_SOURCES:.c=.o)
MPI_TIMING = mpi_barrier_timing

TARGETS = $(OMP_TIMING) $(MPI_TIMING)

all: $(TARGETS)

$(OMP_TIMING): % : %.o $(OMP_BARRIER_OBJECTS) $(HELPER_OBJECTS)
	$(CC) -o $@ $(OMPLIBS) $(LDLIBS) $^

$(OMP_TIMING_OBJECTS): %.o : %.c
	$(CC) -c -o $@ $(CPPFLAGS) $(OMPFLAGS) $^

$(OMP_BARRIER_OBJECTS): %.o : $(PROJECT_ROOT_DIR)/%.c
	$(CC) -c -o $@ $(CPPFLAGS)  $(OMPFLAGS) $^

$(MPI_TIMING): % : %.o $(MPI_BARRIER_OBJECTS) $(HELPER_OBJECTS)
	$(MPICC) -o $@ $(LDLIBS) $^

$(MPI_TIMING_OBJECTS) : %.o : %.c
	$(MPICC) -c -o $@ $(CPPFLAGS) $(MPIFLAGS) $^

$(MPI_BARRIER_OBJECTS): %.o : $(PROJECT_ROOT_DIR)/%.c
	$(MPICC) -c -o $@ $(CPPFLAGS) $(MPIFLAGS) $^

$(HELPER_OBJECTS): %.o : $(PROJECT_ROOT_DIR)/%.c
	$(CC) -c -o $@ $(CPPFLAGS) $^

clean:
	@echo " Cleaning..."; rm -f *.o $(TARGETS)
