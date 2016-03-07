#PBS -q cs6210
#PBS -l nodes=2:sixcore
#PBS -l walltime=00:05:00
#PBS -N output_mpi_test0_2nodes
/opt/openmpi-1.4.3-gcc44/bin/mpirun --hostfile $PBS_NODEFILE -np 2 ${PBS_O_WORKDIR}/mpi_test0 0 5
/opt/openmpi-1.4.3-gcc44/bin/mpirun --hostfile $PBS_NODEFILE -np 2 ${PBS_O_WORKDIR}/mpi_test0 1 5
/opt/openmpi-1.4.3-gcc44/bin/mpirun --hostfile $PBS_NODEFILE -np 2 ${PBS_O_WORKDIR}/mpi_test0 2 5
/opt/openmpi-1.4.3-gcc44/bin/mpirun --hostfile $PBS_NODEFILE -np 2 ${PBS_O_WORKDIR}/mpi_test0 3 5
