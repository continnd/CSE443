cd $HOME/CSE443/hw7/
export OMP_NUM_THREADS=8
for i in `seq 5`; do
    /usr/bin/time -v ./ImageSearch TestImage.png and_mask.png and_result.png true 1> logs/andOutput8.txt 2>> logs/andTime8.txt
done
export OMP_NUM_THREADS=4
for i in `seq 5`; do
    /usr/bin/time -v ./ImageSearch TestImage.png and_mask.png and_result.png true 1> logs/andOutput4.txt 2>> logs/andTime4.txt
done
export OMP_NUM_THREADS=1
for i in `seq 5`; do
    /usr/bin/time -v ./ImageSearch TestImage.png and_mask.png and_result.png true 1> logs/andOutput1.txt 2>> logs/andTime1.txt
done
