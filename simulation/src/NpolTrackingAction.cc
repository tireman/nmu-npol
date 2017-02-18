//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************

// The tracking action was creaded to assist with recording tracks.
// Created: Daniel Wilbern September 2015

#include "G4Track.hh"

#include "NpolTrackingAction.hh"
#include "NpolAnalysisManager.hh"

NpolTrackingAction::NpolTrackingAction() {}

NpolTrackingAction::~NpolTrackingAction() {}

void NpolTrackingAction::PreUserTrackingAction(const G4Track *aTrack) {
	NpolAnalysisManager *analysisMan = NpolAnalysisManager::GetInstance();
	analysisMan->RecordTrack(aTrack);
}

void NpolTrackingAction::PostUserTrackingAction(const G4Track *aTrack) {}

