#!/bin/bash

source scripts/NMUsetuprun.sh

for ((i=$1; i<=$2; i++))
do
    export JOBNUMBER=$i

    if [ -f "$BUILD_DIR/output/electron_11GeV_$i.out" ]
    then
	rm $BUILD_DIR/output/electron_11GeV_$i.out
	rm $BUILD_DIR/output/electron_11GeV_$i.err
    fi
    echo "Starting up Job Number $i."

    $BUILD_DIR/Npolapp $BUILD_DIR/npol.mac 1>$BUILD_DIR/output/electron_11GeV_$i.out 2>$BUILD_DIR/output/electron_11GeV_$i.err &
    sleep 5s

done
