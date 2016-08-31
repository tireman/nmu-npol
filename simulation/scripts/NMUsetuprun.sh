#!/bin/sh

export Lead=15
export Energy=4.4
export Bfield=4
export BUILD_DIR=/home/tireman/simulation/jlab/npol/nmu-npol/build/simulation
export NPOLBASENAME=sourceNeutron_Lead$Lead\cm_$Energy\GeV_$Bfield\Bdl
export NPOLEVENTSPERFILE=100000
export NPOLDIR=/data2/cgen/JlabSimData/Summer2016Run/TargetTaggerSource/4.4GeV/4Bdl
#export NPOLDIR=/home/tireman/simulation/jlab/npol/analysis/test
export NPOLWORKDIR=/data2/cgen/JlabSimData/Summer2016Run/TargetTaggerSource/4.4GeV/4Bdl
#export NPOLWORKDIR=/home/tireman/simulation/jlab/npol/analysis/test
export RawDataDir=/data2/cgen/JlabSimData/Summer2016Run/TargetTaggerSource/4.4GeV/4Bdl/root
export HistoOutputDir=/data2/cgen/JlabSimData/Summer2016Run/TargetTaggerSource/4.4GeV/4Bdl/histos

if [ ! -e $NPOLDIR ]
then
	mkdir $NPOLDIR
fi
if [ ! -e $NPOLDIR/root ]
then
	mkdir $NPOLDIR/root
fi
