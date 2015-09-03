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
#include "G4PhysicalConstants.hh"

#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TClassTable.h>
#include <TSystem.h>
#include <TInterpreter.h>

#include "NpolAnalysisManager.hh"
#include "NpolAnalysisMessenger.hh"
#include "NpolVertex.hh"
#include "NpolTagger.hh"


static NpolAnalysisManager *pInstance = NULL;

NpolAnalysisManager *NpolAnalysisManager::GetInstance() {
  if(pInstance == NULL)
    pInstance = new NpolAnalysisManager();
  
  return pInstance;
}

NpolAnalysisManager::NpolAnalysisManager(){

  //analysisMessenger = new NpolAnalysisMessenger(this);  issues to be resolved - W.T.
  initialized = false;
  npolOutFile = NULL;
  npolTree = NULL;
  tracks = NULL;
  taggedParticles = NULL;
  rootName = "npol"; // default filename, may be changed with setFileName() before file is opened

  Initialize();
}

NpolAnalysisManager::~NpolAnalysisManager() {
  delete npolTree;
  delete npolOutFile;
  delete tracks;
  //delete analysisMessenger;
}

void NpolAnalysisManager::Initialize(){
  if(initialized)
    std::cout << "WARNING: NpolAnalysisManager is already initialized and is being initialized again." << std::endl;
  
  if(!TClassTable::GetDict("include/NpolVertex.hh"))
    gSystem->Load("NpolVertex_hh.so");
  if(!TClassTable::GetDict("include/NpolTagger.hh"))
    gSystem->Load("NpolTagger_hh.so");
  
  gInterpreter->GenerateDictionary("vector<NpolVertex *>","include/NpolVertex.hh;vector");
  gInterpreter->GenerateDictionary("vector<NpolTagger *>","include/NpolTagger.hh;vector");

  tracks = new std::vector<NpolVertex *>();
  taggedParticles = new std::vector<NpolTagger *>();
    
  npolTree = new TTree("t_npolTree","Per-event information from Npol simulation");
  npolTree->Branch("tracks_branch","std::vector<NpolVertex *>",&tracks,32000,2);
  npolTree->Branch("tagger_branch","std::vector<NpolTagger *>",&taggedParticles,32000,2);
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

   std::vector<NpolTagger *>::iterator it2;
  for(it2 = taggedParticles->begin(); it2 != taggedParticles->end(); it2++)
    delete *it2;

  tracks->clear();
  taggedParticles->clear();
}

void NpolAnalysisManager::AddTrack(const G4Track *aTrack) {
  NpolVertex *anNpolVertex = new NpolVertex();
  
  anNpolVertex->trackId = aTrack->GetTrackID();
  anNpolVertex->parentId = aTrack->GetParentID();
  anNpolVertex->posX = (aTrack->GetPosition()).x()/m;
  anNpolVertex->posY = (aTrack->GetPosition()).y()/m;
  anNpolVertex->posZ = (aTrack->GetPosition()).z()/m;
  anNpolVertex->momX = (aTrack->GetMomentum()).x()/MeV;
  anNpolVertex->momY = (aTrack->GetMomentum()).y()/MeV;
  anNpolVertex->momZ = (aTrack->GetMomentum()).z()/MeV;
  anNpolVertex->time = (aTrack->GetGlobalTime())/s;
  anNpolVertex->energy = aTrack->GetTotalEnergy()/MeV;
  anNpolVertex->particle = (aTrack->GetDefinition()->GetParticleName()).data();
  if(aTrack->GetCreatorProcess() != NULL)
    anNpolVertex->process = (aTrack->GetCreatorProcess()->GetProcessName()).data();
  else
    anNpolVertex->process = "";
  anNpolVertex->volume = (aTrack->GetVolume()->GetName()).data();
  
  tracks->push_back(anNpolVertex);
}

void NpolAnalysisManager::AddTaggedParticle(const G4Track *aTrack) {
  NpolTagger *anNpolTaggedParticle = new NpolTagger();
  
  anNpolTaggedParticle->trackId = aTrack->GetTrackID();
   anNpolTaggedParticle->posX = (aTrack->GetPosition()).x()/m;
  anNpolTaggedParticle->posY = (aTrack->GetPosition()).y()/m;
  anNpolTaggedParticle->posZ = (aTrack->GetPosition()).z()/m;
  anNpolTaggedParticle->momX = (aTrack->GetMomentum()).x()/MeV;
  anNpolTaggedParticle->momY = (aTrack->GetMomentum()).y()/MeV;
  anNpolTaggedParticle->momZ = (aTrack->GetMomentum()).z()/MeV;
  anNpolTaggedParticle->time = (aTrack->GetGlobalTime())/s;
  anNpolTaggedParticle->energy = aTrack->GetTotalEnergy()/MeV;
  anNpolTaggedParticle->particle = (aTrack->GetDefinition()->GetParticleName()).data();
   
  taggedParticles->push_back(anNpolTaggedParticle);
}

void NpolAnalysisManager::FillTree() {
  npolTree->Fill();
}

void NpolAnalysisManager::WriteTree() {
  npolTree->Write();
}

void NpolAnalysisManager::OpenFile() {
  G4String fileName = rootName+".root";
  npolOutFile = new TFile(fileName,"RECREATE");
  //npolOutFile = new TFile("/data/tireman/simulation/output/FirstPass/Test/npol_test.root","RECREATE");
}

void NpolAnalysisManager::CloseFile() {
  npolOutFile->Close();
}

void NpolAnalysisManager::setFileName(const G4String& nam) 
{
  rootName = nam;
}
