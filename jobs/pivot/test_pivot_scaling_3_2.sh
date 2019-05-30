#!/bin/sh

#SBATCH --partition=main
#SBATCH -N1
#SBATCH -c16
#SBATCH --exclusive
#SBATCH -w ctit063
srun -N1 -n1 --exclusive test_pivot_maxdeg_scaling.sh Google 3
