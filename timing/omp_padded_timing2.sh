#PBS -q cs6210
#PBS -l nodes=1:fourcore
#PBS -l walltime=00:20:00
#PBS -N output_omp_padded_timing2
for i in $(seq 0 6); do
    for j in $(seq 2 8); do
        ${PBS_O_WORKDIR}/omp_barrier_timing2_padded $i $j
    done
done      
