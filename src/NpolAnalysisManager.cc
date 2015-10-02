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
#include <iostream>
#include <sstream>

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
#include "NpolVertex.hh"
#include "NpolTagger.hh"


static NpolAnalysisManager *pInstance = NULL;

NpolAnalysisManager *NpolAnalysisManager::GetInstance() {
  if(pInstance == NULL)
    pInstance = new NpolAnalysisManager();
  
  return pInstance;
}

NpolAnalysisManager::NpolAnalysisManager(){

  initialized = false;
  npolOutFile = NULL;
  npolTree = NULL;
  tracks = NULL;
  taggedParticles = NULL;  

  if(getenv("NPOLBASENAME")){
    rootName = getenv("NPOLBASENAME");
  }else{
    rootName = "npol"; // default filename
  }

  if(getenv("NPOLDIR")){
    dirName = getenv("NPOLDIR");
  }else{
    dirName = "output"; // default directory location
  }

  if(getenv("JOBNUMBER")){
    jobNumber = getenv("JOBNUMBER");
  }else{
    jobNumber = "1000";
  }

  SetROOTFileNumber(1);
  OpenFile();
  Initialize();
}

NpolAnalysisManager::~NpolAnalysisManager() {
  ClearROOT();
}

void NpolAnalysisManager::Initialize(){
  if(initialized)
    std::cout << "WARNING: NpolAnalysisManager is already initialized and is being initialized again." << std::endl;

  tracks = new std::vector<NpolVertex *>();
  tracks->push_back(NULL);
  taggedParticles = new std::vector<NpolTagger *>();
  taggedParticles->push_back(NULL);
    
  npolTree = new TTree("T","Per-event information from Npol simulation");
  npolTree->Branch("tracks","std::vector<NpolVertex *>",&tracks,32000,2);
  npolTree->Branch("tagger","std::vector<NpolTagger *>",&taggedParticles,32000,2);
  initialized = true;
}

void NpolAnalysisManager::BeginOfRun(){
  // Called from EventAction ... once had the opening of the first 
  // ROOT file but it was double the size.  Moved openFile to constructor
  // and problem was solved. Curious.
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
  G4int trackId = aTrack->GetTrackID();
  G4int parentId = aTrack->GetParentID();
  
  anNpolVertex->trackId = trackId;
  anNpolVertex->parentId = parentId;
  anNpolVertex->posX = (aTrack->GetPosition()).x()/m;
  anNpolVertex->posY = (aTrack->GetPosition()).y()/m;
  anNpolVertex->posZ = (aTrack->GetPosition()).z()/m;
  anNpolVertex->momX = (aTrack->GetMomentum()).x()/MeV;
  anNpolVertex->momY = (aTrack->GetMomentum()).y()/MeV;
  anNpolVertex->momZ = (aTrack->GetMomentum()).z()/MeV;
  anNpolVertex->time = (aTrack->GetGlobalTime())/s;
  anNpolVertex->energy = aTrack->GetKineticEnergy()/MeV;
  anNpolVertex->eMiss = false;
  anNpolVertex->particle = (aTrack->GetDefinition()->GetParticleName()).data();
  anNpolVertex->particleId = (aTrack->GetDefinition()->GetPDGEncoding());
  if(aTrack->GetCreatorProcess() != NULL)
    anNpolVertex->process = (aTrack->GetCreatorProcess()->GetProcessName()).data();
  else
    anNpolVertex->process = "";
  anNpolVertex->volume = (aTrack->GetVolume()->GetName()).data();

  if(tracks->size() <= (unsigned int)trackId)
    tracks->resize(trackId+1);
  (*tracks)[trackId] = anNpolVertex;

  if(parentId != 0) {
    NpolVertex *parent = tracks->at(parentId);
    (parent->daughterIds).push_back(trackId);
  }
}

void NpolAnalysisManager::SetTrackAsKilled(int trackId) {
	(*tracks)[trackId]->eMiss = true;
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
  anNpolTaggedParticle->energy = aTrack->GetKineticEnergy()/MeV;
  anNpolTaggedParticle->particle = (aTrack->GetDefinition()->GetParticleName()).data();
  anNpolTaggedParticle->particleId = (aTrack->GetDefinition()->GetPDGEncoding());
   
  taggedParticles->push_back(anNpolTaggedParticle);
}

void NpolAnalysisManager::FillTree() {

  // This portion of code checks to see if the vector has any daughters at
  // all if it doesn't it does not fill the tree.  If it does then it 
  // checks to see if at least one is in an assembly volume which is 
  // part of the polarimeter.  If it is then it fills the tree, otherwise,
  // it skips filling and the next event is generated.  Cuts down on space 
  // used in the files from the majority of events which do not produce events
  // in the polarimeter

  std::vector<NpolVertex *>::iterator it;
  for(it = tracks->begin(); it != tracks->end(); it++){

    if(*it == NULL) continue;
    G4String volName = (*it)->volume;

    if(!((*it)->daughterIds).empty()){
      G4String subVolName = volName.substr(0,3).c_str();
      if(subVolName == "av_"){
	npolTree->Fill();
	break;
      }
    }
  }
}

void NpolAnalysisManager::WriteTree() {
  npolTree->Write();
}

void NpolAnalysisManager::OpenFile() {

  G4String fileName = Form("%s/%s_%s_%04d.root", dirName.c_str(), rootName.c_str(), jobNumber.c_str(), RootFileNumber);
  npolOutFile = new TFile(fileName,"RECREATE");

  if( npolOutFile->IsZombie() ) {
    G4cerr << "------- File " << fileName << "  could not be opened. " << G4endl;
    G4cerr << "             " << " Does the directory '" << dirName << "/' exist?" << G4endl;
    exit(0);
  }
  G4cout << "------- File " << fileName << " has been opened. " << G4endl;
}

void NpolAnalysisManager::CloseFile() {
  npolOutFile->Close();
}

void NpolAnalysisManager::setFileName(const G4String& nam) 
{
  rootName = nam;
  }

void NpolAnalysisManager::SetROOTFileNumber(G4int number){
  RootFileNumber = number;
}

void NpolAnalysisManager::CloseROOTChainFile(){
  WriteTree();
  CloseFile();
}

void NpolAnalysisManager::ClearROOT(){  
  npolOutFile = NULL;
  npolTree = NULL;
  tracks = NULL;
  taggedParticles = NULL;
  initialized = false;
}
