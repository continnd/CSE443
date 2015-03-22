cd $HOME/CSE443/hw6/
export OMP_NUM_THREADS=8
for i in `seq 5`; do
    /usr/bin/time -v ./continnd_hw6 1000000000 1001000000 1> logs4/paralleloutput8.txt 2>> logs4/time8.txt
done
export OMP_NUM_THREADS=7
for i in `seq 5`; do
    /usr/bin/time -v ./continnd_hw6 1000000000 1001000000 1> logs4/paralleloutput7.txt 2>> logs4/time7.txt
done
export OMP_NUM_THREADS=6
for i in `seq 5`; do
    /usr/bin/time -v ./continnd_hw6 1000000000 1001000000 1> logs4/paralleloutput6.txt 2>> logs4/time6.txt
done
export OMP_NUM_THREADS=5
for i in `seq 5`; do
    /usr/bin/time -v ./continnd_hw6 1000000000 1001000000 1> logs4/paralleloutput5.txt 2>> logs4/time5.txt
done
export OMP_NUM_THREADS=4
for i in `seq 5`; do
    /usr/bin/time -v ./continnd_hw6 1000000000 1001000000 1> logs4/paralleloutput4.txt 2>> logs4/time4.txt
done
export OMP_NUM_THREADS=3
for i in `seq 5`; do
    /usr/bin/time -v ./continnd_hw6 1000000000 1001000000 1> logs4/paralleloutput3.txt 2>> logs4/time3.txt
done
export OMP_NUM_THREADS=2
for i in `seq 5`; do
    /usr/bin/time -v ./continnd_hw6 1000000000 1001000000 1> logs4/paralleloutput2.txt 2>> logs4/time2.txt
done
export OMP_NUM_THREADS=1
for i in `seq 5`; do
    /usr/bin/time -v ./continnd_hw6 1000000000 1001000000 1> logs4/paralleloutput1.txt 2>> logs4/time1.txt
done
