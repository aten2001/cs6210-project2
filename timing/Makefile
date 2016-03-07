all:
	gcc --std=gnu99 omp_barrier_timing.c -fopenmp -o omp_timing ../omp*.c ../helpers.c -O3 -lm -lrt

clean:
	rm -f omp_timing
