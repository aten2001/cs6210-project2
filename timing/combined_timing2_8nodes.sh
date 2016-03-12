#PBS -q cs6210
#PBS -l nodes=8:sixcore
#PBS -l walltime=00:10:00
#PBS -N output_combined_timing2_8nodes
for j in $(seq 2 12); do
    /opt/openmpi-1.4.3-gcc44/bin/mpirun --hostfile $PBS_NODEFILE -np 8 ${PBS_O_WORKDIR}/combined_barrier_timing2 $j
done
