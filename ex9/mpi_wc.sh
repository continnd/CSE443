#!/bin/bash
#PBS -N print_args
#PBS -l walltime=0:05:00
#PBS -l mem=1GB
#PBS -l nodes=3:ppn=1

# Change to directory with actual executable

cd /home/continnd/CSE443/ex9/

# Run the executable

mpiexec -n 3 ./mpi_wc

# end of script
