#!/bin/tsch

setenv BUILD_DIR /home/tireman/simulation/e11_12_009/background/nmu-npol/build/simulation
setenv NPOLDIR /volatile/hallc/cgen/tireman/Summer2016Run/FullGeometry/11GeV
setenv NPOLBASENAME electronBeam_Lead15cm_11GeV_4Bdl
setenv JOBNUMBER $1

cp -R $BUILD_DIR/gdml .
cp -R $BUILD_DIR/macros .
cp -R $BUILD_DIR/../../simulation/include .
cp $BUILD_DIR/../../npollib/libNpolClasses.so .
cp $BUILD_DIR/../../simulation/include/*.hh 

source /site/12gev_phys/production.csh

echo "Starting up Job Number $1."	

$BUILD_DIR/Npolapp $BUILD_DIR/macros/ElectronBeam.mac  

