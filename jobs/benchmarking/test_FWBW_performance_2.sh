#!/bin/sh

#SBATCH --partition=main
#SBATCH -N1
#SBATCH -c64
#SBATCH --exclusive
#SBATCH -w weleveld
srun -N1 -n1 --exclusive test_FWBW_performance.sh Stanford 200
