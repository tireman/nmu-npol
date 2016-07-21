#!/bin/bash

for ((i=$1; i<=$2; i++))
do

    export JOBNUMBER=$i
    root -b -q $BUILD_DIR/../analysis/scripts/ProcessElectrons.cxx+

done
