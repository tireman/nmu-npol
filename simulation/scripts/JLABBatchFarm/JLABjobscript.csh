#!/bin/csh

foreach i (`seq $1 1 $2`)

cat > jsubfile_$i << EOF1
PROJECT: cgen
TRACK : simulation
OS : centos65
JOBNAME : Neutron-NPOL-4.4GeV-4Bdl_$i
MAIL: tireman@jlab.org
TIME: 90
MEMORY: 1800 MB
COMMAND : source JLABsimRunCommands.csh $i
OTHER_FILES : /u/home/tireman/simulation/e11_12_009/background/nmu-npol/build/simulation/scripts/JLABsimRunCommands.csh
EOF1
end

foreach j (`seq $1 1 $2`)

  jsub jsubfile_$j
  sleep 1s
  rm jsubfile_$j

end
