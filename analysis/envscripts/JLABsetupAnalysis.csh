#!/bin/tsch

setenv Lead 10
setenv Energy 4.4
setenv Bfield 4
setenv BUILD_DIR /home/tireman/simulation/e11_12_009/background/nmu-npol/root
setenv NPOLBASENAME npol
setenv NPOLDIR /volatile/hallc/cgen/tireman/4.4GeV/MagField_4Bdl/Lead10cm
setenv NPOLWORKDIR /work/hallc/cgen/tireman/MagFieldOn/MagField_4Bdl/LeadOn10cm

setenv OutputDir $NPOLWORKDIR/Output
setenv InputDir $NPOLDIR/root
setenv WorkInputDir $NPOLWORKDIR/root
setenv WorkOutputDir $NPOLWORKDIR/Plots

if ( ! -e $NPOLWORKDIR/Plots ) then
	mkdir $NPOLWORKDIR/Plots
endif

if ( ! -e $NPOLWORKDIR/Output ) then
	mkdir $NPOLWORKDIR/Output
endif

