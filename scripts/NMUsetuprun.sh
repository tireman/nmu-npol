#!/bin/sh

export BUILD_DIR=/home/tireman/simulation/jlab/npol/nmu-npol/build
export NPOLBASENAME=neutron_2.1GeV
export NPOLEVENTSPERFILE=100000
export NPOLDIR=/data3/cgen/NMUSimData/NeutronOnly/2.1GeV

if [ ! -e $NPOLDIR ]
then
	mkdir $NPOLDIR
fi
if [ ! -e $NPOLDIR/root ]
then
	mkdir $NPOLDIR/root
fi
