#!/bin/sh

#export JOBNUMBER=$1
export Lead=15
export Energy=4.4
export Bfield=4
export BUILD_DIR=/home/tireman/simulation/jlab/npol/nmu-npol/build
export NPOLBASENAME=npol
export NPOLDIR=/data2/cgen/NMUSimData/4.4GeV/4Bdl/Lead15cm
export NPOLWORKDIR=/data2/cgen/NMUSimData/4.4GeV/4Bdl/Lead15cm

export OutputDir=$NPOLDIR/root
export InputDir=$NPOLDIR/root
export WorkOutputDir=$NPOLWORKDIR/Plots
export WorkInputDir=$NPOLWORKDIR/root

if [ ! -e $NPOLDIR ]
then
	mkdir $NPOLDIR
fi
if [ ! -e $NPOLDIR/Output ]
then
	mkdir $NPOLDIR/Output
fi
