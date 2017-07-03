#!/bin/bash

export Lead=5
export Energy=4.4
export Bfield=4
export NPOLLIB_DIR=$BUILD_DIR/npollib
export NPOLBASENAME=source$PType\_Lead$Lead\cm_$Energy\GeV_$Bfield\Bdl
export NPOLEVENTSPERFILE=100000
export NPOLDIR=/scratch
export NPOLWORKDIR=/scratch
export OutputDir=$NPOLDIR/Output
export InputDir=$NPOLDIR/root
export WorkOutputDir=$NPOLWORKDIR
export WorkInputDir=$NPOLWORKDIR
export RawDataDir=$NPOLDIR/root
export HistoOutputDir=$NPOLWORKDIR/histos
export HistoInputDir=$NPOLWORKDIR/histos
export COPYTODIR=/home/tireman/output/TargetTaggerBiased/4.4GeV/4Bdl/LongRuns/Location_1/Lead$Lead\cm

if [ ! -e $NPOLDIR ]
then
	mkdir $NPOLDIR
fi
if [ ! -e $NPOLWORKDIR ]
then
	mkdir $NPOLWORKDIR
fi
if [ ! -e $NPOLDIR/Output ]
then
	mkdir $NPOLDIR/Output
fi
if [ ! -e $NPOLWORKDIR/Output ]
then
	mkdir $NPOLWORKDIR/Output
fi
if [ ! -e $NPOLDIR/Plots ]
then
	mkdir $NPOLDIR/Plots
fi
if [ ! -e $NPOLWORKDIR/Plots ]
then
	mkdir $NPOLWORKDIR/Plots
fi 
if [ ! -e $NPOLDIR/histos ]
then
	mkdir $NPOLDIR/histos
fi
if [ ! -e $NPOLWORKDIR/histos ]
then
	mkdir $NPOLWORKDIR/histos
fi

