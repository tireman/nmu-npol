//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.							 *
//********************************************************************

#ifndef Npol_Stepping_Action_h
#define Npol_Stepping_Action_h

#include "G4UserSteppingAction.hh"

class G4Step;
class NpolDetectorConstruction;
class NpolEventAction;
class NpolRunAction;

class NpolSteppingAction : public G4UserSteppingAction {

public:
  NpolSteppingAction(NpolDetectorConstruction*, NpolEventAction*, NpolRunAction*);
  ~NpolSteppingAction();
  
  virtual void UserSteppingAction(const G4Step *aStep);
  
private:
  NpolDetectorConstruction* detector;
  NpolEventAction* eventAction;
  NpolRunAction* runAction;
};

#endif

