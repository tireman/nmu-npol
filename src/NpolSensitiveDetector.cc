//Daniel Wilbern, dwilbern@nmu.edu

#include "G4SDManager.hh"
#include "NpolSensitiveDetector.hh"
#include "NpolHit.hh"

NpolSensitiveDetector::NpolSensitiveDetector(G4String name, G4String colName)
  :G4VSensitiveDetector(name), hitsCollection(0),
   collectionID(-1) 
{
  collectionName.insert(colName);
}

NpolSensitiveDetector::~NpolSensitiveDetector() {}

//Initialize is invoked at the beginning of each event.
void NpolSensitiveDetector::Initialize(G4HCofThisEvent*) {

  hitsCollection = new NpolHitsCollection
    (SensitiveDetectorName,collectionName[0]);
  
}

//EndOfEvent is invoked at the end of each event.
void NpolSensitiveDetector::EndOfEvent(G4HCofThisEvent *HCE) {
  if(collectionID < 0) {
    collectionID = G4SDManager::GetSDMpointer()->GetCollectionID(hitsCollection);
  }  
  HCE->AddHitsCollection(collectionID,hitsCollection);
}

//ProcessHits is invoked every time a step occurs in the sensitive volume.
G4bool NpolSensitiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory*) {
  NpolHit *aHit = new NpolHit(aStep);
  // if(aHit->GetParticleName() == "neutron" && aHit->GetParentID() == 0 && aHit->GetTrackID() == 1 && aHit->GetProcessName() == "hadElastic")
  // if(aHit->GetParticleName() == "proton" && aHit->GetParentID() == 1)
  //  if(aHit->GetParentID() == 1){
  //G4cout << "Looking for a hit ..." << G4endl;
  //aHit->Print();
  //G4cout << "Collection ID is: " << collectionID << G4endl;

  if(collectionID > -1) {
    hitsCollection->insert(aHit);
  
    G4cout << "A hit was observed and inserted" << G4endl;
  }else{
    delete aHit;
  }
  return true; //G4bool return value is currently unused.
}
