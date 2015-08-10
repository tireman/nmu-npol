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

#include "NpolAnalysisManager.hh"
#include "NpolSteppingAction.hh"
#include "NpolRunAction.hh"

NpolSteppingAction::NpolSteppingAction(NpolDetectorConstruction* det, NpolEventAction* evt, NpolRunAction* run)
  :detector(det), eventAction(evt), runAction(run) 
{
  runAction = run;
}

NpolSteppingAction::~NpolSteppingAction() 
{
  G4cout<< "Ending NpolSteppingAction" << G4endl;
}

void NpolSteppingAction::UserSteppingAction(const G4Step *aStep) {
  // NpolAnalysisManager *analysisMan = NpolAnalysisManager::GetInstance();
  
  //G4Track *aTrack = aStep->GetTrack();
  //G4StepPoint *preStepPoint = aStep->GetPreStepPoint();	
  
  //G4int parentID = aTrack->GetParentID();
  //G4int stepID = aTrack->GetCurrentStepNumber();
  //G4int trackID = aTrack->GetTrackID();
  
  /*G4int particleID = -1;
  G4String particleName = aTrack->GetDynamicParticle()->GetDefinition()->GetParticleName();
  if(particleName == "e-") particleID = ELECTRON_ID;
  else if(particleName == "e+") particleID = POSITRON_ID;
  else if(particleName == "proton") particleID = PROTON_ID;
  else if(particleName == "neutron") particleID = NEUTRON_ID;
  else if(particleName == "gamma") particleID = GAMMA_ID;
  else if(particleName == "pi-") particleID = PINEG;
  else if(particleName == "pi+") particleID = PIPOS;
  else if(particleName == "pi0") particleID = PINEUTRAL;*/
  
  //  G4float vertexEnergy = aTrack->GetVertexKineticEnergy()/MeV;
  //G4float kineticEnergy = aTrack->GetKineticEnergy()/MeV;
  //G4float depositEnergy = aStep->GetTotalEnergyDeposit()/MeV;
  //G4ThreeVector positionInWorld = preStepPoint->GetPosition()/cm;
  // G4ThreeVector positionInVolume = 
  // preStepPoint->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(positionInWorld);
  
  //G4ThreeVector momentum = preStepPoint->GetMomentum()/MeV;
  
  //G4VPhysicalVolume *volume = preStepPoint->GetPhysicalVolume();
  
  //  G4String matName = preStepPoint->GetMaterial()->GetName();

  // if(volume->GetName() == "EndDump"){
  //  aTrack->SetTrackStatus(fStopAndKill);
  //}else if(preStepPoint->GetMaterial()->GetName().compare("Scint") == 0) {
    //analysisMan->AddEDep(volume,aStep->GetTotalEnergyDeposit());
    
    //   analysisMan->FillNtuple(volume, particleID, parentID, trackID, stepID, depositEnergy, vertexEnergy, kineticEnergy, positionInWorld.x(), positionInWorld.y(), positionInWorld.z(), positionInVolume.x(), positionInVolume.y(), positionInVolume.z(), momentum.x(), momentum.y(), momentum.z());
  //}
}

