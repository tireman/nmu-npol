#!/bin/sh

export BUILD_DIR=/home/tireman/simulation/jlab/npol/nmu-npol/build
export NPOLBASENAME=npol_Lead15cm_4.4GeV_4Bdl
export NPOLEVENTSPERFILE=500000
export NPOLDIR=/data2/cgen/NMUSimData/4.4GeV/4Bdl/Lead15cm

if [ ! -e $NPOLDIR ]
then
	mkdir $NPOLDIR
fi
if [ ! -e $NPOLDIR/root ]
then
	mkdir $NPOLDIR/root
fi
