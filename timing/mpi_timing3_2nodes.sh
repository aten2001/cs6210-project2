#PBS -q cs6210
#PBS -l nodes=2:sixcore
#PBS -l walltime=00:10:00
#PBS -N output_mpi_timing3_2nodes
/opt/openmpi-1.4.3-gcc44/bin/mpirun --hostfile $PBS_NODEFILE -np 2 ${PBS_O_WORKDIR}/mpi_barrier_timing3 0
/opt/openmpi-1.4.3-gcc44/bin/mpirun --hostfile $PBS_NODEFILE -np 2 ${PBS_O_WORKDIR}/mpi_barrier_timing3 1
/opt/openmpi-1.4.3-gcc44/bin/mpirun --hostfile $PBS_NODEFILE -np 2 ${PBS_O_WORKDIR}/mpi_barrier_timing3 2
/opt/openmpi-1.4.3-gcc44/bin/mpirun --hostfile $PBS_NODEFILE -np 2 ${PBS_O_WORKDIR}/mpi_barrier_timing3 3
