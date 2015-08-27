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
#include "NpolPrimaryGeneratorActionPS.hh"
#include "NpolPrimaryGeneratorActionFS.hh"
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
 
  // Three sources to choose from. First is just a beam down the beamline
  // Second (*ActionPS) is an attempt at a points source with spherical 
  // distribution of momentum vectors
  // Third (*ActionFS) is a flat source that generates a random point 
  // in the xz plane and rotates it and then fires the particle at a 
  // vector relative to z-axis (NpolAng) all parallel.
  
  SetUserAction(new NpolPrimaryGeneratorAction);
  //SetUserAction(new NpolPrimaryGeneratorActionPS);
  //SetUserAction(new NpolPrimaryGeneratorActionFS);
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

