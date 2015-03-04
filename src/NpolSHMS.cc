
// %% NpolSHMS.cc %%

// Super HMS Constructor file.  Currently only as target and target can.
// Created: Daniel Wilbern - March 2015

#include "G4PhysicalConstants.hh"
#include "G4Material.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4ios.hh"
#include "G4GDMLParser.hh"

#include "NpolMaterials.hh"
#include "NpolSHMS.hh"

NpolSHMS::NpolSHMS() {
  G4cout << "Initializing SHMS" << G4endl;
}

NpolSHMS::~NpolSHMS() {
  G4cout << "Deleting SHMS" << G4endl;
}

G4VPhysicalVolume *NpolSHMS::Construct(G4LogicalVolume *motherLV) {
  
	G4GDMLParser parser;

	G4String gdmlFilename = "gdml/no_beampipe_no_target.gdml";
	parser.Read(gdmlFilename);

	G4LogicalVolume *SHMSLV = parser.GetVolume("World");
	PlaceCylindrical(SHMSLV,motherLV,"SHMS",0,0,0);
	
}

