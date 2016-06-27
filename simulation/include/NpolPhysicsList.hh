//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************

// $Id: NpolPhysicsList.kk  2016-06-16 Will Tireman $

/// \file NpolPhysicsList.hh
/// \brief Definition of the NpolPhysicsList class

#ifndef NpolPhysicsList_h
#define NpolPhysicsList_h 1

#include "G4VModularPhysicsList.hh"

/// Modular physics list
///
/// It includes the folowing physics builders
// to be added to ... 

class NpolPhysicsList: public G4VModularPhysicsList
{
public:
  NpolPhysicsList();
  virtual ~NpolPhysicsList();

  virtual void SetCuts();
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
