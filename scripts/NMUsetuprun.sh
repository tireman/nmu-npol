#!/bin/sh

export BUILD_DIR=/home/tireman/simulation/jlab/npol/nmu-npol/build
export NPOLBASENAME=test_1_Lead10cm_4.4GeV_4Bdl
export NPOLEVENTSPERFILE=100000
export NPOLDIR=/data3/cgen/NMUSimData/4.4GeV/4Bdl/Lead10cm

if [ ! -e $NPOLDIR ]
then
	mkdir $NPOLDIR
fi
if [ ! -e $NPOLDIR/root ]
then
	mkdir $NPOLDIR/root
fi
