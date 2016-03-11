#PBS -q cs6210
#PBS -l nodes=8:sixcore
#PBS -l walltime=00:10:00
#PBS -N output_combined_timing2_8nodes
/opt/openmpi-1.4.3-gcc44/bin/mpirun --hostfile $PBS_NODEFILE -np 8 ${PBS_O_WORKDIR}/combined_barrier_timing2 4 2
/opt/openmpi-1.4.3-gcc44/bin/mpirun --hostfile $PBS_NODEFILE -np 8 ${PBS_O_WORKDIR}/combined_barrier_timing2 4 4
/opt/openmpi-1.4.3-gcc44/bin/mpirun --hostfile $PBS_NODEFILE -np 8 ${PBS_O_WORKDIR}/combined_barrier_timing2 4 6
/opt/openmpi-1.4.3-gcc44/bin/mpirun --hostfile $PBS_NODEFILE -np 8 ${PBS_O_WORKDIR}/combined_barrier_timing2 4 8
/opt/openmpi-1.4.3-gcc44/bin/mpirun --hostfile $PBS_NODEFILE -np 8 ${PBS_O_WORKDIR}/combined_barrier_timing2 4 10
/opt/openmpi-1.4.3-gcc44/bin/mpirun --hostfile $PBS_NODEFILE -np 8 ${PBS_O_WORKDIR}/combined_barrier_timing2 4 12
