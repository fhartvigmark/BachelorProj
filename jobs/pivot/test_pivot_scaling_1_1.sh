#!/bin/sh

#SBATCH --partition=main
#SBATCH -N1
#SBATCH -c16
#SBATCH --exclusive
#SBATCH -w ctit061
srun -N1 -n1 --exclusive test_pivot_random_scaling.sh Slashdot 0