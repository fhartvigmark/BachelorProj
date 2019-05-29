#!/bin/sh

#SBATCH --partition=main
#SBATCH -N 1
#SBATCH -c 12
#SBATCH --exclusive
#SBATCH -w ctit081
srun -N1 -n1 --exclusive test_pivot_maxdeg_scaling.sh
