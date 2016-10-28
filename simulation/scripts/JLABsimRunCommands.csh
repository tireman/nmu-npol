#!/bin/tsch

setenv BUILD_DIR /home/tireman/simulation/e11_12_009/background/nmu-npol/build/simulation
source $BUILD_DIR/../../simulation/scripts/JLABsetupRun.csh

setenv JOBNUMBER $1

cp -R $BUILD_DIR/gdml .
cp -R $BUILD_DIR/macros .
cp -R $BUILD_DIR/../../simulation/include .
cp -R $BUILD_DIR/../../npollib/include/*.hh .
cp $BUILD_DIR/../npollib/libNpolClasses.so .
cp $BUILD_DIR/../../simulation/include/*.hh .

source /site/12gev_phys/production.csh 
use root/6.06.02

if ( ! -e $NPOLDIR/root ) then
	mkdir $NPOLDIR/root
endif

echo "Starting up Job Number $1."	

$BUILD_DIR/Npolapp $BUILD_DIR/macros/ParticleFlux.mac  

hadd -f -k $NPOLDIR/root/$NPOLBASENAME\_$1.root $NPOLDIR/root/$NPOLBASENAME\_$1\_*.root

rm $NPOLDIR/root/$NPOLBASENAME\_$1\_*.root

#$BUILD_DIR/../analysis/NpolAnalysis
#root -b -q $BUILD_DIR/../../analysis/scripts/ProcessElectrons.cxx+

#rm $NPOLDIR/root/$NPOLBASENAME\_$1.root
