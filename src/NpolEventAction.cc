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
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4ios.hh"
#include "G4String.hh"
#include "G4THitsCollection.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4AutoLock.hh"

#include "NpolAnalysisManager.hh"
#include "NpolAnalysis.hh"
#include "NpolEventAction.hh"

typedef G4THitsCollection<NpolHit> NpolHitsCollection;

class NpolHit {};

G4Mutex aMutex = G4MUTEX_INITIALIZER; // Have to use a mutex for calls to strtok()

NpolEventAction::NpolEventAction()
{
	// set printing per each 10 events
	G4RunManager::GetRunManager()->SetPrintProgress(10);     
}

NpolEventAction::~NpolEventAction()
{}

void NpolEventAction::BeginOfEventAction(const G4Event* evt) {

	NpolAnalysisManager *dataStructure = NpolAnalysisManager::GetInstance();

	dataStructure->PrepareNewEvent();
}

void NpolEventAction::EndOfEventAction(const G4Event* evt) {

	NpolAnalysisManager *dataStructure = NpolAnalysisManager::GetInstance();
	G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

	dataStructure->FillHistograms();
}

