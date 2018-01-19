#!/bin/tsch

setenv BUILD_DIR /home/tireman/simulation/e11_12_009/background/nmu-npol/build/simulation
setenv pType $2
source $BUILD_DIR/../../analysis/envscripts/JLABsetupAnalysis.csh

setenv JOBNUMBER $1

cp -R $BUILD_DIR/gdml .
cp -R $BUILD_DIR/macros .
cp -R $BUILD_DIR/../../npollib/include/*.hh .
cp $BUILD_DIR/../npollib/libNpolClasses.so .
cp $BUILD_DIR/../../simulation/include/*.hh .

source /site/12gev_phys/production.csh 2.0

echo "Starting up Job Number $1."	

# These 2 lines combine multi-files together to one file and remove the original
hadd -f -k $NPOLDIR/root/$NPOLBASENAME\_$1.root $NPOLDIR/root/$NPOLBASENAME\_$1\_*.root
rm $NPOLDIR/root/$NPOLBASENAME\_$1\_*.root

if ( -e $NPOLDIR/root/$NPOLBASENAME\_$1.root ) then

	# This runs the NPOL efficiency code
	$BUILD_DIR/../analysis/NpolAnalysis

	# The next 3 run the NPOL Process events code for neutral/charged particles types
	setenv CHARGE_TYPE All
	setenv HistoOutputDir $NPOLWORKDIR/$CHARGE_TYPE\Particles/histos
	$BUILD_DIR/../analysis2/NpolProcessEvents

	setenv CHARGE_TYPE Neutral
	setenv HistoOutputDir $NPOLWORKDIR/$CHARGE_TYPE\Particles/histos
	$BUILD_DIR/../analysis2/NpolProcessEvents

	setenv CHARGE_TYPE Charged
	setenv HistoOutputDir $NPOLWORKDIR/$CHARGE_TYPE\Particles/histos
	$BUILD_DIR/../analysis2/NpolProcessEvents

	# Uncomment this line if you DO NOT want to keep the raw ROOT file after run #
	#rm $NPOLDIR/root/$NPOLBASENAME\_$1.root  
	
endif 

