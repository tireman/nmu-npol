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
#include "globals.hh"
#include "G4Types.hh"
#include "G4ios.hh"

class NpolTagger : public TObject {

public:
  G4int trackId;
  //G4int parentId;
  double posX;
  double posY;
  double posZ;
  double momX;
  double momY;
  double momZ;
  double time;
  double energy;
  G4int  particleId;
  std::string particle;
  //std::string process;
  //std::string volume;

  inline NpolTagger() {};
  virtual ~NpolTagger() {};

  ClassDef(NpolTagger, 1);
};

#endif

