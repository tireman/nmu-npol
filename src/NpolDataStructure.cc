//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************

// %% NpolDataStructure.cc %%
// Daniel Wilbern, dwilbern@nmu.edu February 2015

#include <cstdlib>
#include <map>

#include "G4ios.hh"
#include "G4SystemOfUnits.hh"
#include "G4VPhysicalVolume.hh"

#include "NpolDataStructure.hh"
#include "NpolAnalysis.hh"

static NpolDataStructure *pInstance = NULL;

NpolDataStructure::NpolDataStructure() {
	memset(&cols,0,sizeof(struct NtupleColumns));
}

NpolDataStructure::~NpolDataStructure() {
}

// NpolDataStructure is a singleton.  This function will return a pointer to the singleton.
NpolDataStructure *NpolDataStructure::GetInstance() {
	if(pInstance == NULL)
		pInstance = new NpolDataStructure();

	return pInstance;
}

void NpolDataStructure::RegisterActiveDetectorEDepHistogram(G4VPhysicalVolume *PV, G4String nname, G4String ttitle,
		G4int nnbins, G4double xxmin, G4double xxmax) {

	if(!isVolumeActive(PV)) {

		detData[PV].histoID = -1;
		detData[PV].name = strdup(nname.data());
		detData[PV].title = strdup(ttitle.data());
		detData[PV].nbins = nnbins;
		detData[PV].xmin = xxmin;
		detData[PV].xmax = xxmax;
		detData[PV].EDep = 0.0;
	}
}

void NpolDataStructure::CreateHistograms() {

	G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
	std::map<G4VPhysicalVolume *, struct HistoData>::iterator it;

	G4cout << "Creating EDep Histograms." << G4endl;

	for(it = detData.begin(); it != detData.end(); it++) {
		struct HistoData *histEntry = &(it->second);
		histEntry->histoID = analysisManager->CreateH1(G4String(histEntry->name), G4String(histEntry->title),
				histEntry->nbins, histEntry->xmin, histEntry->xmax);
	}
}

void NpolDataStructure::CreateNtuple() {

	G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

	G4cout << "Creating Ntuple." << G4endl;

	// For some reason (a bug?) the Ntuple is not created properly if at least one histogram doesn't exist.
	// We'll make a dummy one here just to make sure
	analysisManager->CreateH1("dummy", "Dummy Histogram - Please Disregard", 100, 0*MeV, 100*MeV);

	analysisManager->CreateNtuple("NpolData", "Npol Data");
	//cols.volumeNameColID = analysisManager->CreateNtupleSColumn("VolumeName");
	cols.particleIDColID = analysisManager->CreateNtupleIColumn("ParticleID");
	cols.parentIDColID = analysisManager->CreateNtupleIColumn("ParentID");
	cols.vertexEnergyColID = analysisManager->CreateNtupleDColumn("VertexEnergy");
	cols.xPosColID = analysisManager->CreateNtupleDColumn("XPosition");
	cols.yPosColID = analysisManager->CreateNtupleDColumn("YPosition");
	cols.zPosColID = analysisManager->CreateNtupleDColumn("ZPosition");
	analysisManager->FinishNtuple();
}

void NpolDataStructure::PrepareNewEvent() {
	std::map<G4VPhysicalVolume *, struct HistoData>::iterator it;

	for(it = detData.begin(); it != detData.end(); it++)
		(it->second).EDep = 0.0;
}

void NpolDataStructure::AddEDep(G4VPhysicalVolume *PV, G4double dep) {

	if(isVolumeActive(PV))
		detData[PV].EDep += dep;
}

void NpolDataStructure::FillHistograms() {

	std::map<G4VPhysicalVolume *, struct HistoData>::iterator it;
	for(it = detData.begin(); it != detData.end(); it++)
		FillAHistogram(&(it->second));
}

void NpolDataStructure::FillNtuple(G4String volName, G4int particleID, G4int parentID, G4double vertexEnergy,
		G4double xPos, G4double yPos, G4double zPos) {

	G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

	//analysisManager->FillNtupleSColumn(cols.volumeNameColID, volName);
	analysisManager->FillNtupleIColumn(cols.particleIDColID, particleID);
	analysisManager->FillNtupleIColumn(cols.parentIDColID, parentID);
	analysisManager->FillNtupleDColumn(cols.vertexEnergyColID, vertexEnergy);
	analysisManager->FillNtupleDColumn(cols.xPosColID, xPos);
	analysisManager->FillNtupleDColumn(cols.yPosColID, yPos);
	analysisManager->FillNtupleDColumn(cols.zPosColID, zPos);
	analysisManager->AddNtupleRow();
}

/* *** private methods *** */

void NpolDataStructure::FillAHistogram(struct HistoData *histoData) {

	G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

	if(histoData->EDep != 0.0*MeV)
		analysisManager->FillH1(histoData->histoID, histoData->EDep);
}

bool NpolDataStructure::isVolumeActive(G4VPhysicalVolume *PV) {
	return (detData.find(PV) != detData.end());
}

