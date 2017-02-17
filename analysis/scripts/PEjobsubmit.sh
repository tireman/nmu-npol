#!/bin/bash

for ((i=$1; i<=$2; i++))
do
    export JOBNUMBER=$i
    $BUILD_DIR/../analysis/NpolAnalysis 1>$NPOLDIR/${NPOLBASENAME}_$i.out 2>$NPOLDIR/${NPOLBASENAME}_$i.err 
    sleep 1s
done    

