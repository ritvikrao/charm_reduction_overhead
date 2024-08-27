#!/bin/bash
#SBATCH -N 1
#SBATCH --ntasks-per-node=8
#SBATCH --cpus-per-task=7
#SBATCH --output=reduction_1.out
#SBATCH -A bip249
#SBATCH --job-name=reduction
#SBATCH --exclusive
#SBATCH --time=00:10:00      # hh:mm:ss for the job

export PPN=6
# single node: --network=single_node_vni
srun --network=single_node_vni ./reductiontest +ppn $PPN +setcpuaffinity