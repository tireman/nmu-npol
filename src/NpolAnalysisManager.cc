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
#include <G4Step.hh>
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
#include "NpolStep.hh"
#include "NpolStatistics.hh"


static NpolAnalysisManager *pInstance = NULL;

NpolAnalysisManager *NpolAnalysisManager::GetInstance() {
  if(pInstance == NULL)
    pInstance = new NpolAnalysisManager();
  
  return pInstance;
}

NpolAnalysisManager::NpolAnalysisManager(){

  npolOutFile = NULL;
  npolTree = NULL;
  statsTree = NULL;
  tracks = NULL;
  NPOLTaggedParticle = NULL;
  SHMSTaggedParticle = NULL;
  TargetTaggedParticle = NULL;
  EventSteps = NULL; 
  statistics = NULL;
  Initialize();

}

NpolAnalysisManager::~NpolAnalysisManager() {
  ClearObjects();
  delete statistics;
}

void NpolAnalysisManager::Initialize(){
  if(singletonInitialized)
    std::cout << "WARNING: NpolAnalysisManager is already initialized and is being initialized again." << std::endl;
  
  InitializeFilenameVariables();
  eventsPerFile = 100000; // the number of primary events that will be 
                           //simulated before the output file is changed
  
  InitializeObjects();
  
  singletonInitialized = true;
}

void NpolAnalysisManager::InitializeObjects() {

  statistics = new std::vector<NpolStatistics *>();
  statistics->push_back(new NpolStatistics());
  ((*statistics)[0])->version = 20151216;  // Determined by Date: YYYYMMDD
  ((*statistics)[0])->totalEvents = 0;
  ((*statistics)[0])->eventsSaved = 0;

  tracks = new std::vector<NpolVertex *>();
  tracks->push_back(NULL);

  NPOLTaggedParticle = new std::vector<NpolTagger *>();
  NPOLTaggedParticle->push_back(NULL);
  
  SHMSTaggedParticle = new std::vector<NpolTagger *>();
  SHMSTaggedParticle->push_back(NULL);

  TargetTaggedParticle = new std::vector<NpolTagger *>();
  TargetTaggedParticle->push_back(NULL);

  EventSteps = new std::vector<NpolStep *>();
  EventSteps ->push_back(NULL);
  
  npolTree = new TTree("T","Per-event information from Npol simulation");
  npolTree->Branch("tracks","std::vector<NpolVertex *>",&tracks,32000,2);
  npolTree->Branch("steps","std::vector<NpolStep *>",&EventSteps,32000,2);
  npolTree->Branch("NPOL_Tagger","std::vector<NpolTagger *>",&NPOLTaggedParticle,32000,2);
  npolTree->Branch("SHMS_Tagger","std::vector<NpolTagger *>",&SHMSTaggedParticle,32000,2);
  npolTree->Branch("Target_Tagger","std::vector<NpolTagger *>",&TargetTaggedParticle,32000,2);

  statsTree = new TTree("T2","Per-run information from Npol simulation");
  statsTree->Branch("stats","std::vector<NpolStatistics *>",&statistics,32000,2);
}

void NpolAnalysisManager::BeginOfRun(){}

void NpolAnalysisManager::EndOfRun(){
  WriteObjectsToFile();
  npolOutFile->Close();
  ClearObjects();
  G4cout << "Shutting down the run!" << G4endl;
}

void NpolAnalysisManager::PrepareNewEvent(const G4int evtID) {
  eventFlag = false;

  // clean out the vectors from last event
  std::vector<NpolVertex *>::iterator it;
  for(it = tracks->begin(); it != tracks->end(); it++)
    delete *it;
  
  std::vector<NpolTagger *>::iterator it2;
  for(it2 = NPOLTaggedParticle->begin(); it2 != NPOLTaggedParticle->end(); it2++)
    delete *it2;

  std::vector<NpolTagger *>::iterator it3;
  for(it3 = SHMSTaggedParticle->begin(); it3 != SHMSTaggedParticle->end(); it3++)
    delete *it3;
  
  std::vector<NpolStep *>::iterator it4;
  for(it4 = EventSteps->begin(); it4 != EventSteps->end(); it4++)
    delete *it4;

  std::vector<NpolTagger *>::iterator it5;
  for(it5 = TargetTaggedParticle->begin(); it5 != TargetTaggedParticle->end(); it5++)
    delete *it5;
 
  tracks->clear();
  NPOLTaggedParticle->clear();
  SHMSTaggedParticle->clear();
  TargetTaggedParticle->clear();
  EventSteps->clear();

  // maybe open a new root file
  if((evtID % eventsPerFile == 0)/* && (evtId != 0)*/) {
    if(npolOutFile != NULL) {
      WriteObjectsToFile();
      npolOutFile->Close();
      ClearObjects();
    }
    
    RootFileNumber++;
    OpenRootFile();
    InitializeObjects();
  }
  
  ((*statistics)[0])->totalEvents++;
}

