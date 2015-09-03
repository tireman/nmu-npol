//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************

#include <string>

#include <G4Track.hh>
#include <G4ThreeVector.hh>
#include <G4String.hh>
#include <G4ParticleDefinition.hh>
#include <G4VProcess.hh>
#include "G4VVisManager.hh"
#include "G4UIcommand.hh"
#include "G4UImanager.hh"

#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TClassTable.h>
#include <TSystem.h>
#include <TInterpreter.h>

#include "NpolAnalysisManager.hh"
#include "NpolAnalysisMessenger.hh"
#include "NpolVertex.hh"


static NpolAnalysisManager *pInstance = NULL;

NpolAnalysisManager *NpolAnalysisManager::GetInstance() {
  if(pInstance == NULL)
    pInstance = new NpolAnalysisManager();
  
  return pInstance;
}

NpolAnalysisManager::NpolAnalysisManager(){
  analysisMessenger = new NpolAnalysisMessenger(this);
  initialized = false;
  npolOutFile = NULL;
  npolTree = NULL;
  tracksBranch = NULL;
  tracks = NULL;

  Initialize();
}

NpolAnalysisManager::~NpolAnalysisManager() {
  delete npolTree;
  delete npolOutFile;
  delete tracks;
  delete analysisMessenger;
}

void NpolAnalysisManager::Initialize(){
  if(initialized)
    std::cout << "WARNING: NpolAnalysisManager is already initialized and is being initialized again." << std::endl;
  
  if(!TClassTable::GetDict("include/NpolVertex.hh"))
    gSystem->Load("NpolVertex_hh.so");
  
  gInterpreter->GenerateDictionary("vector<NpolVertex *>","include/NpolVertex.hh;vector");

  tracks = new std::vector<NpolVertex *>();
    
  npolTree = new TTree("t_npolTree","Per-event information from Npol simulation");
  npolTree->Branch("tracks_branch","std::vector<NpolVertex *>",&tracks,32000,2);
  initialized = true;
}

void NpolAnalysisManager::BeginOfRun(){
  // Method to open TFile.  Hoping to make this available from macro at
  // some point via the AnalysisMessenger class. -- W.T.
  OpenFile();
}

void NpolAnalysisManager::EndOfRun(){
  WriteTree();
  CloseFile();
  G4cout << "Shutting down the run!" << G4endl;
}

void NpolAnalysisManager::PrepareNewEvent() {
  std::vector<NpolVertex *>::iterator it;
  for(it = tracks->begin(); it != tracks->end(); it++)
    delete *it;
  
  tracks->clear();
}

void NpolAnalysisManager::AddTrack(const G4Track *aTrack) {
  NpolVertex *anNpolVertex = new NpolVertex();
  
  anNpolVertex->trackId = aTrack->GetTrackID();
  anNpolVertex->parentId = aTrack->GetParentID();
  anNpolVertex->posX = (aTrack->GetPosition()).x();
  anNpolVertex->posY = (aTrack->GetPosition()).y();
  anNpolVertex->posZ = (aTrack->GetPosition()).z();
  anNpolVertex->momX = (aTrack->GetMomentum()).x();
  anNpolVertex->momY = (aTrack->GetMomentum()).y();
  anNpolVertex->momZ = (aTrack->GetMomentum()).z();
  anNpolVertex->time = (aTrack->GetGlobalTime());
  anNpolVertex->energy = aTrack->GetTotalEnergy();
  anNpolVertex->particle = (aTrack->GetDefinition()->GetParticleName()).data();
  if(aTrack->GetCreatorProcess() != NULL)
    anNpolVertex->process = (aTrack->GetCreatorProcess()->GetProcessName()).data();
  else
    anNpolVertex->process = "";
  anNpolVertex->volume = (aTrack->GetVolume()->GetName()).data();
  
  tracks->push_back(anNpolVertex);
}

void NpolAnalysisManager::FillTree() {
  npolTree->Fill();
}

void NpolAnalysisManager::WriteTree() {
  npolTree->Write();
}

void NpolAnalysisManager::OpenFile() {
  G4String fileName = rootName+".root";
  //npolOutFile = new TFile(fileName);
  npolOutFile = new TFile("/data/tireman/simulation/output/FirstPass/Test/npol_test.root","RECREATE");
}

void NpolAnalysisManager::CloseFile() {
  npolOutFile->Close();
}

void NpolAnalysisManager::setFileName(const G4String& nam) 
{
  rootName = nam;
}
