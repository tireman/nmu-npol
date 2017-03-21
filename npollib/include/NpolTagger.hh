//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************

#ifndef Npol_Tagger_h
#define Npol_Tagger_h

#include <string>
#include "TObject.h"

class NpolTagger : public TObject {

public:

  int trackId;
  //int parentId;
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
  double energy;
  int  particleId;
  std::string particle;

  inline NpolTagger() {};
  virtual ~NpolTagger() {};

  ClassDef(NpolTagger, 1);
};

#endif

