#!/bin/sh

export Lead=15
export Energy=4.4
export Bfield=1
export PType=Total
export BUILD_DIR=/home/tireman/simulation/jlab/nmu-npol/build/simulation
export NPOLLIB_DIR=/home/tireman/simulation/jlab/nmu-npol/build/npollib
export NPOLBASENAME=source$PType\_Lead$Lead\cm_$Energy\GeV_$Bfield\Bdl

#electronBeam_Lead$Lead\cm_$Energy\GeV_$Bfield\Bdl 
#source$PType\_Lead$Lead\cm_$Energy\GeV_$Bfield\Bdl 

export NPOLDIR=/home2/tireman-h2/data1/TargetTaggerSource/$Energy\GeV/$Bfield\Bdl/6mSameAcceptance

#/home/tireman/data1/TargetTaggerSource/$Energy\GeV/$Bfield\Bdl
#/home/tireman/data1/JlabSimData/AprilRun2017/4.4GeV/4Bdl
#/home/tireman/data1/TargetTaggerSource/$Energy\GeV/17April2017/$Bfield\Bdl/7meterSameAcceptance
#/home/tireman/data1/TargetTaggerSource/$Energy\GeV/$Bfield\Bdl #/5meterSameAcceptance 
#/home/tireman/data1/JlabSimData/Feb2017_ElectronRunTest/$Energy\GeV/$Bfield\Bdl/70cmWide26cmTallTargetTagger #/ChargedParticles #/5meterSameAcceptance/NeutralParticles #ChargedParticles 
  
export NPOLWORKDIR=/home2/tireman-h2/data1/TargetTaggerSource/$Energy\GeV/$Bfield\Bdl/6mSameAcceptance/ChargedParticles

#/home2/tireman-h2/data1/TargetTaggerSource/$Energy\GeV/$Bfield\Bdl/5mSameAcceptance
#/home/tireman/data1/TargetTaggerSource/$Energy\GeV/$Bfield\Bdl/OnlyDipoleSurface/AllParticles
#/home/tireman/data1/JlabSimData/AprilRun2017/4.4GeV/4Bdl/CutFrontWall/AllParticles #LeadWallOnly #/NeutralParticles #/ChargedParticles

export OutputDir=$NPOLDIR/Output
export InputDir=$NPOLDIR/root
export WorkOutputDir=$NPOLWORKDIR
export WorkInputDir=$NPOLWORKDIR
export RawDataDir=$NPOLDIR/root
export HistoOutputDir=$NPOLWORKDIR/histos
export HistoInputDir=$NPOLWORKDIR/histos

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

