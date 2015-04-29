cd $HOME/CSE443/hw7/
export OMP_NUM_THREADS=8
for i in `seq 5`; do
    /usr/bin/time -v ./ImageSearch Mammogram.png Cancer_mask.png result.png true 1> logs/paralleloutput8.txt 2>> logs/time8.txt
done
export OMP_NUM_THREADS=4
for i in `seq 5`; do
    /usr/bin/time -v ./ImageSearch Mammogram.png Cancer_mask.png result.png true 1> logs/paralleloutput4.txt 2>> logs/time4.txt
done
export OMP_NUM_THREADS=1
for i in `seq 5`; do
    /usr/bin/time -v ./ImageSearch Mammogram.png Cancer_mask.png result.png true 1> logs/paralleloutput1.txt 2>> logs/time1.txt
done
