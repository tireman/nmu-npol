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
  
  inline NpolDetectorEvent() { }
  virtual ~NpolDetectorEvent() { }

  double gPosX;
  double gPosY;
  double gPosZ;
  double lPosX;
  double lPosY;
  double lPosZ;
  double time;
  double totEnergyDep;
  bool thresholdExceeded;

  ClassDef(NpolDetectorEvent, 1);
};

#endif

