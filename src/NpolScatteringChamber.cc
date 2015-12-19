//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//******************************************************************

// %% NpolScatteringChamber.cc %%

// Scattering Chamber constructor file.
// Created: Daniel Wilbern - March 2015

#include "globals.hh"
#include "G4PhysicalConstants.hh"
#include "G4Material.hh"
#include "G4Tubs.hh"
#include "G4ExtrudedSolid.hh"
#include "G4TwoVector.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4ios.hh"

#include "NpolMaterials.hh"
#include "NpolTarget.hh"
#include "NpolScatteringChamber.hh"

G4double NpolScatteringChamber::insideRadius = 0.5*1.041*m;
G4double NpolScatteringChamber::insideHeight = 1.2*m;
G4double NpolScatteringChamber::wallThickness = 1.30*cm;
G4double NpolScatteringChamber::holeRadius = 2.5*cm;
G4double NpolScatteringChamber::WindowThickness = 0.1016*cm;
G4double NpolScatteringChamber::shmsWindowAngle = 110.0*deg;
G4double NpolScatteringChamber::shmsWindowDeltaAngle = 60.0*deg;
G4double NpolScatteringChamber::npolWindowAngle = 190.0*deg;
G4double NpolScatteringChamber::npolWindowDeltaAngle = 60.0*deg;
G4double NpolScatteringChamber::npolWindowHeight = 10.00*cm;
G4double NpolScatteringChamber::shmsWindowHeight = 12.00*cm;

NpolScatteringChamber::NpolScatteringChamber() {
  target = new NpolTarget();
  ConstructChamber();
}

NpolScatteringChamber::~NpolScatteringChamber() {
  delete target;
}

G4String NpolScatteringChamber::GetName() {
  return G4String("Scattering Chamber");
}

