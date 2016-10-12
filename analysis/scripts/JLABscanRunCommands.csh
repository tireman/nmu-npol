#!/bin/tsch

setenv BUILD_DIR /home/tireman/simulation/e11_12_009/background/nmu-npol/build/simulation
source $BUILD_DIR/../../analysis/envscripts/JLABsetupAnalysis.csh

cp -R $BUILD_DIR/gdml .
cp -R $BUILD_DIR/macros .
cp -R $BUILD_DIR/../../simulation/include .
cp -R $BUILD_DIR/../../npollib/include/*.hh .
cp $BUILD_DIR/../npollib/libNpolClasses.so .
cp $BUILD_DIR/../../simulation/include/*.hh .

source /site/12gev_phys/production.csh
use root/6.06.02

@ NUM1 = ( $1 - 1 ) * 25 + 1
@ NUM2 = $1 * 25

foreach j (`seq $NUM1 1 $NUM2`)
  
  setenv JOBNUMBER $j

     echo "    Processing Job Number $j"
	 root -b -q $BUILD_DIR/../../analysis/scripts/ProcessElectrons.cxx+
 	 #$BUILD_DIR/build/analysis/NpolAnalysis

  echo "Done processing job $j"

end
