// %% NpolDataStructure.cc %%

// Daniel Wilbern, dwilbern@nmu.edu February 2015

#include <map>

#include "G4ios.hh"
#include "G4SystemOfUnits.hh"
#include "G4VPhysicalVolume.hh"

#include "NpolHistogramManager.hh"
#include "NpolDataStructure.hh"

static NpolDataStructure *pInstance = NULL;

NpolDataStructure::NpolDataStructure() {}

NpolDataStructure::~NpolDataStructure() {}

NpolDataStructure *NpolDataStructure::GetInstance() {
	if(pInstance == NULL)
		pInstance = new NpolDataStructure();

	return pInstance;
}

void NpolDataStructure::PrepareNewEvent() {
	std::map<G4VPhysicalVolume *, G4double>::iterator it;

	for(it = EDep.begin(); it != EDep.end(); it++)
		it->second = 0.0;
}

void NpolDataStructure::AddEDep(G4VPhysicalVolume *PV, G4double dep) {

	NpolHistogramManager *histoManager = NpolHistogramManager::GetInstance();

	if(histoManager->CheckHistogramAssociation(PV))
		EDep[PV] += dep;
}

void NpolDataStructure::FillHistograms() {

	NpolHistogramManager *histoManager = NpolHistogramManager::GetInstance();
	std::map<G4VPhysicalVolume *, G4double>::iterator it;

	for(it = EDep.begin(); it != EDep.end(); it++)
		histoManager->FillHistogram(it->first,it->second);
}

