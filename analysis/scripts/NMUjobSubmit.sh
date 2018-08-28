#!/bin/bash

for ((i=$1; i<=$2; i++))
do
    export JOBNUMBER=$i
    $BUILD_DIR/../analysis/NpolAnalysis 1>$NPOLWORKDIR/dumpFiles/${NPOLBASENAME}Analysis1_$i.out 2>$NPOLWORKDIR/dumpFiles/${NPOLBASENAME}Analysis1_$i.err
	
	#$BUILD_DIR/../analysis2/NpolProcessEvents 1>$NPOLDIR/dumpFiles/${NPOLBASENAME}Analysis2_$i.out 2>$NPOLDIR/dumpFiles/${NPOLBASENAME}Analysis2_$i.err &
    sleep 1s
done    

