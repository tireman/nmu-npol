#!/bin/sh

export BUILD_DIR=/home/tireman/simulation/jlab/npol/nmu-npol/build
export NPOLBASENAME=npolLead10cm_11GeV_4Bdl
export NPOLEVENTSPERFILE=100000
export NPOLDIR=/data3/cgen/NMUSimData/11GeV/4Bdl/Lead10cm

if [ !(-e $NPOLDIR) ]
then
	mkdir $NPOLDIR
fi
if [ !(-e $NPOLDIR/root) ]
then
	mkdir $NPOLDIR/root
fi
