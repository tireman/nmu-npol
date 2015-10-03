#!/bin/bash

source scripts/NMUsetuprun.sh

for ((i=$1; i<=$2; i++))
do
    export JOBNUMBER=$i
    echo $JOBNUMBER
    if [ -f "out_$i.out" ]
    then
	rm $BUILD_DIR/output/out_$i.out
	echo "Starting up Job Number $i."
    else
	echo "Starting up Job Number $i."
    fi
    
    $BUILD_DIR/Npolapp $BUILD_DIR/npol.mac >$BUILD_DIR/output/out_$i.out &
    sleep 5s

done
