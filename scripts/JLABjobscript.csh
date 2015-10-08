#!/bin/csh

foreach i (`seq $1 1 $2`)

cat > jsubfile_$i << EOF1
PROJECT: cgen
TRACK : simulation
OS : centos65
JOBNAME : CGEN-sim_$i
MAIL: tireman@jlab.org
TIME: 2880
MEMORY: 1100 MB
COMMAND : source JLABsimRunCommands.csh $i
OTHER_FILES : /u/home/tireman/simulation/e11_12_009/background/nmu-npol/build/scripts/JLABsimRunCommands.csh
EOF1
end

foreach j (`seq $1 1 $2`)

  jsub jsubfile_$j
  sleep 2s

end

rm jsubfile_*
