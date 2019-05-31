#!/bin/sh

#SBATCH --partition=r930
#SBATCH -N1
#SBATCH -c96
#SBATCH --exclusive
#SBATCH -w caserta
srun -N1 -n1 --exclusive test_pivot.sh Google 3
