#PBS -q cs6210
#PBS -l nodes=4:sixcore
#PBS -l walltime=00:20:00
#PBS -N output_combined_test0_4nodes
for i in $(seq 0 1); do
    for j in $(seq 2 12); do
	/opt/openmpi-1.4.3-gcc44/bin/mpirun --hostfile $PBS_NODEFILE -np 4 ${PBS_O_WORKDIR}/combined_test0 $i $j 4
    done
done      
