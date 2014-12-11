// %% NpolWorld.cc %%

// COnstructor for the World
// Created: Danield Wilbern - November 2014

#include "G4SystemOfUnits.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"

#include "NpolWorld.hh"
#include "NpolMaterials.hh"

NpolWorld::NpolWorld() {
  G4cout << "Initializing world" << G4endl;
}

NpolWorld::~NpolWorld() {
  G4cout << "Deleting world" << G4endl;
}

void NpolWorld::Construct(G4LogicalVolume *motherLV) {

  G4Box *expHall = new G4Box("ExpHall", 25.0*m, 25.0*m, 27.0*m);
  expHallLV = new G4LogicalVolume(expHall, 
	    NpolMaterials::GetInstance()->GetAir(), "expHallLV", 0, 0, 0);
  expHallPV = new G4PVPlacement(0 , G4ThreeVector(), expHallLV, 
  	    "ExpHall", 0, false, 0);
  expHallLV->SetVisAttributes(G4VisAttributes::GetInvisible());
}

G4VPhysicalVolume *NpolWorld::GetWorldPV() {
  return expHallPV;
}

G4LogicalVolume *NpolWorld::GetWorldLV() {
  return expHallLV;
}

