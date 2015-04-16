//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//******************************************************************

// %% NpolRunAction.cc %%

// Daniel Wilbern, dwilbern@nmu.edu  September 2014
// Geant4 example B4a used for reference

#include <cstdio> // for sprintf

#include "G4Run.hh"
#include "G4SystemOfUnits.hh"
#include "G4Timer.hh"

#include "NpolRunAction.hh"
#include "NpolDataStructure.hh"
#include "NpolAnalysis.hh"

NpolRunAction::NpolRunAction() {

	G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
	NpolDataStructure *dataStructure = NpolDataStructure::GetInstance();

	G4cout << "Using " << analysisManager->GetType() << " Analysis Manager." << G4endl;

	analysisManager->SetHistoDirectoryName("histograms");
	analysisManager->SetNtupleDirectoryName("ntuples");
	analysisManager->SetVerboseLevel(1);
	analysisManager->SetFirstHistoId(1);

	dataStructure->CreateNtuple();
	dataStructure->CreateHistograms();

	// Setup the run timer
	runTimer = new G4Timer();
}

NpolRunAction::~NpolRunAction() {
	delete G4AnalysisManager::Instance();
}

void NpolRunAction::BeginOfRunAction(const G4Run* aRun) {

	G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

	analysisManager->OpenFile();
	runTimer->Start();

	G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
}

void NpolRunAction::EndOfRunAction(const G4Run*) {

	G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
	NpolDataStructure *dataStructure = NpolDataStructure::GetInstance();

	analysisManager->Write();
	analysisManager->CloseFile();

	dataStructure->WriteDetectorIDsToFile();

	runTimer->Stop();
	G4cout << "Run Time: " << *(runTimer) << G4endl;
}

