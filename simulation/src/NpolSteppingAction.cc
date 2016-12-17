//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//******************************************************************

// %% NpolSteppingAction.cc %% //

// Created: Daniel Wilbern - November 2014

#include "G4ios.hh"
#include "G4NavigationHistory.hh"
#include "G4Track.hh"
#include "G4Step.hh"

#include "NpolSteppingAction.hh"
#include "NpolAnalysisManager.hh"
#include "NpolRunAction.hh"

NpolSteppingAction::NpolSteppingAction(NpolEventAction* evt, NpolRunAction* run)
  :eventAction(evt), runAction(run) 
{
  runAction = run;
  G4cout << "Firing up Stepping Action!" << G4endl;
}

NpolSteppingAction::~NpolSteppingAction() 
{
  G4cout<< "Shutting Down Stepping Action" << G4endl;
}

void NpolSteppingAction::UserSteppingAction(const G4Step *aStep) {
  NpolAnalysisManager *analysisMan = NpolAnalysisManager::GetInstance();

  G4Track *aTrack = aStep->GetTrack();
  G4StepPoint *preStepPoint = aStep->GetPreStepPoint();	
  G4StepPoint *postStepPoint = aStep->GetPostStepPoint();	
  G4VPhysicalVolume *preStepVolume = preStepPoint->GetPhysicalVolume();
  G4VPhysicalVolume *postStepVolume = postStepPoint->GetPhysicalVolume();
  G4String volName = preStepVolume->GetName();
 
  // Kill tracks that reach certian volumes and won't produce background in 
  // the polarimeter
  if(volName == "Cap" || postStepVolume == NULL ||
     volName == "HallShellRoof" ||volName == "HallShellFloor" || volName == "HallShellWall"
     || volName == "HutBackWall" || volName == "HutSideWall" || volName == "HutRoof" || volName == "HutFrontWall"){
 	analysisMan->TrackKilled(aTrack->GetTrackID());
	aTrack->SetTrackStatus(fStopAndKill);
  }
  
  // All Stepping information is saved to a vector for analysis later
  analysisMan->RecordStep(aStep);
}

