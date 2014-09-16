   //********************************************************************
   //* License and Disclaimer: From GEANT Collaboration                 *
   //*                                                                  *
   //* The  Geant4 software  is  copyright of the Copyright Holders  of *
   //* the Geant4 Collaboration.  It is provided  under  the terms  and *
   //* conditions of the Geant4 Software License,  included in the file *
   //* LICENSE and available at  http://cern.ch/geant4/license .  These *
   //* include a list of copyright holders.     		      	*
   //********************************************************************
   //* The Geant4 software is used by the Northern Michigan University  *
   //* in accordance to the Geant4 software license specified above.    *
   //* The NMU Collaboration does not hold any rights to this software  *
   //* and gives full permission for its use to others under the limits *
   //* imposed by the GEANT4 Collaboration.  The NMU Collaboration      *
   //* gives no express or implied warranty and use of our code is at   *
   //* the users discretion only.  		    			*
   //********************************************************************

// Daniel Wilbern, dwilbern@nmu.edu
// Geant4 example B4a used for reference

#include <cstdio> // for sprintf

#include "G4Run.hh"
#include "G4SystemOfUnits.hh"

#include "NpolRunAction.hh"
#include "NpolAnalysis.hh"

NpolRunAction::NpolRunAction() {

	G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

	G4cout << "Using " << analysisManager->GetType() << " Analysis Manager." << G4endl;

	analysisManager->SetHistoDirectoryName("histograms");
	analysisManager->SetVerboseLevel(1);

	CreateHistograms();
}


void NpolRunAction::CreateHistograms() {
	
	int i;
	char name[13];
	char title[34];
	G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

	analysisManager->SetFirstHistoId(1);

	// TopArray1
	for(i=0; i<26; i++) {

		sprintf(name,"TopDet%02d",i);
		sprintf(title,"Top Array %d: Detector %02d", i/13 + 1, i%13 + 1);

		analysisManager->CreateH1(name, title, 100, 0., 100*MeV);
	}

	// TopArray2
	for(i=0; i<28; i++) {
	
		sprintf(name,"TopDet%02d",26+i);
		sprintf(title,"Top Array %d: Detector %02d", i/14 + 3, i%14 + 1);
	
		analysisManager->CreateH1(name, title, 100, 0., 100*MeV);
	}

	// TopVetoArray1
	for(i=0; i<13; i++) {
	
		sprintf(name,"TopVeto%02d",i);
		sprintf(title,"Top Veto Array %d: Detector %02d", 1, i%13 + 1);
	
		analysisManager->CreateH1(name, title, 100, 0., 100*MeV);
	}

	// TopVetoArray2
	for(i=0; i<14; i++) {
	
		sprintf(name,"TopVeto%02d",13+i);
		sprintf(title,"Top Veto Array %d: Detector %02d", 2, i%14 + 1);
	
		analysisManager->CreateH1(name, title, 100, 0., 100*MeV);
	}
	
	// BottomArray1
	for(i=0; i<26; i++) {
	
		sprintf(name,"BottomDet%02d",i);
		sprintf(title,"Bottom Array %d: Detector %02d", i/13 + 1, i%13 + 1);
	
		analysisManager->CreateH1(name, title, 100, 0., 100*MeV);
	}
	
	// BottomArray2
	for(i=0; i<28; i++) {
	
		sprintf(name,"BottomDet%02d",26+i);
		sprintf(title,"Bottom Array %d: Detector %02d", i/14 + 3, i%14 + 1);
	
		analysisManager->CreateH1(name, title, 100, 0., 100*MeV);
	}

	// BottomVetoArray1
	for(i=0; i<13; i++) {
	
		sprintf(name,"BottomVeto%02d",i);
		sprintf(title,"Bottom Veto Array %d: Detector %02d", 1, i%13 + 1);
	
		analysisManager->CreateH1(name, title, 100, 0., 100*MeV);
	}

	// BottomVetoArray2
	for(i=0; i<14; i++) {
	
		sprintf(name,"BottomVeto%02d",13+i);
		sprintf(title,"Bottom Veto Array %d: Detector %02d", 2, i%14 + 1);
	
		analysisManager->CreateH1(name, title, 100, 0., 100*MeV);
	}

	// FrontArray1
	for(i=0; i<12; i++) {
	
		sprintf(name,"FrontDet%02d",i);
		sprintf(title,"Front Array %d: Detector %02d", i/6 + 1, i%6 + 1);
	
		analysisManager->CreateH1(name, title, 100, 0., 1*MeV);
	}

	// FrontArray2
	for(i=0; i<16; i++) {
	
		sprintf(name,"FrontDet%02d",26+i);
		sprintf(title,"Front Array %d: Detector %02d", i/8 + 3, i%8 + 1);
	
		analysisManager->CreateH1(name, title, 100, 0., 1*MeV);
	}

	// FrontTagger1
	for(i=0; i<12; i++) {
	
		sprintf(name,"FrontTag%02d",26+i);
		sprintf(title,"Front Tagger Array %d: Detector %02d", i/6 + 1, i%6 + 1);
	
		analysisManager->CreateH1(name, title, 100, 0., 100*MeV);
	}

	// FrontTagger2
	for(i=0; i<16; i++) {
	
		sprintf(name,"FrontTag%02d",26+i);
		sprintf(title,"Front Tagger Array %d: Detector %02d", i/8 + 3, i%8 + 1);
	
		analysisManager->CreateH1(name, title, 100, 0., 100*MeV);
	}
}

NpolRunAction::~NpolRunAction() {

	delete G4AnalysisManager::Instance();
}

void NpolRunAction::BeginOfRunAction(const G4Run* aRun) {

	G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

	analysisManager->OpenFile("npol");

	G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
}

void NpolRunAction::EndOfRunAction(const G4Run*) {

	G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

	analysisManager->Write();
	analysisManager->CloseFile();

}

