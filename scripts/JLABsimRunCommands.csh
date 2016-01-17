#!/bin/tsch

setenv BUILD_DIR /home/tireman/simulation/e11_12_009/background/nmu-npol/build
setenv OUTPUT_DIR /volatile/hallc/cgen/tireman/4.4GeV/MagField_4Bdl
cp $BUILD_DIR/scripts/JLABsetuprun.csh .
cp -R $BUILD_DIR/gdml .
cp -R $BUILD_DIR/../include .
cp $BUILD_DIR/../root/NpolClass.so .
cp $BUILD_DIR/../root/NpolStatistics.hh .
cp $BUILD_DIR/../root/NpolTagger.hh . 
cp $BUILD_DIR/../root/NpolVertex.hh .
cp $BUILD_DIR/../root/NpolStep.hh .

source /site/12gev_phys/production.csh
source JLABsetuprun.csh $1

echo "Starting up Job Number $1."	

$BUILD_DIR/Npolapp $BUILD_DIR/npol.mac  

sleep 30s

root -b -q $BUILD_DIR/../root/ProcessElectronsJLAB4Bdl10cm.cxx+

hadd -f -k $OUTPUT_DIR/npolLead10cm_4.4GeV_4Bdl_$1.root $OUTPUT_DIR/npolLead10cm_4.4GeV_4Bdl_$1_*.root

