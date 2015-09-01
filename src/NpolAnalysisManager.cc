//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************

#include <G4Track.hh>
#include <G4ThreeVector.hh>
#include <G4String.hh>
#include <G4ParticleDefinition.hh>
#include <G4VProcess.hh>

#include <TFile.h>
#include <TTree.h>
#include <TClassTable.h>
#include <TSystem.h>
#include <TInterpreter.h>

#include "NpolAnalysisManager.hh"
#include "NpolTrack.hh"

static NpolAnalysisManager *pInstance = NULL;

NpolAnalysisManager *NpolAnalysisManager::GetInstance() {
	if(pInstance == NULL)
		pInstance = new NpolAnalysisManager();

	return pInstance;
}

NpolAnalysisManager::NpolAnalysisManager() {
	initialized = false;
	npolOutFile = NULL;
	npolTree = NULL;

	Initialize();
}

NpolAnalysisManager::~NpolAnalysisManager() {
	delete npolTree;
	npolOutFile->Close();
	delete npolOutFile;
}

void NpolAnalysisManager::Initialize() {
	if(initialized)
		std::cout << "WARNING: NpolAnalysisManager is already initialized and is being initialized again." << std::endl;

	if(!TClassTable::GetDict("include/NpolTrack.hh"))
		gSystem->Load("NpolTrack_hh.so");

	gInterpreter->GenerateDictionary("vector<NpolTrack *>","include/NpolTrack.hh;vector");

	// Method to open TFile.  Hoping to make this available from macro as
	// some point -- W.T.
	OpenFile();

	npolTree = new TTree("t_npolTree","Per-event information from Npol simulation");
	npolTree->Branch("tracks_branch","std::vector<NpolTrack *>",&tracks,32000,2);
	initialized = true;
}

void NpolAnalysisManager::PrepareNewEvent() {
	std::vector<NpolTrack *>::iterator it;
	for(it = tracks.begin(); it != tracks.end(); it++)
		delete *it;
	
	tracks.clear();
}

void NpolAnalysisManager::AddTrack(const G4Track *aTrack) {
	NpolTrack *anNpolTrack = new NpolTrack();

	anNpolTrack->trackId = aTrack->GetTrackID();
	anNpolTrack->parentId = aTrack->GetParentID();
	anNpolTrack->posX = (aTrack->GetPosition()).x();
	anNpolTrack->posY = (aTrack->GetPosition()).y();
	anNpolTrack->posZ = (aTrack->GetPosition()).z();
	anNpolTrack->momX = (aTrack->GetMomentum()).x();
	anNpolTrack->momY = (aTrack->GetMomentum()).y();
	anNpolTrack->momZ = (aTrack->GetMomentum()).z();
	anNpolTrack->time = (aTrack->GetGlobalTime());
	anNpolTrack->energy = aTrack->GetTotalEnergy();
	anNpolTrack->particle = (aTrack->GetDefinition()->GetParticleName()).data();
	if(aTrack->GetCreatorProcess() != NULL)
		anNpolTrack->process = (aTrack->GetCreatorProcess()->GetProcessName()).data();
	anNpolTrack->volume = (aTrack->GetVolume()->GetName()).data();

	tracks.push_back(anNpolTrack);
}

void NpolAnalysisManager::FillTree() {
	npolTree->Fill();
}

void NpolAnalysisManager::WriteTree() {
	npolTree->Write();
}
void NpolAnalysisManager::OpenFile() {
  npolOutFile = new TFile("/data/tireman/simulation/output/FirstPass/Test/npol_test.root","RECREATE");
}

void NpolAnalysisManager::CloseFile() {
	npolOutFile->Close();
}

