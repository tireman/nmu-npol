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

#include "NpolDataStructure.hh"
#include "NpolSteppingAction.hh"

NpolSteppingAction::NpolSteppingAction() {}

NpolSteppingAction::~NpolSteppingAction() {}

void NpolSteppingAction::UserSteppingAction(const G4Step *aStep) {
  NpolDataStructure *dataStructure = NpolDataStructure::GetInstance();
  
  G4Track *aTrack = aStep->GetTrack();
  G4StepPoint *preStepPoint = aStep->GetPreStepPoint();	
  
  G4int parentID = aTrack->GetParentID();
  
  G4int particleID = -1;
  G4String particleName = aTrack->GetDynamicParticle()->GetDefinition()->GetParticleName();
  if(particleName == "e-") particleID = ELECTRON_ID;
  else if(particleName == "e+") particleID = POSITRON_ID;
  else if(particleName == "proton") particleID = PROTON_ID;
  else if(particleName == "neutron") particleID = NEUTRON_ID;
  else if(particleName == "gamma") particleID = GAMMA_ID;
  
  G4double vertexEnergy = aTrack->GetVertexKineticEnergy();
  
  G4ThreeVector positionInWorld = preStepPoint->GetPosition();
  G4ThreeVector positionInVolume = 
    preStepPoint->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(positionInWorld);
  
  G4String volName = preStepPoint->GetPhysicalVolume()->GetName();
  
  G4String matName = preStepPoint->GetMaterial()->GetName();
  if(volName == "EndDump"){
    aTrack->SetTrackStatus(fStopAndKill);
  }else if (matName == "Scint"){
    dataStructure->AddEDep(preStepPoint->GetPhysicalVolume(),
       aStep->GetTotalEnergyDeposit());
    
    dataStructure->FillNtuple(volName, particleID, parentID, 
       vertexEnergy, positionInWorld.x(), positionInWorld.y(), 
       positionInWorld.z());
  }
}

