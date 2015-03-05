
// %% NpolHBender.cc %%

// Super HMS horizontal bender magnet Constructor file.
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
#include "NpolHBender.hh"

NpolHBender::NpolHBender() {
  G4cout << "Initializing HBender" << G4endl;
}

NpolHBender::~NpolHBender() {
  G4cout << "Deleting HBender" << G4endl;
}

G4VPhysicalVolume *NpolHBender::Construct(G4LogicalVolume *motherLV) {
  
	G4GDMLParser parser;

	G4String gdmlFilename = "gdml/HBender.gdml";
	parser.Read(gdmlFilename);

	G4LogicalVolume *HBenderLV = parser.GetVolume("HBender");
	PlaceCylindrical(HBenderLV,motherLV,"HBender",0,0,0);
	
}

