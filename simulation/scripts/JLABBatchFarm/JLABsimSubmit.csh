#!/bin/csh

foreach i (`seq $1 1 $2`)

cat > jsubfile_SIM_$3_$i << EOF1
PROJECT: cgen
TRACK : simulation
OS : centos7
JOBNAME : TargetTagger_SIM$3_$i
MAIL: tireman@jlab.org
TIME: 360
MEMORY: 1700 MB
COMMAND : source JLABsimRunCommands.csh $i $3
OTHER_FILES : /u/home/tireman/simulation/e11_12_009/background/nmu-npol/build/simulation/scripts/JLABBatchFarm/JLABsimRunCommands.csh
EOF1
end

foreach j (`seq $1 1 $2`)

  jsub jsubfile_SIM_$3_$j
  rm jsubfile_SIM_$3_$j

end
