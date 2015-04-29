cd $HOME/CSE443/hw7/
# export OMP_NUM_THREADS=8
# for i in `seq 5`; do
    # /usr/bin/time -v ./ImageSearch MiamiMarcumCenter.png WindowPane_mask.png windowResult.png true 50 64 1> logs/windowOutput8.txt 2>> logs/windowTime8.txt
# done
# export OMP_NUM_THREADS=4
# for i in `seq 5`; do
    # /usr/bin/time -v ./ImageSearch MiamiMarcumCenter.png WindowPane_mask.png windowResult.png true 50 64 1> logs/windowOutput4.txt 2>> logs/windowTime4.txt
# done
export OMP_NUM_THREADS=1
for i in `seq 5`; do
    /usr/bin/time -v ./ImageSearch MiamiMarcumCenter.png WindowPane_mask.png windowResult.png true 50 64 1> logs/windowOutput1.txt 2>> logs/windowTime1.txt
done
