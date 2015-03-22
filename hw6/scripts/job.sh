#!/bin/sh

#PBS -l walltime=4:00:00
#PBS -m ae

export PATH="/home/mizunon/gentoo/usr/bin:/home/mizunon/gentoo/bin:/home/mizunon/tmp/usr/bin:/home/mizunon/gentoo/tmp/bin:$PATH"
cd /home/continnd/CSE443/hw6/
./scripts/run_cpp.sh continnd_hw6.cpp
