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
#include "G4tgbGeometryDumper.hh" 
#include "NpolMaterials.hh"
#include "NpolHBender.hh"

NpolHBender::NpolHBender() {
  ConstructHBender();
  ConstructSHMSTagger();
}

NpolHBender::~NpolHBender() {}

void NpolHBender::ConstructSHMSTagger(){
  G4double xlen = 0.1588*m; G4double ylen = 0.2075*m; G4double zlen = 0.010*cm;
  
  G4Box *SHMSTagger = new G4Box("SHMSTagger",xlen/2,ylen/2,zlen/2);
  SHMSTaggerLV = new G4LogicalVolume(SHMSTagger,NpolMaterials::GetInstance()->GetAir(),"SHMSTaggerLV",0,0,0);
  G4VisAttributes *TaggerVisAtt = new G4VisAttributes(G4Colour(0.2, 0.2, 0.2));
  SHMSTaggerLV->SetVisAttributes(TaggerVisAtt);
}

void NpolHBender::ConstructHBender(){
  G4GDMLParser parser;
  
  G4String gdmlFilename = "gdml/HBender.gdml";
  parser.Read(gdmlFilename);
  
  //HBenderLV = parser.GetVolume("HBenderPV");
  HBCryoBoxLV = parser.GetVolume("HBCryoBoxLogic");  
  HBBoreLogicLV = parser.GetVolume("HBBoreLogic");
  HBCoilLogicLV = parser.GetVolume("HBCoilLogic");
  HBYorkLogicLV = parser.GetVolume("HBYorkLogic");
  
  //G4VisAttributes *VisAtt= new G4VisAttributes(G4Colour(0.0,0.0,1.0));
  //HBenderLV->SetVisAttributes(VisAtt);
  //HBenderLV->SetVisAttributes(G4VisAttributes::GetInvisible());
  
  G4VisAttributes *VisAtt= new G4VisAttributes(G4Colour(1.0,0.0,1.0));
  HBCryoBoxLV->SetVisAttributes(VisAtt);
  
  VisAtt= new G4VisAttributes(G4Colour(0.0,1.0,1.0));
  HBBoreLogicLV->SetVisAttributes(VisAtt);
}

G4String NpolHBender::GetName() {
  return G4String("Horizontal Bender");
}

void NpolHBender::Place(G4LogicalVolume *motherLV) {
  G4double ShmsAng = 36.53*deg; // Angle at 4.4 GeV/c 36.53*deg; 
  G4double PosHB = 1.76*m, PosTagger = 1.195*m;
  
  //PlaceCylindrical(HBenderLV,motherLV, "HBender", PosHB, ShmsAng, 0);
  PlaceCylindrical(HBBoreLogicLV, motherLV, "HBBore", PosHB, ShmsAng, 0);
  //PlaceCylindrical(HBCoilLogicLV, motherLV, "HBCoil", PosHB, ShmsAng, 0);
  PlaceCylindrical(HBYorkLogicLV, motherLV, "HBYork", PosHB, ShmsAng, 0);
  PlaceCylindrical(HBCryoBoxLV, motherLV, "HBCyro", PosHB, ShmsAng, 0);
  PlaceRectangular(SHMSTaggerLV,motherLV, "SHMSTagger", (PosTagger+4.0*cm)*sin(ShmsAng), 0.0, (PosTagger-4.0*cm)*cos(ShmsAng), 0.0, ShmsAng, 0.0);
} 

