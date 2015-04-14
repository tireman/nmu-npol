//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************

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
  
  G4double ShmsAng = 16.0*deg, PosHB = 1.76*m;
  G4GDMLParser parser;
  
  G4String gdmlFilename = "gdml/HBender.gdml";
  parser.Read(gdmlFilename);
  
  G4LogicalVolume *HBenderLV = parser.GetVolume("HBenderPV");
  G4LogicalVolume *HBCryoBoxLV = parser.GetVolume("HBCryoBoxLogic");  
  G4LogicalVolume *HBBoreLogicLV = parser.GetVolume("HBBoreLogic");

  //G4VisAttributes *TopVisAtt= new G4VisAttributes(G4Colour(0.0,0.0,1.0));
  //HBenderLV->SetVisAttributes(TopVisAtt);
  HBenderLV->SetVisAttributes(G4VisAttributes::GetInvisible());

  G4VisAttributes *TopVisAtt= new G4VisAttributes(G4Colour(1.0,0.0,1.0));
  HBCryoBoxLV->SetVisAttributes(TopVisAtt);

  TopVisAtt= new G4VisAttributes(G4Colour(0.0,1.0,1.0));
  HBBoreLogicLV->SetVisAttributes(TopVisAtt);

  PlaceCylindrical(HBenderLV,motherLV, "HBender", PosHB, ShmsAng, 0);
  } 

