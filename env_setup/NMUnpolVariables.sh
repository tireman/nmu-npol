#!/bin/sh

export Lead=15
export Energy=4.4
export Bfield=4
export pType=QENeutron
export NPOLBASENAME=source$pType\_Lead$Lead\cm_$Energy\GeV_$Bfield\Bdl
export NPOLEVENTSPERFILE=10000

export BUILD_DIR=/home/tireman/simulation/jlab/nmu-npol/build/simulation
export NPOLLIB_DIR=/home/tireman/simulation/jlab/nmu-npol/build/npollib

export NPOLDIR=/home/tireman/data1/TargetTaggerSource/$pType\RawSim/$Energy\GeV/$Bfield\Bdl/Lead$Lead\cm/Location_2
export NPOLWORKDIR=/home/tireman/data1/TargetTaggerSource/$pType\RawSim/$Energy\GeV/$Bfield\Bdl/Lead$Lead\cm/Location_2

export RawDataDir=$NPOLDIR/root
export OutputDir=$NPOLDIR/Output
export InputDir=$NPOLDIR/root
export WorkOutputDir=$NPOLWORKDIR
export WorkInputDir=$NPOLWORKDIR
export HistoOutputDir=$NPOLWORKDIR/histos
export HistoInputDir=$NPOLWORKDIR/histos
export PlotsOutputDir=$NPOLWORKDIR/Plots
export DumpFileDIR=$NPOLDIR/dumpFiles

if [ ! -e $NPOLDIR ]
then
	mkdir -p $NPOLDIR
fi
if [ ! -e $NPOLWORKDIR ]
then
	mkdir -p $NPOLWORKDIR
fi
if [ ! -e $NPOLDIR/root ]
then
	mkdir -p $NPOLDIR/root
fi
if [ ! -e $NPOLDIR/Output ]
then
	mkdir -p $NPOLDIR/Output
fi
if [ ! -e $NPOLWORKDIR/Output ]
then
	mkdir -p $NPOLWORKDIR/Output
fi
if [ ! -e $NPOLDIR/Plots ]
then
	mkdir -p $NPOLDIR/Plots
fi
if [ ! -e $NPOLWORKDIR/Plots ]
then
	mkdir -p $NPOLWORKDIR/Plots
fi
if [ ! -e $NPOLDIR/histos ]
then
	mkdir -p $NPOLDIR/histos
fi

if [ ! -e $NPOLDIR/dumpFiles ]
then
	mkdir -p $NPOLDIR/dumpFiles
fi

if [ ! -e $NPOLWORKDIR/dumpFiles ]
then
	mkdir -p $NPOLWORKDIR/dumpFiles
fi

if [ ! -e $NPOLWORKDIR/histos ]
then
	mkdir -p $NPOLWORKDIR/histos
fi

