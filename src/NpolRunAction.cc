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
#include <string>

#include "G4Run.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

//#include "TROOT.h"
//#include "TFile.h"
//#include "TNtuple.h"
//#include "TTree.h"
#include "NpolRunAction.hh"
#include "NpolAnalysisManager.hh"

NpolRunAction::NpolRunAction() {
	// Setup the run timer
	runTimer = new G4Timer();
}

NpolRunAction::~NpolRunAction() {
}

void NpolRunAction::BeginOfRunAction(const G4Run* aRun) {
	runTimer->Start();
	G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
}

void NpolRunAction::EndOfRunAction(const G4Run*) {
	NpolAnalysisManager *analysisMan = NpolAnalysisManager::GetInstance();
	analysisMan->WriteTree();
	analysisMan->CloseFile();

	runTimer->Stop();
	G4cout << "Run Time: " << *(runTimer) << G4endl;
}

