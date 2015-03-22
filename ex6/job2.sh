cd $HOME/CSE443/ex6/
export OMP_NUM_THREADS=1
for i in `seq 2`; do
    /usr/bin/time -v ./ex6_part2_parallel 175000 1> paralleloutput1$i.txt 2> time1$i.txt
done
export OMP_NUM_THREADS=2
for i in `seq 2`; do
    /usr/bin/time -v ./ex6_part2_parallel 175000 1> paralleloutput2$i.txt 2> time2$i.txt
done
export OMP_NUM_THREADS=3
for i in `seq 2`; do
    /usr/bin/time -v ./ex6_part2_parallel 175000 1> paralleloutput3$i.txt 2> time3$i.txt
done
export OMP_NUM_THREADS=4
for i in `seq 2`; do
    /usr/bin/time -v ./ex6_part2_parallel 175000 1> paralleloutput4$i.txt 2> time4$i.txt
done
