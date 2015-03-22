cd $HOME/CSE443/ex6/
export OMP_NUM_THREADS=1
for i in `seq 2`; do
    /usr/bin/time -v ./ex6_part2 175000 1> origoutput1$i.txt 2>> origtime1.txt
done
export OMP_NUM_THREADS=2
for i in `seq 2`; do
    /usr/bin/time -v ./ex6_part2 175000 1> origoutput2$i.txt 2>> origtime2.txt
done
export OMP_NUM_THREADS=3
for i in `seq 2`; do
    /usr/bin/time -v ./ex6_part2 175000 1> origoutput3$i.txt 2>> origtime3.txt
done
export OMP_NUM_THREADS=4
for i in `seq 2`; do
    /usr/bin/time -v ./ex6_part2 175000 1> origoutput4$i.txt 2>> origtime4.txt
done
