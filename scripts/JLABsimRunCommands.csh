#!/bin/tsch

@ NUM = 187899798 + $1

setenv BUILD_DIR /home/tireman/simulation/e11_12_009/background/nmu-npol/build
setenv NPOLDIR /volatile/hallc/cgen/tireman/4.4GeV/MagField_4Bdl/Lead0cm
setenv NPOLBASENAME npolLead0cm_4.4GeV_4Bdl
setenv JOBNUMBER $1
setenv SEED $NUM

cp $BUILD_DIR/scripts/JLABsetuprun.csh .
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

sleep 30s

root -b -q $BUILD_DIR/../root/ProcessElectronsJLAB4Bdl0cm.cxx+

hadd -f -k $NPOLDIR/npolLead0cm_4.4GeV_4Bdl_$1.root $NPOLDIR/npolLead0cm_4.4GeV_4Bdl_$1_*.root

jput $NPOLDIR/npolLead0cm_4.4GeV_4Bdl_$1.root /mss/hallc/cgen/simulation/tireman/4.4GeV/4Bdl/Lead0cm

rm $NPOLDIR/npolLead0cm_4.4GeV_4Bdl_$1_*.root
