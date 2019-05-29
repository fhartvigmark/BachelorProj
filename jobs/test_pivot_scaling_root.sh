#!/bin/sh

#SBATCH --partition=main
#SBATCH -N1
#SBATCH -c24
#SBATCH -w westervlier
srun -N1 -n1 --exclusive test_pivot_random_scaling.sh