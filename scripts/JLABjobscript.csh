#!/bin/csh

for ((i=$1; i<=$2; i++))
do

cat > jsubfile_$i << EOF1
PROJECT: e93038
TRACK : simulation
OS : centos65
JOBNAME : CGEN
MAIL: tireman@jlab.org
TIME: 2880
COMMAND : source JLABsimRunCommands.csh $i
OTHER_FILES : /home/tireman/scripts/JLABsimRunCommands.csh
EOF1
done

for ((j=$1; j<=$2; j++))
do

jsub jsubfile_$j
sleep 2s

done

rm jsubfile_*
