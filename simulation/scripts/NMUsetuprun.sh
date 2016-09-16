#!/bin/sh

export Lead=15
export Energy=4.4
export Bfield=4
export BUILD_DIR=/home/tireman/simulation/jlab/npol/nmu-npol/build/simulation
export NPOLBASENAME=sourceNeutron_Lead$Lead\cm_$Energy\GeV_$Bfield\Bdl
export NPOLEVENTSPERFILE=100000
export NPOLDIR=/data1/cgen/NeutronOnly/RealisticSource/10cmScint
export NPOLWORKDIR=/data1/cgen/NeutronOnly/RealisticSource/10cmScint
export RawDataDir=$NPOLDIR/root
export HistoOutputDir=$NPOLDIR/histos

if [ ! -e $NPOLDIR ]
then
	mkdir $NPOLDIR
fi
if [ ! -e $NPOLDIR/root ]
then
	mkdir $NPOLDIR/root
fi
