#!/bin/sh

#SBATCH --partition=main
#SBATCH -N1
#SBATCH -c12
#SBATCH --exclusive
#SBATCH -w ctit081
srun -N1 -n1 --exclusive test_pivot_random_scaling.sh &
srun -N1 -n1 --exclusive test_pivot_maxdeg_scaling.sh &
srun -N1 -n1 --exclusive test_pivot_maxdeg_color_scaling.sh &
srun -N1 -n1 --exclusive test_pivot_randwalk_scaling.sh &
srun -N1 -n1 --exclusive test_pivot_maxdeg_precomp_scaling.sh
