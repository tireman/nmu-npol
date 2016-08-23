#!/bin/csh

foreach i (`seq $1 1 $2`)

cat > jsubfile_$i << EOF1
PROJECT: cgen
TRACK : simulation
OS : centos65
JOBNAME : CGEN-Lead15cm_11GeV_4Bdl_$i
MAIL: tireman@jlab.org
TIME: 2000
MEMORY: 1700 MB
COMMAND : source JLABsimRunCommands.csh $i
OTHER_FILES : /u/home/tireman/simulation/e11_12_009/background/nmu-npol/build/simulation/scripts/JLABsimRunCommands.csh
EOF1
end

foreach j (`seq $1 1 $2`)

  jsub jsubfile_$j
  sleep 1s

end

rm jsubfile_*