void NpolScatteringChamber::ConstructChamber() {
  
  G4ThreeVector translation = G4ThreeVector(0,0,0);
  G4RotationMatrix yRot90deg;
  yRot90deg.rotateY(90*deg);
  G4Transform3D transform = G4Transform3D(yRot90deg, translation);
  G4int gnum = 360;  // number of facets for the scattering chamber solid
  std::vector<G4TwoVector> polygon(gnum);
  std::vector<G4TwoVector> polygon2(122); // using 120 degrees for windows
  
  // New inner chamber using extruded solid
  for (int i = 0; i < gnum; i++){
    polygon[i] = G4TwoVector(insideRadius*cos(360.0*deg/gnum*i),insideRadius*sin(360.0*deg/gnum*i));
  }
  std::vector<G4ExtrudedSolid::ZSection> zsections1;
  zsections1.push_back(G4ExtrudedSolid::ZSection((-insideHeight/2),G4TwoVector(0.0*cm,0.0*cm),1.0));
  zsections1.push_back(G4ExtrudedSolid::ZSection((+insideHeight/2),G4TwoVector(0.0*cm,0.0*cm),1.0));
  G4ExtrudedSolid *innerChamber = new G4ExtrudedSolid("InnerChamber", polygon, zsections1);

  // New Solid chamber using extruded solid
  for (int i = 0; i < gnum; i++){
    polygon[i] = G4TwoVector((insideRadius+wallThickness)*cos(360.0*deg/gnum*i),(insideRadius+wallThickness)*sin(360.0*deg/gnum*i));
  }
  std::vector<G4ExtrudedSolid::ZSection> zsections2;
  zsections2.push_back(G4ExtrudedSolid::ZSection((-(insideHeight+(2*wallThickness))/2),G4TwoVector(0.0*cm,0.0*cm),1.0));
  zsections2.push_back(G4ExtrudedSolid::ZSection((+(insideHeight+(2*wallThickness))/2),G4TwoVector(0.0*cm,0.0*cm),1.0));
  G4ExtrudedSolid *solidChamber = new G4ExtrudedSolid("SolidChamber", polygon, zsections2);


  // New Npol Window using extruded solid
  for (int i = 0; i < 61; i++){
    polygon2[i] = G4TwoVector((insideRadius+wallThickness-WindowThickness)*cos(npolWindowAngle+i*deg),(insideRadius+wallThickness-WindowThickness)*sin(npolWindowAngle+i*deg));
  }
  for (int i = 0; i < 61; i++){
    polygon2[61+i] = G4TwoVector((insideRadius-1.0*cm)*cos(npolWindowAngle+npolWindowDeltaAngle-i*deg),(insideRadius-1.0*cm)*sin(npolWindowAngle+npolWindowDeltaAngle-i*deg));
  }
  std::vector<G4ExtrudedSolid::ZSection> zsections3;
  zsections3.push_back(G4ExtrudedSolid::ZSection(-npolWindowHeight/2, G4TwoVector(0.0*cm, 0.0*cm), 1.0));
  zsections3.push_back(G4ExtrudedSolid::ZSection(+npolWindowHeight/2, G4TwoVector(0.0*cm, 0.0*cm), 1.0));
  G4ExtrudedSolid *npolWindow = new G4ExtrudedSolid("npolWindow", polygon2, zsections3);

// New SHMS Window using extruded solid
  for (int i = 0; i < 61; i++){
    polygon2[i] = G4TwoVector((insideRadius+wallThickness-WindowThickness)*cos(shmsWindowAngle+i*deg),(insideRadius+wallThickness-WindowThickness)*sin(shmsWindowAngle+i*deg));
  }
  for (int i = 0; i < 61; i++){
    polygon2[61+i] = G4TwoVector((insideRadius-1.0*cm)*cos(shmsWindowAngle+shmsWindowDeltaAngle-i*deg),(insideRadius-1.0*cm)*sin(shmsWindowAngle+shmsWindowDeltaAngle-i*deg));
  }
  std::vector<G4ExtrudedSolid::ZSection> zsections4;
  zsections4.push_back(G4ExtrudedSolid::ZSection(-shmsWindowHeight/2, G4TwoVector(0.0*cm, 0.0*cm), 1.0));
  zsections4.push_back(G4ExtrudedSolid::ZSection(+shmsWindowHeight/2, G4TwoVector(0.0*cm, 0.0*cm), 1.0));
  G4ExtrudedSolid *shmsWindow = new G4ExtrudedSolid("shmsWindow", polygon2, zsections4);


  // Boolean operations to form the final scattering champer
  G4SubtractionSolid *chamberWall = new G4SubtractionSolid("ChamberWall",solidChamber,innerChamber);
  G4Tubs *beamPipeHoles = new G4Tubs("BeamPipeHoles", 0*m, holeRadius, insideRadius+1*m, 0*deg, 360*deg);
  G4SubtractionSolid *chamberWallWithHoles = 
    new G4SubtractionSolid("ChamberWallWithHoles",chamberWall, beamPipeHoles, transform);
  G4SubtractionSolid *chamberWallWithWindow = 
    new G4SubtractionSolid("ChamberWallWithWindow", chamberWallWithHoles, npolWindow);
  G4SubtractionSolid *chamberWallWithSHMS = new 
    G4SubtractionSolid("ChamberWallWithSHMS", chamberWallWithWindow, shmsWindow);
  
  innerChamberLV = new G4LogicalVolume(innerChamber,
       NpolMaterials::GetInstance()->GetVacuum(), "InnerChamberLV, 0,0,0");
  chamberWallLV = new G4LogicalVolume(chamberWallWithSHMS,
       NpolMaterials::GetInstance()->GetAl(), "ChamberWallLV", 0,0,0);
}

void NpolScatteringChamber::Place(G4LogicalVolume *motherLV) {
  
  target->Place(innerChamberLV);
  PlaceRectangular(chamberWallLV, motherLV, "ChamberWall", 0,0,0,90*deg,90*deg,0);
  PlaceRectangular(innerChamberLV,  motherLV, "InnerChamber", 0,0,0,90*deg,0,0);
}

