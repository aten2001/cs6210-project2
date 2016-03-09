#PBS -q cs6210
#PBS -l nodes=11:sixcore
#PBS -l walltime=00:10:00
#PBS -N output_mpi_timing_11nodes
/opt/openmpi-1.4.3-gcc44/bin/mpirun --hostfile $PBS_NODEFILE -np 11 ${PBS_O_WORKDIR}/mpi_barrier_timing 0
/opt/openmpi-1.4.3-gcc44/bin/mpirun --hostfile $PBS_NODEFILE -np 11 ${PBS_O_WORKDIR}/mpi_barrier_timing 1
/opt/openmpi-1.4.3-gcc44/bin/mpirun --hostfile $PBS_NODEFILE -np 11 ${PBS_O_WORKDIR}/mpi_barrier_timing 2
/opt/openmpi-1.4.3-gcc44/bin/mpirun --hostfile $PBS_NODEFILE -np 11 ${PBS_O_WORKDIR}/mpi_barrier_timing 3
