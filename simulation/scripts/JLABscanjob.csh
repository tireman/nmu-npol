#!/bin/csh

foreach i (`seq $1 1 $2`)

cat > jsubscan_$i << EOF1
PROJECT: cgen
TRACK : simulation
OS : centos65
JOBNAME : SCAN-4.4GeV_4Bdl_0cm_$i
MAIL: tireman@jlab.org
TIME: 720
MEMORY: 750 MB
COMMAND : source JLABscanRunCommands.csh $i
OTHER_FILES : /u/home/tireman/simulation/e11_12_009/background/nmu-npol/build/scripts/JLABscanRunCommands.csh
EOF1
end

foreach j (`seq $1 1 $2`)

  jsub jsubscan_$j
  sleep 1s

end

rm jsubscan_*
