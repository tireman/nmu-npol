//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************

#ifndef Npol_DetectorEvent_h
#define Npol_DetectorEvent_h

#include "TObject.h"

class NpolDetectorEvent : public TObject {

public:
  
  NpolDetectorEvent() {
	gPosX = 0.0;
	gPosY = 0.0;
	gPosZ = 0.0;
	lPosX = 0.0;
	lPosY = 0.0;
	lPosZ = 0.0;
	hPosX = 0.0;
	hPosY = 0.0;
	hPosZ = 0.0;
	time = 0.0;
	thresholdExceeded = false;
	totEnergyDep = 0.0;
  }
  ~NpolDetectorEvent() { }
  
  double gPosX;
  double gPosY;
  double gPosZ;
  double lPosX;
  double lPosY;
  double lPosZ;
  double hPosX;
  double hPosY;
  double hPosZ;
  double time;
  double totEnergyDep;
  bool thresholdExceeded;

  ClassDef(NpolDetectorEvent, 1);
};

#endif

