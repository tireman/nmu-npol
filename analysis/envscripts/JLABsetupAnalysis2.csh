#!/bin/tsch

setenv Lead 15
setenv Energy 4.4
setenv Bfield 4
setenv BUILD_DIR /home/tireman/simulation/e11_12_009/background/nmu-npol
setenv NPOLLIB_DIR $BUILD_DIR/build/npollib
setenv NPOLBASENAME electronBeam
setenv NPOLDIR /cache/hallc/cgen/simulation/tireman/Summer2016Run/FullGeometry/4.4GeV
setenv NPOLWORKDIR /volatile/hallc/cgen/tireman/Summer2016Run/FullGeometry/4.4GeV

setenv RawDataDir $NPOLDIR/root
setenv OutputDir $NPOLWORKDIR/Output
setenv HistoInputDir $NPOLWORKDIR/histos
setenv HistoOutputDir $NPOLWORKDIR/histos
setenv WorkInputDir $NPOLWORKDIR
setenv WorkOutputDir $NPOLWORKDIR

if ( ! -e $NPOLWORKDIR/Plots ) then
	mkdir $NPOLWORKDIR/Plots
endif

if ( ! -e $NPOLWORKDIR/Output ) then
	mkdir $NPOLWORKDIR/Output
endif

if ( ! -e $NPOLWORKDIR/histos ) then
	mkdir $NPOLWORKDIR/histos
endif