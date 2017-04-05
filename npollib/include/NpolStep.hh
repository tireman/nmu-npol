//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************

#ifndef Npol_Step_h
#define Npol_Step_h

#include <string>
#include "TObject.h"

class NpolStep : public TObject {

public:
  int trackId;
  int parentId;
  double gPosX;
  double gPosY;
  double gPosZ;
  double lPosX;
  double lPosY;
  double lPosZ;
  double momX;
  double momY;
  double momZ;
  double time;
  double eDep;
  int particleId;
  std::string volume;

  inline NpolStep() {};
  virtual ~NpolStep() {};

  ClassDef(NpolStep, 1);
};

#endif

