cd $HOME/CSE443/ex6/
export OMP_NUM_THREADS=1
./exercise6_part1 1> output.txt 2> error.txt
export OMP_NUM_THREADS=2
./exercise6_part1 1> output2.txt 2> error2.txt
export OMP_NUM_THREADS=4
./exercise6_part1 1> output3.txt 2> error3.txt
