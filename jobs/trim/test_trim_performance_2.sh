#!/bin/sh

#SBATCH --partition=r930
#SBATCH -N1
#SBATCH -c32
#SBATCH --exclusive
#SBATCH -w caserta
srun -N1 -n1 --exclusive test_trim_performance.sh Stanford
