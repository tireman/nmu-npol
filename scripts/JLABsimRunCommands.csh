#!/bin/tsch

setenv BUILD_DIR /home/tireman/simulation/e11_12_009/background/nmu-npol/build
cp $BUILD_DIR/JLABsetuprun.csh .
source /site/12gev_phys/production.csh
source JLABsetuprun.csh $1
rm $BUILD_DIR/out_$1.out
$BUILD_DIR/Npolapp $BUILD_DIR/npol.mac >/$BUILD_DIR/out_$1.out
