#!/bin/sh

#export JOBNUMBER=$1
export Lead=15
export Energy=4.4
export Bfield=4
export BUILD_DIR=/home/tireman/simulation/jlab/npol/nmu-npol/build/simulation
export NPOLBASENAME=targetGamma
export NPOLDIR=/data1/cgen/TargetTaggerBiasRuns/shortTest
#export NPOLDIR=/home/tireman/simulation/jlab/npol/analysis/test2
#export NPOLDIR=/data2/cgen/JlabSimData/Summer2016Run/NoNpolArm/4.4GeV

export NPOLWORKDIR=/data1/cgen/TargetTaggerBiasRuns/shortTest
#export NPOLWORKDIR=/data2/cgen/JlabSimData/Summer2016Run/NoNpolArm/4.4GeV

export OutputDir=$NPOLDIR/histos
export InputDir=$NPOLDIR/root
export WorkOutputDir=$NPOLWORKDIR/Plots
export WorkInputDir=$NPOLWORKDIR/histos

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
