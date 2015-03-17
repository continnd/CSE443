#!/bin/bash

FILE=$1
FILE2=$2

SIZE="500 750 1000 1500 2000 2500 3000"

for S in $SIZE; do
    cat $FILE | perl -p -e "s/(?<=MATRIX_SIZE )\d+/$S/" > tmp
    mv tmp $FILE

    EXEC_NAME=`echo $FILE | sed -e 's/\.cpp//'`
    icpc -O2 -std=c++11 -g -Wall $FILE $FILE2 -o $EXEC_NAME
    for I in `seq 5`; do
        /usr/bin/time -v ./$EXEC_NAME 2>>logs/cppblock_$S.log
    done
done
