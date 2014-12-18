// %% NpolTarget.cc %%

// Target Constructor file.  Currently only as target and target can.
// Created: William Tireman - December 2014
// Modified: Daniel Wilbern - December 2014

#include "G4PhysicalConstants.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4AssemblyVolume.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4ios.hh"

#include "NpolMaterials.hh"
#include "NpolThing.hh"

NpolThing::NpolThing() {
  G4cout << "Initializing Thing" << G4endl;
}

NpolThing::~NpolThing() {
  G4cout << "Deleting Thing" << G4endl;
}

G4VPhysicalVolume *NpolThing::Construct(G4LogicalVolume *motherLV) {
 
   G4Box *aBox = new G4Box("thingBox", 1*m, 1*m, 1*m);
	G4LogicalVolume *ThingLV = new G4LogicalVolume(aBox, NpolMaterials::GetInstance()->GetAl(), "ThingLV",0,0,0);
	

  return PlaceCylindrical(ThingLV, motherLV, "Thing", 10*m,-45*deg,5*m);
}

