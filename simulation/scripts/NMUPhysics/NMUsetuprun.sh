#!/bin/sh

export Lead=15
export Energy=4.4
export Bfield=4
export BUILD_DIR=/home/tireman/simulation/jlab/npol/nmu-npol/build/simulation
export NPOLBASENAME=sourceMuon+_Lead$Lead\cm_$Energy\GeV_$Bfield\Bdl
export NPOLEVENTSPERFILE=100000
export NPOLDIR=/data2/cgen/JlabSimData/December2016/TargetTaggerSource/$Energy\GeV/$Bfield\Bdl/TargetTagger56by21
export NPOLWORKDIR=/data2/cgen/JlabSimData/December2016/TargetTaggerSource/$Energy\GeV/$Bfield\Bdl/TargetTagger56by21
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
