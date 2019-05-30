#!/bin/sh

#SBATCH --partition=main
#SBATCH -N1
#SBATCH -c16
#SBATCH --exclusive
#SBATCH -w ctit066
srun -N1 -n1 --exclusive test_pivot_randwalk2_scaling.sh Google 3
