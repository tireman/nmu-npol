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

    if [ -f "$NPOLDIR/${NPOLBASENAME}_$i.out" ]
    then
		rm $NPOLDIR/${NPOLBASENAME}_$i.out
		rm $NPOLDIR/${NPOLBASENAME}_$i.err
    fi

	echo "Starting up Job Number $i."
    $BUILD_DIR/Npolapp $BUILD_DIR/macros/ParticleFlux.mac 1>$NPOLDIR/${NPOLBASENAME}_$i.out 2>$NPOLDIR/${NPOLBASENAME}_$i.err &
    sleep 5s
done
