#! /bin/bash
#
#$ -V
#$ -t 1-500
#$ -cwd

export Energy=4.4
export Bfield=4
export Lead=15

export BUILD_DIR=/home/tireman/simulation/jlab/nmu-npol/build
export NPOLLIB_DIR=$BUILD_DIR/npollib
export NPOLBASENAME=sourceNeutron_Lead$Lead\cm_$Energy\GeV_$Bfield\Bdl
export NPOLEVENTSPERFILE=100000
export NPOLDIR=/scratch
export NPOLWORKDIR=/scratch
export OutputDir=$NPOLDIR/Output
export InputDir=$NPOLDIR/root
export WorkOutputDir=$NPOLWORKDIR
export WorkInputDir=$NPOLWORKDIR
export RawDataDir=$NPOLDIR/root
export HistoOutputDir=$NPOLWORKDIR/histos
export HistoInputDir=$NPOLWORKDIR/histos

export JOBNUMBER=$SGE_TASK_ID
export SEED=$SGE_TASK_ID

if [ ! -e $NPOLDIR ]
then
	mkdir $NPOLDIR
fi
if [ ! -e $NPOLWORKDIR ]
then
	mkdir $NPOLWORKDIR
fi
if [ ! -e $NPOLDIR/Output ]
then
	mkdir $NPOLDIR/Output
fi
if [ ! -e $NPOLWORKDIR/Output ]
then
	mkdir $NPOLWORKDIR/Output
fi
if [ ! -e $NPOLDIR/Plots ]
then
	mkdir $NPOLDIR/Plots
fi
if [ ! -e $NPOLWORKDIR/Plots ]
then
	mkdir $NPOLWORKDIR/Plots
fi 
if [ ! -e $NPOLDIR/histos ]
then
	mkdir $NPOLDIR/histos
fi
if [ ! -e $NPOLWORKDIR/histos ]
then
	mkdir $NPOLWORKDIR/histos
fi

echo "Starting up Job Number $JOBNUMBER."
$BUILD_DIR/simulation/Npolapp $BUILD_DIR/simulation/macros/ParticleFlux.mac

/share/apps/root/bin/hadd -f -k /scratch/root/$NPOLBASENAME\_$SGE_TASK_ID.root /scratch/root/$NPOLBASENAME\_$SGE_TASK_ID\_*.root

#mv /scratch/root/$NPOLBASENAME\_$SGE_TASK_ID.root /home/tireman/output/electron/$Energy\GeV/$Bfield\Bdl/Lead$Lead\cm/root

rm /scratch/root/$NPOLBASENAME\_$SGE_TASK_ID\_*.root

$BUILD_DIR/analysis/NpolAnalysis

rm /scratch/root/$NPOLBASENAME\_$SGE_TASK_ID.root
