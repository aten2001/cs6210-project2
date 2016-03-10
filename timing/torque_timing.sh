#PBS -q cs6210
#PBS -l nodes=1:sixcore
#PBS -l walltime=00:05:00
#PBS -N dmanOMP_3

for i in $(seq 0 6); do
    ./omp_timing $i 4
done
