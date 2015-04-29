cd $HOME/CSE443/hw7/
export OMP_NUM_THREADS=8
for i in `seq 5`; do
    /usr/bin/time -v ./ImageSearch Mammogram.png Cancer_mask.png mammogramResult.png true 1> logs/mammogramOutput8.txt 2>> logs/mammogramTime8.txt
done
export OMP_NUM_THREADS=4
for i in `seq 5`; do
    /usr/bin/time -v ./ImageSearch Mammogram.png Cancer_mask.png mammogramResult.png true 1> logs/mammogramOutput4.txt 2>> logs/mammogramTime4.txt
done
export OMP_NUM_THREADS=1
for i in `seq 5`; do
    /usr/bin/time -v ./ImageSearch Mammogram.png Cancer_mask.png mammogramResult.png true 1> logs/mammogramOutput1.txt 2>> logs/mammogramTime1.txt
done
