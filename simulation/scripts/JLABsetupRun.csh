#!/bin/tsch

setenv Lead 15
setenv Energy 4.4
setenv Bfield 4

setenv NPOLLIB_DIR $BUILD_DIR/../npollib
setenv NPOLBASENAME sourceNeutron_Lead$Lead\cm_$Energy\GeV_$Bfield\Bdl
setenv NPOLDIR /volatile/hallc/cgen/tireman/Summer2016Run/NpolTaggerSource/$Energy\GeV/$Bfield\Bdl/NeutronOnly/15cm
setenv NPOLWORKDIR /volatile/hallc/cgen/tireman/Summer2016Run/NpolTaggerSource/$Energy\GeV/$Bfield\Bdl/NeutronOnly/15cm
setenv NPOLEVENTSPERFILE 10000

setenv RawDataDir $NPOLDIR/root
setenv OutputDir $NPOLWORKDIR/Output
setenv HistoInputDir $NPOLWORKDIR/histos
setenv HistoOutputDir $NPOLWORKDIR/histos
setenv WorkInputDir $NPOLWORKDIR
setenv WorkOutputDir $NPOLWORKDIR

if ( ! -e $NPOLDIR/root ) then
	mkdir $NPOLDIR/root
endif

if ( ! -e $NPOLWORKDIR/Plots ) then
	mkdir $NPOLWORKDIR/Plots
endif

if ( ! -e $NPOLWORKDIR/Output ) then
	mkdir $NPOLWORKDIR/Output
endif

if ( ! -e $NPOLWORKDIR/histos ) then
	mkdir $NPOLWORKDIR/histos
endif
