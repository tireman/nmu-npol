//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************

// %% NpolAnalysis.cc %%

// Created: Daniel Wilbern - February 2015

#include <map>

#include "G4ios.hh"
#include "G4SystemOfUnits.hh"
#include "G4VPhysicalVolume.hh"

#include "NpolAnalysis.hh"
#include "NpolHistogramManager.hh"

static NpolHistogramManager *pInstance = NULL;

NpolHistogramManager::NpolHistogramManager() {}

NpolHistogramManager::~NpolHistogramManager() {}

NpolHistogramManager *NpolHistogramManager::GetInstance() {

	if(pInstance == NULL)
		pInstance = new NpolHistogramManager();

	return pInstance;
}

// Given a PV and desired histogram characteristics, get set up to create a histogram for this PV when RunAction happens.
void NpolHistogramManager::RegisterActiveDetector(G4VPhysicalVolume *PV, G4String name, G4String title, G4int nbins, G4double xmin, G4double xmax) {

	struct EDepHistoData st = {-1, name, title, nbins, xmin, xmax};

	histoData[PV] = st;
}

// This method will iterate through histoData and create any histograms that haven't been created yet.
// This method is intended to be called in RunAction.
void NpolHistogramManager::CreateHistograms() {

	G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
	std::map<G4VPhysicalVolume *, struct EDepHistoData>::iterator it;
	struct EDepHistoData *histEntry;

	for(it = histoData.begin(); it != histoData.end(); it++) {
		histEntry = &(it->second);
		G4cout << "Creating histogram for " << histEntry->name << G4endl;
		if(histEntry->histoID == -1)
			histEntry->histoID = analysisManager->CreateH1(histEntry->name, histEntry->title,
					histEntry->nbins, histEntry->xmin, histEntry->xmax);
	}
}

// Given a PV pointer, fill its histograma with the value 'value'
// This method is intended to be called in EndOfEventAction.
void NpolHistogramManager::FillHistogram(G4VPhysicalVolume *PV, G4double value) {

	G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

	if((value >= 1.0) && (histoData.find(PV) != histoData.end()))
		analysisManager->FillH1((histoData[PV]).histoID, value);
}

// Returns true if the given PV has a histogram associated with it.
bool NpolHistogramManager::CheckHistogramAssociation(G4VPhysicalVolume *PV) {

	return (histoData.find(PV) != histoData.end());
}

