#!/bin/sh

#SBATCH --partition=r930
#SBATCH -N1
#SBATCH -c32
#SBATCH --exclusive
#SBATCH -w caserta
srun -N1 -n1 --exclusive test_pivot_randwalk_scaling.sh Pokec 1
