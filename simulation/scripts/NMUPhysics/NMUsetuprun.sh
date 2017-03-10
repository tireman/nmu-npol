#!/bin/sh

export Lead=15
export Energy=4.4
export Bfield=4
export PType=Proton

export BUILD_DIR=/home/tireman/simulation/jlab/nmu-npol/build/simulation
export NPOLBASENAME=source$PType\_Lead$Lead\cm_$Energy\GeV_$Bfield\Bdl
export NPOLEVENTSPERFILE=100000
export NPOLDIR=/home/tireman/data1/TargetTaggerSource/$Energy\GeV/$Bfield\Bdl
export NPOLWORKDIR=/home/tireman/data1/TargetTaggerSource/$Energy\GeV/$Bfield\Bdl
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
