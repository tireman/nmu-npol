//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************

//%% NpolEventAction.cc %% 

// This is were the event level actions are placed
// Created: Daniel Wilbern November 2014
// Modified: William Tireman December 2014

#include <cstring>

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4RunManager.hh"
#include "G4ios.hh"
#include "G4String.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4SDManager.hh"
#include "G4THitsCollection.hh"
#include "G4HCofThisEvent.hh"

#include "NpolEventAction.hh"
#include "NpolAnalysis.hh"
#include "NpolAnalysisManager.hh"
#include "NpolSensitiveDetector.hh"
#include "NpolHit.hh"

//typedef G4THitsCollection<NpolHit> NpolHitsCollection;

NpolEventAction::NpolEventAction()
{
  // set printing per each 10 events default (can be changed in macro)
  G4RunManager::GetRunManager()->SetPrintProgress(10);     
}

NpolEventAction::~NpolEventAction()
{}

void NpolEventAction::BeginOfEventAction(const G4Event* evt) {
  
  NpolAnalysisManager *analysisMan = NpolAnalysisManager::GetInstance();
  
  analysisMan->PrepareNewEvent(evt->GetEventID());

}

// Basic EndofEventAction method for now.  It includes the "loop" portion
// for processing hits in the collection(s).  It should probably be broken
// up for better readability and easier future processing.

//Currently it just calls the old Ntuple Fill and Histogram Fill commands as the previous SteppingAction did.  This needs changed to a TTree processing method. 

void NpolEventAction::EndOfEventAction(const G4Event* evt) {
  
  //G4int event_id = evt->GetEventID();
  NpolAnalysisManager *analysisMan = NpolAnalysisManager::GetInstance();
  G4SDManager *SDMan = G4SDManager::GetSDMpointer();
  G4HCofThisEvent *HCE = evt->GetHCofThisEvent();
  NpolHitsCollection *hitsCollection = NULL;

  int CHCIDs[6] = {SDMan->GetCollectionID("TopDetHC"),
		   SDMan->GetCollectionID("TopVetoHC"),
		   SDMan->GetCollectionID("BottomDetHC"),
		   SDMan->GetCollectionID("BottomVetoHC"),
		   SDMan->GetCollectionID("FrontDetHC"),
		   SDMan->GetCollectionID("FrontTagHC")};
 
  int i,j;
  int n_hits =0;
  G4VPhysicalVolume *volume;

  for(j = 0; j < 6; j++){
    if (HCE != NULL){
      hitsCollection = (NpolHitsCollection *)(HCE->GetHC(CHCIDs[j]));
    }

    if(hitsCollection != NULL){
      n_hits = hitsCollection->entries();
      for(i = 0; i < n_hits; i++){
	NpolHit *aHit = (*hitsCollection)[i];
	volume = aHit->GetVolumePointer();
	analysisMan->AddEDep(volume, aHit->GetTotalEnergyDeposit());
	analysisMan->FillNtuple(volume, aHit->GetParticleID(), aHit->GetParentID(), aHit->GetTrackID(), aHit->GetStepNumber(),  aHit->GetTotalEnergyDeposit(),  aHit->GetVertexEnergy(),  aHit->GetKineticEnergy()); //, aHit->GetPreStepPos.x()), aHit->GetPreStepPos.y(), aHit->GetPreStepPos.z(), aHit->GetParticleMomentum.x(), aHit->GetParticleMomentum.y(), aHit->GetParticleMomentum.z());
      }
    }
  }
  analysisMan->FillHistograms();
}

