#!/bin/bash
#
#$ -S /bin/bash
#$ -t 2-200
#$ -cwd

export BUILD_DIR=/home/tireman/simulation/jlab/nmu-npol/build
export PType=Neutron

source $BUILD_DIR/simulation/scripts/NMUCluster/ClusterRunSetup.sh

export JOBNUMBER=$SGE_TASK_ID
export SEED=$SGE_TASK_ID


echo "Starting up Job Number $JOBNUMBER."
$BUILD_DIR/simulation/Npolapp $BUILD_DIR/simulation/macros/Run4.4GeV/QE$PType.mac

/share/apps/root/6.09.02/bin/hadd -f -k /scratch/root/$NPOLBASENAME\_$SGE_TASK_ID.root /scratch/root/$NPOLBASENAME\_$SGE_TASK_ID\_*.root

rm /scratch/root/$NPOLBASENAME\_$SGE_TASK_ID\_*.root

if [ -e $NPOLDIR/root/$NPOLBASENAME\_$SGE_TASK_ID.root ]
then
    
	export CHARGE_TYPE=All
	export HistoOutputDir=$NPOLWORKDIR/$CHARGE_TYPE\Particles/histos
	$BUILD_DIR/analysis2/NpolProcessEvents
	mv $HistoOutputDir/$NPOLBASENAME\_Histos_$CHARGE_TYPE\_$SGE_TASK_ID.root $COPYTODIR/$CHARGE_TYPE\Particles/histos

	export HistoOutputDir=$NPOLWORKDIR/histos
	$BUILD_DIR/analysis/NpolAnalysis
	mv $HistoOutputDir/$NPOLBASENAME\_NpolEff_$SGE_TASK_ID.root $COPYTODIR/histos

	export CHARGE_TYPE=Neutral
	export HistoOutputDir=$NPOLWORKDIR/$CHARGE_TYPE\Particles/histos
	$BUILD_DIR/analysis2/NpolProcessEvents
	mv $HistoOutputDir/$NPOLBASENAME\_Histos_$CHARGE_TYPE\_$SGE_TASK_ID.root $COPYTODIR/$CHARGE_TYPE\Particles/histos

	export CHARGE_TYPE=Charged
	export HistoOutputDir=$NPOLWORKDIR/$CHARGE_TYPE\Particles/histos
	$BUILD_DIR/analysis2/NpolProcessEvents
	mv $HistoOutputDir/$NPOLBASENAME\_Histos_$CHARGE_TYPE\_$SGE_TASK_ID.root $COPYTODIR/$CHARGE_TYPE\Particles/histos

	rm /scratch/root/$NPOLBASENAME\_$SGE_TASK_ID.root
	
fi

