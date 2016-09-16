#!/bin/bash

if [ $# -eq 0 ]
then
	START=1
	FINISH=1
elif [ $# -eq 1 ]
then
	START=1
	FINISH=$1
else
	START=$1
	FINISH=$(($1 + $2 - 1))
fi

for ((i=$START; i<=$FINISH; i++))
do
    export JOBNUMBER=$i

    if [ -f "$HistoOutputDir/${NPOLBASENAME}_$i.out" ]
    then
		rm $HistoOutputDir/${NPOLBASENAME}_$i.out
		rm $HistoOutputDir/${NPOLBASENAME}_$i.err
    fi

	echo "Starting up Job Number $i."
    $BUILD_DIR/../analysis/NpolAnalysis 1>$HistoOutputDir/${NPOLBASENAME}_$i.out 2>$HistoOutputDir/${NPOLBASENAME}_$i.err &
    sleep 5s
done
