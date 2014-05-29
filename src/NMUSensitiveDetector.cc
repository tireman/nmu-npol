//Daniel Wilbern, dwilbern@nmu.edu

#include "G4SDManager.hh"

#include "NMUSensitiveDetector.hh"

NMUSensitiveDetector::NMUSensitiveDetector(G4String name)
		:G4VSensitiveDetector(name),
		collectionID(-1) {
				
	G4SDManager *SDMan = G4SDManager::GetSDMpointer();
	
	collectionName.insert("collection_name");
	SDMan->AddNewDetector(this);
}

NMUSensitiveDetector::~NMUSensitiveDetector() {}

//Initialize is invoked at the beginning of each event.
void NMUSensitiveDetector::Initialize(G4HCofThisEvent *HCE) {

	if(collectionID < 0) collectionID = GetCollectionID(0);

	hitsCollection = new NMUHitsCollection(SensitiveDetectorName,collectionName[0]);
	HCE->AddHitsCollection(collectionID,hitsCollection);
}

//EndOfEvent is invoked at the end of each event.
void NMUSensitiveDetector::EndOfEvent(G4HCofThisEvent *HCE) {;}

//ProcessHits is invoked every time a step occurs in the sensitive volume.
G4bool NMUSensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist) {

	NMUHit *aHit = new NMUHit(aStep);
	//...

	hitsCollection->insert(aHit);
	return true;  //G4bool return value is currently unused.
}

