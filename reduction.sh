#!/bin/bash
#SBATCH -N 16
#SBATCH --ntasks-per-node=8
#SBATCH --cpus-per-task=7
#SBATCH --output=reduction_16.out
#SBATCH -A bip249
#SBATCH --job-name=reduction
#SBATCH --exclusive
#SBATCH --time=00:10:00      # hh:mm:ss for the job

export PPN=6
# single node: --network=single_node_vni
srun ./reductiontest +ppn $PPN +setcpuaffinity