void NpolAnalysisManager::AddStep(const G4Step *aStep, std::string volName){
  NpolStep *Step = new NpolStep();
  G4Track *aTrack = aStep->GetTrack();
  Step->time = (aTrack->GetGlobalTime())/ns;
  Step->eDep = (aStep->GetTotalEnergyDeposit())/MeV;
  Step->volume = volName;

  EventSteps->push_back(Step);
}


void NpolAnalysisManager::AddTrack(const G4Track *aTrack) {
  NpolVertex *anNpolVertex = new NpolVertex();
  G4int trackId = aTrack->GetTrackID();
  G4int parentId = aTrack->GetParentID();
  
  anNpolVertex->trackId = trackId;
  anNpolVertex->parentId = parentId;
  anNpolVertex->posX = (aTrack->GetPosition()).x()/cm;
  anNpolVertex->posY = (aTrack->GetPosition()).y()/cm;
  anNpolVertex->posZ = (aTrack->GetPosition()).z()/cm;
  anNpolVertex->momX = (aTrack->GetMomentum()).x()/MeV;
  anNpolVertex->momY = (aTrack->GetMomentum()).y()/MeV;
  anNpolVertex->momZ = (aTrack->GetMomentum()).z()/MeV;
  anNpolVertex->time = (aTrack->GetGlobalTime())/ns;
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

void NpolAnalysisManager::AddTargetTaggedParticle(const G4Track *aTrack) {
  NpolTagger *anTargetTaggedParticle = new NpolTagger();
  
  anTargetTaggedParticle->trackId = aTrack->GetTrackID();
  anTargetTaggedParticle->posX = (aTrack->GetPosition()).x()/cm;
  anTargetTaggedParticle->posY = (aTrack->GetPosition()).y()/cm;
  anTargetTaggedParticle->posZ = (aTrack->GetPosition()).z()/cm;
  anTargetTaggedParticle->momX = (aTrack->GetMomentum()).x()/MeV;
  anTargetTaggedParticle->momY = (aTrack->GetMomentum()).y()/MeV;
  anTargetTaggedParticle->momZ = (aTrack->GetMomentum()).z()/MeV;
  anTargetTaggedParticle->time = (aTrack->GetGlobalTime())/ns;
  anTargetTaggedParticle->energy = aTrack->GetKineticEnergy()/MeV;
  anTargetTaggedParticle->particle = (aTrack->GetDefinition()->GetParticleName()).data();
  anTargetTaggedParticle->particleId = (aTrack->GetDefinition()->GetPDGEncoding());
  
  TargetTaggedParticle->push_back(anTargetTaggedParticle);
  
  // Trigers the event to be saved as long as one step takes placed in this volume
  eventFlag = true;
 
}

void NpolAnalysisManager::AddNPOLTaggedParticle(const G4Track *aTrack) {
  NpolTagger *anNpolTaggedParticle = new NpolTagger();
  
  anNpolTaggedParticle->trackId = aTrack->GetTrackID();
  anNpolTaggedParticle->posX = (aTrack->GetPosition()).x()/cm;
  anNpolTaggedParticle->posY = (aTrack->GetPosition()).y()/cm;
  anNpolTaggedParticle->posZ = (aTrack->GetPosition()).z()/cm;
  anNpolTaggedParticle->momX = (aTrack->GetMomentum()).x()/MeV;
  anNpolTaggedParticle->momY = (aTrack->GetMomentum()).y()/MeV;
  anNpolTaggedParticle->momZ = (aTrack->GetMomentum()).z()/MeV;
  anNpolTaggedParticle->time = (aTrack->GetGlobalTime())/ns;
  anNpolTaggedParticle->energy = aTrack->GetKineticEnergy()/MeV;
  anNpolTaggedParticle->particle = (aTrack->GetDefinition()->GetParticleName()).data();
  anNpolTaggedParticle->particleId = (aTrack->GetDefinition()->GetPDGEncoding());
  
  NPOLTaggedParticle->push_back(anNpolTaggedParticle);
}

void NpolAnalysisManager::AddSHMSTaggedParticle(const G4Track *aTrack) {
  NpolTagger *anSHMSTaggedParticle = new NpolTagger();
  
  anSHMSTaggedParticle->trackId = aTrack->GetTrackID();
  anSHMSTaggedParticle->posX = (aTrack->GetPosition()).x()/cm;
  anSHMSTaggedParticle->posY = (aTrack->GetPosition()).y()/cm;
  anSHMSTaggedParticle->posZ = (aTrack->GetPosition()).z()/cm;
  anSHMSTaggedParticle->momX = (aTrack->GetMomentum()).x()/MeV;
  anSHMSTaggedParticle->momY = (aTrack->GetMomentum()).y()/MeV;
  anSHMSTaggedParticle->momZ = (aTrack->GetMomentum()).z()/MeV;
  anSHMSTaggedParticle->time = (aTrack->GetGlobalTime())/ns;
  anSHMSTaggedParticle->energy = aTrack->GetKineticEnergy()/MeV;
  anSHMSTaggedParticle->particle = (aTrack->GetDefinition()->GetParticleName()).data();
  anSHMSTaggedParticle->particleId = (aTrack->GetDefinition()->GetPDGEncoding());
  
  SHMSTaggedParticle->push_back(anSHMSTaggedParticle);
}

void NpolAnalysisManager::FillTree() {

  if(eventFlag){
    if(EventSteps != NULL) QSort(EventSteps,1,EventSteps->size());
    npolTree->Fill();
    ((*statistics)[0])->eventsSaved++;
  }
}


void NpolAnalysisManager::WriteObjectsToFile() {
  statsTree->Fill();
  npolTree->Write(); 
  statsTree->Write();
}

void NpolAnalysisManager::OpenRootFile() {
  
  G4String fileName = Form("%s/%s_%s_%04d.root", dirName.c_str(), rootName.c_str(), jobNumber.c_str(), RootFileNumber);
  npolOutFile = new TFile(fileName,"RECREATE");
  
  if( npolOutFile->IsZombie() ) {
    G4cerr << "------- File " << fileName << "  could not be opened. " << G4endl;
    G4cerr << "             " << " Does the directory '" << dirName << "/' exist?" << G4endl;
    exit(0);
  }
  G4cout << "------- File " << fileName << " has been opened. " << G4endl;
}

void NpolAnalysisManager::setFileName(const G4String& nam) {
  rootName = nam;
}

void NpolAnalysisManager::ClearObjects(){

  std::vector<NpolStatistics *>::iterator it;
  for(it = statistics->begin(); it != statistics->end(); it++)
    delete *it;
  statistics->clear();

  delete npolOutFile;
  delete tracks;
  delete NPOLTaggedParticle;
  delete SHMSTaggedParticle;
  delete TargetTaggedParticle;
  delete EventSteps;
  delete statistics;

  npolOutFile = NULL;
  npolTree = NULL;
  statsTree = NULL;
  tracks = NULL;
  NPOLTaggedParticle = NULL;
  SHMSTaggedParticle = NULL;
  TargetTaggedParticle = NULL;
  EventSteps = NULL;
  statistics = NULL;
}

void NpolAnalysisManager::InitializeFilenameVariables(){
  if(getenv("NPOLBASENAME")){
    rootName = getenv("NPOLBASENAME");
  }else{
    rootName = "npol"; // default filename
  }
  
  if(getenv("NPOLDIR")){
    dirName = getenv("NPOLDIR");
  }else{
    dirName = "output"; // default directory location is build/output
  }
  
  if(getenv("JOBNUMBER")){
    jobNumber = getenv("JOBNUMBER");
  }else{
    jobNumber = "99999"; // default job number is 99999; anyone plan to submit more than this number of jobs?
  }
  
  RootFileNumber = 0;
}


// Quick sort routine for the step vector to put the steps in time order
int NpolAnalysisManager::partition(std::vector<NpolStep*> *aVector, int p, int q){
 
  double x = (*aVector)[p]->time;
  int i = p;
  int j;
 
  for(j = p+1; j < q; j++){
    if((*aVector)[j]->time <= x){
      i = i +1;
      std::swap((*aVector)[i],(*aVector)[j]);
    }
  }

  std::swap((*aVector)[i],(*aVector)[p]);
  return i;
}

void NpolAnalysisManager::QSort(std::vector<NpolStep*> *aVector, int p, int q){

  if(aVector == NULL) return;
  int r; 

  if(p<q){
    r = partition(aVector,p,q);
    QSort(aVector,p,r);
    QSort(aVector,r+1,q);
  }
}
