OMPFLAGS = -fopenmp
OMPLIBS = -lgomp

CC = gcc
CPPFLAGS = -g -Wall -std=gnu99 -O3
LDFLAGS = -g -Wall 
LDLIBS = -lm -lrt

MPICC = /opt/openmpi-1.4.3-gcc44/bin/mpicc 
MPIFLAGS = -I/opt/openmpi-1.4.3-gcc44/include

PROJECT_ROOT_DIR = ..

MPI_BARRIER_SOURCES = $(PROJECT_ROOT_DIR)/mpi_dissemination_barrier.c \
                      $(PROJECT_ROOT_DIR)/mpi_tournament_barrier.c \
                      $(PROJECT_ROOT_DIR)/mpi_mcs_barrier.c \
											$(PROJECT_ROOT_DIR)/combined_barrier.c

OMP_BARRIER_SOURCES = $(PROJECT_ROOT_DIR)/omp_centralized_barrier.c \
                      $(PROJECT_ROOT_DIR)/omp_tree_barrier.c \
                      $(PROJECT_ROOT_DIR)/omp_tournament_barrier.c \
                      $(PROJECT_ROOT_DIR)/omp_dissemination_barrier.c \
                      $(PROJECT_ROOT_DIR)/omp_mcs_barrier.c

MPI_BARRIER_OBJECTS = $(patsubst $(PROJECT_ROOT_DIR)/%, %, $(MPI_BARRIER_SOURCES:.c=.o))
OMP_BARRIER_OBJECTS = $(patsubst $(PROJECT_ROOT_DIR)/%, %, $(OMP_BARRIER_SOURCES:.c=.o))

OMP_TIMING_SOURCES = omp_barrier_timing.c omp_barrier_timing2.c omp_barrier_timing3.c 
OMP_TIMING_OBJECTS = $(OMP_TIMING_SOURCES:.c=.o)
OMP_TIMING = omp_barrier_timing omp_barrier_timing2 omp_barrier_timing3
OMP_TIMING_PADDED = $(OMP_TIMING:%=%_padded)

MPI_TIMING_SOURCES = mpi_barrier_timing.c mpi_barrier_timing2.c mpi_barrier_timing3.c combined_barrier_timing2.c
MPI_TIMING_OBJECTS = $(MPI_TIMING_SOURCES:.c=.o)
MPI_TIMING = mpi_barrier_timing mpi_barrier_timing2 mpi_barrier_timing3 combined_barrier_timing2

TARGETS = $(OMP_TIMING) $(OMP_TIMING_PADDED) $(MPI_TIMING)

all: $(TARGETS)

$(OMP_TIMING): % : %.o $(OMP_BARRIER_OBJECTS)
	$(CC) -o $@ $(OMPLIBS) $(LDLIBS) $^

$(OMP_TIMING_PADDED): %_padded : %.o $(OMP_BARRIER_OBJECTS:.o=_padded.o)
	$(CC) -o $@ $(OMPLIBS) $(LDLIBS) $^
	
$(OMP_TIMING_OBJECTS): %.o : %.c
	$(CC) -c -o $@ $(CPPFLAGS) $(OMPFLAGS) $^

$(OMP_BARRIER_OBJECTS): %.o : $(PROJECT_ROOT_DIR)/%.c
	$(CC) -c -o $@ $(CPPFLAGS)  $(OMPFLAGS) $^

$(OMP_BARRIER_OBJECTS:%.o=%_padded.o): %_padded.o : $(PROJECT_ROOT_DIR)/%.c
	$(CC) -c -o $@ $(CPPFLAGS) -DCACHE_PADDING $(OMPFLAGS) $^

$(MPI_TIMING): % : %.o $(MPI_BARRIER_OBJECTS)
	$(MPICC) -o $@ $(LDLIBS) $^ $(OMPFLAGS) $(OMPLIBS)

$(MPI_TIMING_OBJECTS) : %.o : %.c
	$(MPICC) -c -o $@ $(CPPFLAGS) $(MPIFLAGS) $^ $(OMPFLAGS) $(OMPLIBS)

$(MPI_BARRIER_OBJECTS): %.o : $(PROJECT_ROOT_DIR)/%.c
	$(MPICC) -c -o $@ $(CPPFLAGS) $(MPIFLAGS) $^ $(OMPFLAGS) $(OMPLIBS)

$(HELPER_OBJECTS): %.o : $(PROJECT_ROOT_DIR)/%.c
	$(CC) -c -o $@ $(CPPFLAGS) $^

clean:
	@echo " Cleaning..."; rm -f *.o $(TARGETS)
