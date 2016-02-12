#!/bin/tsch

@ NUM = 187897798 + $1

setenv BUILD_DIR /home/tireman/simulation/e11_12_009/background/nmu-npol/build
setenv NPOLDIR /volatile/hallc/cgen/tireman/4.4GeV/MagField_1Bdl/Lead0cm
setenv NPOLBASENAME npolLead0cm_4.4GeV_1Bdl
setenv JOBNUMBER $1
setenv SEED $NUM

cp -R $BUILD_DIR/gdml .
cp -R $BUILD_DIR/../include .
cp $BUILD_DIR/../root/NpolClass.so .
cp $BUILD_DIR/../root/NpolStatistics.hh .
cp $BUILD_DIR/../root/NpolTagger.hh . 
cp $BUILD_DIR/../root/NpolVertex.hh .
cp $BUILD_DIR/../root/NpolStep.hh .

source /site/12gev_phys/production.csh

echo "Starting up Job Number $1."	

$BUILD_DIR/Npolapp $BUILD_DIR/npol.mac  

