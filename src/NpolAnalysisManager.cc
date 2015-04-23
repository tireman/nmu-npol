//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************

// %% NpolAnalysisManager.cc %%
// Daniel Wilbern, dwilbern@nmu.edu February 2015

#include <cstdlib>
#include <map>

#include "G4ios.hh"
#include "G4SystemOfUnits.hh"
#include "G4VPhysicalVolume.hh"

#include "NpolAnalysisManager.hh"
#include "NpolAnalysis.hh"

static NpolAnalysisManager *pInstance = NULL;

NpolAnalysisManager::NpolAnalysisManager() {
	nextVolumeID = 0;
	memset(&cols,0,sizeof(struct NtupleColumns));
}

NpolAnalysisManager::~NpolAnalysisManager() {
}

// NpolAnalysisManager is a singleton.  This function will return a pointer to the singleton.
NpolAnalysisManager *NpolAnalysisManager::GetInstance() {
	if(pInstance == NULL)
		pInstance = new NpolAnalysisManager();

	return pInstance;
}

void NpolAnalysisManager::RegisterActiveDetectorEDepHistogram(G4VPhysicalVolume *PV, G4String nname, G4String ttitle,
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

void NpolAnalysisManager::CreateHistograms() {

	G4AnalysisManager *analysisMan = G4AnalysisManager::Instance();
	std::map<G4VPhysicalVolume *, struct HistoData>::iterator it;

	G4cout << "Creating EDep Histograms." << G4endl;

	for(it = detData.begin(); it != detData.end(); it++) {
		struct HistoData *histEntry = &(it->second);
		histEntry->histoID = analysisMan->CreateH1(G4String(histEntry->name), G4String(histEntry->title),
				histEntry->nbins, histEntry->xmin, histEntry->xmax);
	}
}

void NpolAnalysisManager::CreateNtuple() {

	G4AnalysisManager *analysisMan = G4AnalysisManager::Instance();

	G4cout << "Creating Ntuple." << G4endl;

	// For some reason (a bug?) the Ntuple is not created properly if at least one histogram doesn't exist.
	// We'll make a dummy one here just to make sure
	analysisMan->CreateH1("dummy", "Dummy Histogram - Please Disregard", 100, 0*MeV, 100*MeV);

	analysisMan->CreateNtuple("NpolData", "Npol Data");
	cols.volumeIDColID = analysisMan->CreateNtupleIColumn("VolumeID");
	cols.particleIDColID = analysisMan->CreateNtupleIColumn("ParticleID");
	cols.parentIDColID = analysisMan->CreateNtupleIColumn("ParentID");
	cols.vertexEnergyColID = analysisMan->CreateNtupleDColumn("VertexEnergy");
	cols.xPosColID = analysisMan->CreateNtupleDColumn("XPosition");
	cols.yPosColID = analysisMan->CreateNtupleDColumn("YPosition");
	cols.zPosColID = analysisMan->CreateNtupleDColumn("ZPosition");
	cols.xMomColID = analysisMan->CreateNtupleDColumn("XMomentum");
	cols.yMomColID = analysisMan->CreateNtupleDColumn("YMomentum");
	cols.zMomColID = analysisMan->CreateNtupleDColumn("ZMomentum");
	analysisMan->FinishNtuple();
}

void NpolAnalysisManager::PrepareNewEvent() {
	std::map<G4VPhysicalVolume *, struct HistoData>::iterator it;

	for(it = detData.begin(); it != detData.end(); it++)
		(it->second).EDep = 0.0;
}

void NpolAnalysisManager::AddEDep(G4VPhysicalVolume *PV, G4double dep) {

	if(isVolumeActive(PV))
		detData[PV].EDep += dep;
}

void NpolAnalysisManager::FillHistograms() {

	std::map<G4VPhysicalVolume *, struct HistoData>::iterator it;
	for(it = detData.begin(); it != detData.end(); it++)
		FillAHistogram(&(it->second));
}

void NpolAnalysisManager::FillNtuple(G4VPhysicalVolume *PV, G4int particleID, G4int parentID, G4double vertexEnergy, G4double xPos, G4double yPos, G4double zPos, G4double xMom, G4double yMom, G4double zMom) {

	G4AnalysisManager *analysisMan = G4AnalysisManager::Instance();

	int volumeID = getVolIDFor(PV);

	analysisMan->FillNtupleIColumn(cols.volumeIDColID, volumeID);
	analysisMan->FillNtupleIColumn(cols.particleIDColID, particleID);
	analysisMan->FillNtupleIColumn(cols.parentIDColID, parentID);
	analysisMan->FillNtupleDColumn(cols.vertexEnergyColID, vertexEnergy);
	analysisMan->FillNtupleDColumn(cols.xPosColID, xPos);
	analysisMan->FillNtupleDColumn(cols.yPosColID, yPos);
	analysisMan->FillNtupleDColumn(cols.zPosColID, zPos);
	analysisMan->FillNtupleDColumn(cols.xMomColID, xMom);
	analysisMan->FillNtupleDColumn(cols.yMomColID, yMom);
	analysisMan->FillNtupleDColumn(cols.zMomColID, zMom);
	analysisMan->AddNtupleRow();
}

int NpolAnalysisManager::getVolIDFor(G4VPhysicalVolume *PV) {

	std::pair<std::map<G4VPhysicalVolume *, int>::iterator, bool> ret;

	ret = detectorIDs.insert(std::pair<G4VPhysicalVolume *, int>(PV, nextVolumeID++));
	if(!(ret.second))
		nextVolumeID--;
	return (ret.first)->second;
}

void NpolAnalysisManager::FillAHistogram(struct HistoData *histoData) {

	G4AnalysisManager *analysisMan = G4AnalysisManager::Instance();

	if(histoData->EDep != 0.0*MeV)
		analysisMan->FillH1(histoData->histoID, histoData->EDep);
}

bool NpolAnalysisManager::isVolumeActive(G4VPhysicalVolume *PV) {
	return (detData.find(PV) != detData.end());
}

void NpolAnalysisManager::WriteDetectorIDsToFile() {

	std::map<G4VPhysicalVolume *, int>::iterator it;
	FILE *f = fopen("/data/dwilbern/detIDs.txt","w+");

	for(it = detectorIDs.begin(); it != detectorIDs.end(); it++)
		fprintf(f,"%03d,%s\n",it->second, it->first->GetName().data());

	fclose(f);
}

