#!/bin/bash

source /home/tireman/simulation/jlab/nmu-npol/analysis/envscripts/NMUsetupAnalysis.sh

for ((i=$1; i<=$2; i++))
do
    NUM1=($i-1)*$3+1
    NUM2=$i*$3

    /home/tireman/simulation/jlab/nmu-npol/analysis/scripts/NMUjobSubmit.sh $NUM1 $NUM2 &
 
done

