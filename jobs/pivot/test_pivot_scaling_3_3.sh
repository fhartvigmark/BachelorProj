#!/bin/sh

#SBATCH --partition=main
#SBATCH -N1
#SBATCH -c16
#SBATCH --exclusive
#SBATCH -w ctit064
srun -N1 -n1 --exclusive test_pivot_maxdeg_color_scaling.sh Google 3
