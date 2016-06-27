#! /bin/bash
#
#$ -V
#$ -t 1502-2000
#$ -cwd

export BUILD_DIR=/home/tireman/simulation/jlab/nmu-npol/build
export NPOLBASENAME=npol_Lead0cm_4.4GeV_0Bdl
export NPOLEVENTSPERFILE=100000
export NPOLDIR=/scratch
export JOBNUMBER=$SGE_TASK_ID
export SEED=$SGE_TASK_ID


if [ ! -e $NPOLDIR ]
then
	mkdir $NPOLDIR
fi
if [ ! -e $NPOLDIR/root ]
then
	mkdir $NPOLDIR/root
fi

echo "Starting up Job Number $JOBNUMBER."
$BUILD_DIR/Npolapp $BUILD_DIR/npol.mac

/share/apps/root/bin/hadd -f -k /scratch/root/npol_Lead0cm_4.4GeV_0Bdl_$SGE_TASK_ID.root /scratch/root/npol_Lead0cm_4.4GeV_0Bdl_$SGE_TASK_ID\_*.root

mv /scratch/root/npol_Lead0cm_4.4GeV_0Bdl_$SGE_TASK_ID.root /home/tireman/output/electron/4.4GeV/0Bdl/Lead0cm/root

rm /scratch/root/npol_Lead0cm_4.4GeV_0Bdl_$SGE_TASK_ID\_*.root
