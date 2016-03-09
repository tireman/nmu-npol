#!/bin/sh

export JOBNUMBER=$1
export Lead=10
export Energy=4.4
export Bfield=4
export BUILD_DIR=/home/tireman/simulation/jlab/npol/nmu-npol/build
export NPOLBASENAME=test_1_Lead10cm_4.4GeV_4Bdl
export NPOLDIR=/data3/cgen/NMUSimData/4.4GeV/4Bdl/Lead10cm

export OutputDir=$NPOLDIR/Output/
export InputDir=$NPOLDIR/root/

if [ ! -e $NPOLDIR ]
then
	mkdir $NPOLDIR
fi
if [ ! -e $NPOLDIR/Output ]
then
	mkdir $NPOLDIR/Output
fi
