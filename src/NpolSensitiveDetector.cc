//Daniel Wilbern, dwilbern@nmu.edu

#include "G4SDManager.hh"

#include "NpolSensitiveDetector.hh"

NpolSensitiveDetector::NpolSensitiveDetector(G4String name)
		:G4VSensitiveDetector(name),
		collectionID(-1) {
				
	G4SDManager *SDMan = G4SDManager::GetSDMpointer();
	
	collectionName.insert("collection_name");
	SDMan->AddNewDetector(this);
}

NpolSensitiveDetector::~NpolSensitiveDetector() {}

//Initialize is invoked at the beginning of each event.
void NpolSensitiveDetector::Initialize(G4HCofThisEvent *HCE) {

	if(collectionID < 0) collectionID = GetCollectionID(0);

	hitsCollection = new NpolHitsCollection(SensitiveDetectorName,collectionName[0]);
	HCE->AddHitsCollection(collectionID,hitsCollection);
}

//EndOfEvent is invoked at the end of each event.
void NpolSensitiveDetector::EndOfEvent(G4HCofThisEvent *HCE) {;}

//ProcessHits is invoked every time a step occurs in the sensitive volume.
G4bool NpolSensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist) {

	NpolHit *aHit = new NpolHit(aStep);
	//...

	hitsCollection->insert(aHit);
	return true;  //G4bool return value is currently unused.
}

