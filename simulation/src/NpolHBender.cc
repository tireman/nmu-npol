//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	     *
//********************************************************************

// %% NpolHBender.cc %%

// Super HMS horizontal bender magnet Constructor file.
// Created: Daniel Wilbern - March 2015

#include "G4ios.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4GDMLParser.hh"

#include "NpolAnalysisManager.hh"
#include "NpolMaterials.hh"
#include "NpolHBender.hh"

G4double NpolHBender::ShmsAng = 36.53*deg; // Angle at 4.4 GeV/c 36.53*deg; at 11 GeV/c 16.79; 
G4double NpolHBender::PosHB = 1.76*m;   // Position of HB center from target
G4double NpolHBender::PosTagger = 1.195*m; // just in front of HB opening
G4double NpolHBender::thicknessTagger = 0.10*cm; // thickness of SHMS tagger
G4double NpolHBender::horizontalAcceptance = 0.048*rad; // SHMS horizontal acceptance (full)
G4double NpolHBender::verticalAcceptance = 0.080*rad; // SHMS veritical acceptance (full)

NpolHBender::NpolHBender() {
  ConstructHBender();
  ConstructSHMSTagger();
}

NpolHBender::~NpolHBender() {}

void NpolHBender::ConstructSHMSTagger(){
  
  G4Box *SHMSTagger = new G4Box("SHMSTagger", PosTagger*tan(horizontalAcceptance/2),PosTagger*tan(verticalAcceptance/2),thicknessTagger/2);
  SHMSTaggerLV = 
	new G4LogicalVolume(SHMSTagger,NpolMaterials::GetInstance()->GetMaterial("HardVacuum"),"SHMSTaggerLV",0,0,0);
  G4VisAttributes *TaggerVisAtt = new G4VisAttributes(G4Colour(0.2, 0.2, 0.2));
  SHMSTaggerLV->SetVisAttributes(TaggerVisAtt);
}

void NpolHBender::ConstructHBender(){
  G4GDMLParser parser;

  G4String buildDir = NpolAnalysisManager::GetInstance()->GetBuildDir();
  G4String gdmlFilename = buildDir + "gdml/HBender.gdml";
  parser.Read(gdmlFilename);
  
  HBCryoBoxLV = parser.GetVolume("HBCryoBoxLogic");  
  HBBoreLogicLV = parser.GetVolume("HBBoreLogic");
  //HBCoilLogicLV = parser.GetVolume("HBCoilLogic");
  HBYorkLogicLV = parser.GetVolume("HBYorkLogic");
  
  G4VisAttributes *VisAtt= new G4VisAttributes(G4Colour(1.0,0.0,1.0));
  HBCryoBoxLV->SetVisAttributes(VisAtt);
  
  VisAtt= new G4VisAttributes(G4Colour(0.0,1.0,1.0));
  HBBoreLogicLV->SetVisAttributes(VisAtt);
}

G4String NpolHBender::GetName() {
  return G4String("Horizontal Bender");
}

void NpolHBender::Place(G4LogicalVolume *motherLV) {
  
  PlaceCylindrical(HBBoreLogicLV, motherLV, "HBBore", PosHB, ShmsAng, 0);
  //PlaceCylindrical(HBCoilLogicLV, motherLV, "HBCoil", PosHB, ShmsAng, 0);
  PlaceCylindrical(HBYorkLogicLV, motherLV, "HBYork", PosHB, ShmsAng, 0);
  PlaceCylindrical(HBCryoBoxLV, motherLV, "HBCyro", PosHB, ShmsAng, 0);
  PlaceRectangular(SHMSTaggerLV,motherLV, "SHMSTagger", (PosTagger+4.0*cm)*sin(ShmsAng),0.0, (PosTagger-4.0*cm)*cos(ShmsAng), 0.0, ShmsAng, 0.0);
} 

