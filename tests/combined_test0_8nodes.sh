#PBS -q cs6210
#PBS -l nodes=8:sixcore
#PBS -l walltime=00:05:00
#PBS -N output_combined_test0_8nodes
for i in $(seq 0 1); do
    for j in $(seq 2 12); do
	/opt/openmpi-1.4.3-gcc44/bin/mpirun --hostfile $PBS_NODEFILE -np 8 ${PBS_O_WORKDIR}/combined_test0 $i $j 5
    done
done      
