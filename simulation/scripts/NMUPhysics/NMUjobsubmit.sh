#!/bin/bash

source /home/tireman/simulation/jlab/nmu-npol/build/simulation/scripts/NMUPhysics/NMUsetuprun.sh

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
    $BUILD_DIR/Npolapp $BUILD_DIR/macros/Run$Energy\GeV/ParticleFlux$PType.mac 1>$NPOLDIR/dumpFiles/${NPOLBASENAME}_$i.out 2>$NPOLDIR/dumpFiles/${NPOLBASENAME}_$i.err &
    sleep 1s
done
