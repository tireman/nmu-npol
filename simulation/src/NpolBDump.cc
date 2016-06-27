// %% NpolBDump.cc %%

// Beam Dump Constructor file. Filled with helium and a smack at the end.
// Created: William Tireman - March 2015

#include "G4PhysicalConstants.hh"
#include "G4Material.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4ios.hh"
#include "G4GDMLParser.hh"

#include "NpolMaterials.hh"
#include "NpolBDump.hh"

NpolBDump::NpolBDump() {
  G4GDMLParser parser;
  
  G4String gdmlFilename = "gdml/BeamDump.gdml";
  parser.Read(gdmlFilename);
  
  // retrieve the Beam Dump volume from GDML file and place in the
  // world. Beam dump contains simplifed helium vessel and stainless
  // steel end dump
  BDumpLV = parser.GetVolume("BDump");
  //BDumpLV->SetVisAttributes(G4VisAttributes::GetInvisible());
}

NpolBDump::~NpolBDump() {}

G4String NpolBDump::GetName() {
  return G4String("Beam Dump");
}

void NpolBDump::Place(G4LogicalVolume *motherLV) {
  // location of dump object center relative to target center
  G4double PosDump =38.0*m; 
  
  PlaceCylindrical(BDumpLV,motherLV,"BDump", PosDump, 0.0*deg, 0); 
}

