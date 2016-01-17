#!/bin/tcsh

@ NUM = 187889968 + $1

setenv NPOLDIR /volatile/hallc/cgen/tireman/4.4GeV/MagField_4Bdl
setenv NPOLBASENAME npolLead10cm_4.4GeV_4Bdl
setenv JOBNUMBER $1
setenv SEED $NUM

