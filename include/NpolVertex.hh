//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************

#ifndef Npol_Vertex_h
#define Npol_Vertex_h

#include <string>
#include <vector>

class NpolVertex {

public:
  G4int trackId;
  G4int parentId;
  double posX;
  double posY;
  double posZ;
  double momX;
  double momY;
  double momZ;
  double time;
  double energy;
  bool eMiss;
  G4int  particleId;
  std::string particle;
  std::string process;
  std::string volume;
  std::vector<G4int> daughterIds;
};

#endif

