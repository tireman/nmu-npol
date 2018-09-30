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
#include "TObject.h"

class NpolVertex : public TObject {

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
  double energy;
  bool eMiss;
  int  particleId;
  std::string particle;
  std::string process;
  std::string volume;
  std::vector<int> daughterIds;

  inline NpolVertex() {};
  virtual ~NpolVertex() {};

  ClassDef(NpolVertex, 1);
};

#endif

