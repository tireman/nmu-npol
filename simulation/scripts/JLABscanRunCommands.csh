#!/bin/tsch

setenv BUILD_DIR /home/tireman/simulation/e11_12_009/background/nmu-npol/build
setenv INPUT_DIR /volatile/hallc/cgen/tireman/4.4GeV/MagField_4Bdl/Lead0cm
setenv OUTPUT_DIR /volatile/hallc/cgen/tireman/4.4GeV/MagField_4Bdl/Lead0cm
setenv DATA_DIR /volatile/hallc/cgen/tireman/4.4GeV/MagField_4Bdl/Lead0cm 

cp $BUILD_DIR/../root/NpolClass.so .
cp $BUILD_DIR/../include/NpolStatistics.hh .
cp $BUILD_DIR/../include/NpolTagger.hh . 
cp $BUILD_DIR/../include/NpolVertex.hh .
cp $BUILD_DIR/../include/NpolStep.hh .

source /site/12gev_phys/production.csh

@ NUM1 = ( $1 - 1 ) * 50 + 1
@ NUM2 = $1 * 50

foreach j (`seq $NUM1 1 $NUM2`)
  
  setenv JOBNUMBER $j

     echo "    Processing Job Number $j"
     root -b -q $BUILD_DIR/../root/ProcessElectronsJLAB4Bdl0cm.cxx+

  echo "Done processing job $j"
