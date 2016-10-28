#!/bin/tsch

setenv Lead 15
setenv Energy 11
setenv Bfield 4

setenv NPOLLIB_DIR $BUILD_DIR/../npollib
setenv NPOLBASENAME electronBeam_Lead$Lead\cm_$Energy\GeV_$Bfield\Bdl
setenv NPOLDIR /cache/hallc/cgen/simulation/tireman/Summer2016Run/FullGeometry/$Energy\GeV
setenv NPOLWORKDIR /volatile/hallc/cgen/tireman/Summer2016Run/FullGeometry/$Energy\GeV/$Bfield\Bdl/SmallTargetTagger

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
