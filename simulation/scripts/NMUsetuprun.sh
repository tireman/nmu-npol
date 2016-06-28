#!/bin/sh

export BUILD_DIR=/home/tireman/simulation/jlab/npol/nmu-npol/build/simulation
export NPOLBASENAME=npol_Lead15cm_4.4GeV_4Bdl
export NPOLEVENTSPERFILE=100000
export NPOLDIR=/data1/cgen/NeutronOnly/PointSource/test

if [ ! -e $NPOLDIR ]
then
	mkdir $NPOLDIR
fi
if [ ! -e $NPOLDIR/root ]
then
	mkdir $NPOLDIR/root
fi
