// %% NpolWorld.cc %%

// COnstructor for the World
// Created: Danield Wilbern - November 2014

#include "G4SystemOfUnits.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
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

G4VPhysicalVolume *NpolWorld::Construct(G4LogicalVolume *motherLV) {

  G4Box *expHall = new G4Box("ExpHall", 30.0*m, 30.0*m, 32.0*m);
  expHallLV = new G4LogicalVolume(expHall, 
	    NpolMaterials::GetInstance()->GetAir(), "expHallLV", 0, 0, 0);
  expHallLV->SetVisAttributes(G4VisAttributes::GetInvisible());

  return (expHallPV = PlaceRectangular(expHallLV, motherLV, "ExpHall", 0,0,0));
}

G4VPhysicalVolume *NpolWorld::GetWorldPV() {
  return expHallPV;
}

G4LogicalVolume *NpolWorld::GetWorldLV() {
  return expHallLV;
}

