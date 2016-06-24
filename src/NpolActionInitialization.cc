//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************

// %% NpolActionInitization.cc  %% 
// It exists to exist to start this stuff

#include "NpolActionInitialization.hh"
#include "NpolPrimaryGeneratorAction.hh"
#include "NpolRunAction.hh"
#include "NpolEventAction.hh"
#include "NpolTrackingAction.hh"
#include "NpolSteppingAction.hh"

#include "G4RunManager.hh"

NpolActionInitialization::NpolActionInitialization()
{}

NpolActionInitialization::~NpolActionInitialization()
{}

void NpolActionInitialization::BuildForMaster() const {
  SetUserAction(new NpolRunAction);
}

void NpolActionInitialization::Build() const {
   
  SetUserAction(new NpolPrimaryGeneratorAction);
  
  NpolEventAction* event_action = new NpolEventAction();
  SetUserAction(event_action);
  NpolRunAction* run_action = new NpolRunAction();
  SetUserAction(run_action);
  NpolTrackingAction *tracking_action = new NpolTrackingAction();
  SetUserAction(tracking_action);
  NpolSteppingAction* step_action = new NpolSteppingAction
    (event_action, run_action);
  SetUserAction(step_action);
}

