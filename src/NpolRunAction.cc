// %% NpolRunAction.cc %%

// Daniel Wilbern, dwilbern@nmu.edu  September 2014
// Geant4 example B4a used for reference

#include <cstdio> // for sprintf

#include "G4Run.hh"
#include "G4SystemOfUnits.hh"
#include "G4Timer.hh"

#include "NpolRunAction.hh"
#include "NpolHistogramManager.hh"
#include "NpolAnalysis.hh"

NpolRunAction::NpolRunAction() {

	G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
	NpolHistogramManager *histoManager = NpolHistogramManager::GetInstance();

	G4cout << "Using " << analysisManager->GetType() << " Analysis Manager." << G4endl;

	analysisManager->SetHistoDirectoryName("histograms");
	analysisManager->SetVerboseLevel(1);
	analysisManager->SetFirstHistoId(1);

	histoManager->CreateHistograms();

	// Setup the run timer
	runTimer = new G4Timer();
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

		analysisManager->CreateH1(name, title, 100, 0., 120*MeV);
	}

	// TopArray2
	for(i=0; i<28; i++) {
	
		sprintf(name,"TopDet%02d",26+i);
		sprintf(title,"Top Array %d: Detector %02d", i/14 + 3, i%14 + 1);
	
		analysisManager->CreateH1(name, title, 100, 0., 120*MeV);
	}

	// TopVetoArray1
	for(i=0; i<13; i++) {
	
		sprintf(name,"TopVeto%02d",i);
		sprintf(title,"Top Veto Array %d: Detector %02d", 1, i%13 + 1);
	
		analysisManager->CreateH1(name, title, 100, 0., 120*MeV);
	}

	// TopVetoArray2
	for(i=0; i<14; i++) {
	
		sprintf(name,"TopVeto%02d",13+i);
		sprintf(title,"Top Veto Array %d: Detector %02d", 2, i%14 + 1);
	
		analysisManager->CreateH1(name, title, 100, 0., 120*MeV);
	}
	
	// BottomArray1
	for(i=0; i<26; i++) {
	
		sprintf(name,"BottomDet%02d",i);
		sprintf(title,"Bottom Array %d: Detector %02d", i/13 + 1, i%13 + 1);
	
		analysisManager->CreateH1(name, title, 100, 0., 120*MeV);
	}
	
	// BottomArray2
	for(i=0; i<28; i++) {
	
		sprintf(name,"BottomDet%02d",26+i);
		sprintf(title,"Bottom Array %d: Detector %02d", i/14 + 3, i%14 + 1);
	
		analysisManager->CreateH1(name, title, 100, 0., 120*MeV);
	}

	// BottomVetoArray1
	for(i=0; i<13; i++) {
	
		sprintf(name,"BottomVeto%02d",i);
		sprintf(title,"Bottom Veto Array %d: Detector %02d", 1, i%13 + 1);
	
		analysisManager->CreateH1(name, title, 100, 0., 120*MeV);
	}

	// BottomVetoArray2
	for(i=0; i<14; i++) {
	
		sprintf(name,"BottomVeto%02d",13+i);
		sprintf(title,"Bottom Veto Array %d: Detector %02d", 2, i%14 + 1);
	
		analysisManager->CreateH1(name, title, 100, 0., 120*MeV);
	}

	// FrontArray1
	for(i=0; i<12; i++) {
	
		sprintf(name,"FrontDet%02d",i);
		sprintf(title,"Front Array %d: Detector %02d", i/6 + 1, i%6 + 1);
	
		analysisManager->CreateH1(name, title, 100, 0., 120*MeV);
	}

	// FrontArray2
	for(i=0; i<16; i++) {
	
		sprintf(name,"FrontDet%02d",26+i);
		sprintf(title,"Front Array %d: Detector %02d", i/8 + 3, i%8 + 1);
	
		analysisManager->CreateH1(name, title, 100, 0., 120*MeV);
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

	// Create dE/E histograms
	analysisManager->CreateH2("TopdEoverE", "Top Array dE/E", 480, 0., 120.*MeV, 80, 0., 20.*MeV);
	analysisManager->CreateH2("BottomdEoverE", "Bottom Array dE/E", 480, 0., 120.*MeV, 80, 0., 20.*MeV);
	analysisManager->CreateH2("CombineddEoverE", "Top and Bottom Arrays dE/E", 480, 0., 120.*MeV, 80, 0., 20.*MeV);
}

NpolRunAction::~NpolRunAction() {

	delete G4AnalysisManager::Instance();
}

void NpolRunAction::BeginOfRunAction(const G4Run* aRun) {

	G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

	analysisManager->OpenFile("npol");
	runTimer->Start(); //Start the timer

	G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
}

void NpolRunAction::EndOfRunAction(const G4Run*) {

	G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

	analysisManager->Write();
	analysisManager->CloseFile();

	runTimer->Stop(); // Stop the timer
	G4cout << "Run Time: " << *(runTimer) << G4endl;
}

