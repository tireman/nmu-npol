#!/bin/tsch

setenv Lead 15
setenv Energy 11
setenv Bfield 4
setenv BUILD_DIR /home/tireman/simulation/e11_12_009/background/nmu-npol/build/simulation
setenv NPOLDIR /volatile/hallc/cgen/tireman/Summer2016Run/FullGeometry/$Energy\GeV
setenv NPOLBASENAME electronBeam_Lead$Lead\cm_$Energy\GeV_$Bfield\dl
setenv JOBNUMBER $1

cp -R $BUILD_DIR/gdml .
cp -R $BUILD_DIR/macros .
cp -R $BUILD_DIR/../../simulation/include .
cp $BUILD_DIR/../../npollib/libNpolClasses.so .
cp $BUILD_DIR/../../simulation/include/*.hh .

source /site/12gev_phys/production.csh 

if ( ! -e $NPOLDIR/root ) then
	mkdir $NPOLDIR/root
endif

echo "Starting up Job Number $1."	

$BUILD_DIR/Npolapp $BUILD_DIR/macros/ParticleFlux.mac  

hadd -f -k $NPOLDIR/root/$NPOLBASENAME\_$1.root $NPOLDIR/root/$NPOLBASENAME\_$1\_*.root

