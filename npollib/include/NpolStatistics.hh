//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************

#ifndef Npol_Statistics_h
#define Npol_Statistics_h

#include "TObject.h"

class NpolStatistics: public TObject {

public:
  int version;
  int totalEvents;
  int eventsSaved;
  
  inline NpolStatistics() {};
  virtual ~NpolStatistics() {};
  
  ClassDef(NpolStatistics, 1);
};

#endif

