#!/bin/tsch

setenv BUILD_DIR /home/tireman/simulation/e11_12_009/background/nmu-npol-tagger/build/simulation
setenv pType $2
source $BUILD_DIR/scripts/JLABBatchFarm/JLABsetupRun.csh

setenv JOBNUMBER $1

cp -R $BUILD_DIR/gdml .
cp -R $BUILD_DIR/macros .
cp -R $BUILD_DIR/../../simulation/include .
cp -R $BUILD_DIR/../../npollib/include/*.hh .
cp $BUILD_DIR/../npollib/libNpolClasses.so .
cp $BUILD_DIR/../../simulation/include/*.hh .

source /site/12gev_phys/production.csh 2.0

echo "Starting up Job Number $1."	

$BUILD_DIR/Npolapp $BUILD_DIR/macros/Run4.4GeV/ParticleFlux$pType\.mac  
source $BUILD_DIR/scripts/JLABBatchFarm/JLABAnalysisRunCommands.csh $1 $2


