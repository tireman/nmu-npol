#!/bin/csh

foreach i (`seq $1 1 $2`)

cat > jsubscan_$i << EOF1
PROJECT: cgen
TRACK : simulation
OS : centos65
JOBNAME : SCAN-Electron_4.4GeV_4Bdl_15cm_$i
MAIL: tireman@jlab.org
TIME: 180
MEMORY: 1200 MB
COMMAND : source JLABscanRunCommands.csh $i
OTHER_FILES : /u/home/tireman/simulation/e11_12_009/background/nmu-npol/build/analysis/scripts/JLABscanRunCommands.csh
EOF1
end

foreach j (`seq $1 1 $2`)

	jsub jsubscan_$j
	rm jsubscan_$j
end

