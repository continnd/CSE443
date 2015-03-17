#!/bin/sh

FILE=$1
shift

SIZE="3000"
# SIZE="500 750 1000 1500 2000 2500 3000"

for S in $SIZE; do
    cat $FILE | perl -p -e "s/(?<=MATRIX_SIZE = )\d+/$S/" > tmp
    mv tmp $FILE

    javac $FILE
    for I in `seq 5`; do
        /usr/bin/time -v java -Xmx2g `echo $FILE | sed -e 's/\.java//'` 2>>java_$S.log
    done
done
