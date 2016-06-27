#!/bin/bash

for ((i=$1; i<=$2; i++))
do

    export JOBNUMBER=$i
    root -b -q $BUILD_DIR/../root/ProcessElectrons.cxx+

done
