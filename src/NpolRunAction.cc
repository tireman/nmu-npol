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
#include "G4Timer.hh"
#include "G4ios.hh"
 
#include "NpolRunAction.hh"
#include "NpolAnalysisManager.hh"
#include "NpolAnalysis.hh"
#include "NpolRun.hh"

NpolRunAction::NpolRunAction() {

G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
NpolAnalysisManager *analysisMan = NpolAnalysisManager::GetInstance();

G4cout << "Using " << analysisManager->GetType() << " Analysis Manager." << G4endl;

analysisManager->SetHistoDirectoryName("histograms");
analysisManager->SetNtupleDirectoryName("ntuples");
analysisManager->SetVerboseLevel(1);
analysisManager->SetFirstHistoId(1);

analysisMan->CreateNtuple();
analysisMan->CreateHistograms();

// Setup the run timer
runTimer = new G4Timer();
}

NpolRunAction::~NpolRunAction() {
if(theTFile) delete theTFile;
delete G4AnalysisManager::Instance();
}

G4Run* NpolRunAction::GenerateRun()
{
  G4cout << "Generate Run" << G4endl;
 
  NpolRun* run = new NpolRun;

  return run;
}

void NpolRunAction::BeginOfRunAction(const G4Run* aRun) {

G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

analysisManager->OpenFile();


G4cout << "\n----> Open rootfile .. " << G4endl;
theTFile = new TFile("TestRoot.root", "RECREATE", "Example");
tree = new TTree("T","Tree for Npol simulation");

NpolRun* RunData = (NpolRun*) aRun;
tree->Branch("Run Branch",RunData->GetNtuplePointer(),"vertex/I:event/I");
RunData->SetTree(tree);

runTimer->Start();

G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
}

void NpolRunAction::EndOfRunAction(const G4Run*) {
  
  G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
  NpolAnalysisManager *analysisMan = NpolAnalysisManager::GetInstance();
  
analysisManager->Write();
analysisManager->CloseFile();

if (theTFile)
  {
theTFile -> Write(); 
      theTFile -> Close();
}
theTFile = 0;

analysisMan->WriteDetectorIDsToFile();

runTimer->Stop();
G4cout << "Run Time: " << *(runTimer) << G4endl;
}

