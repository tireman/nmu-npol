//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************

// %% NpolHallShell.cc %%

// Hall C outer wall, floor, and dome ceiling constructor
// Created: William Tireman - December 2014
// Modified: Daniel Wilbern - December 2014
// Updated: W. Tireman - January 2015 

#include "G4PhysicalConstants.hh"
#include "G4Material.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4SubtractionSolid.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4ios.hh"

#include "NpolMaterials.hh"
#include "NpolHallShell.hh"

// inside diameter and outside diameter of the hall; hall height; concrete
// is set to 2.0m thick (guess 1/1/15) and I used a roof diamter of 42m 
// to get a curve that from a sphere will cover the shell roof.  Estimated.
G4double NpolHallShell::insideRadius = 22.860*m;
G4double NpolHallShell::floorRadius = 24.7744*m;
G4double NpolHallShell::shellHeight = 7.0*m;
G4double NpolHallShell::roofRadius = 42.0*m;
G4double NpolHallShell::creteThick = 2.0*m;
G4double NpolHallShell::xPlacementOffset = -4.700*m;
G4double NpolHallShell::zPlacementOffset = 4.700*m;

NpolHallShell::NpolHallShell() {
	ConstructHallShellWall();
	ConstructHallShellFloor();
	ConstructHallShellRoof();
}

NpolHallShell::~NpolHallShell() {}

G4String NpolHallShell::GetName() {
	return G4String("Hall Shell");
}

// Construct the Hall Wall in the world; cylinder of concrete
void NpolHallShell::ConstructHallShellWall() {
  
  G4Tubs *Wall = new G4Tubs("Wall", insideRadius, floorRadius, shellHeight, 0.0*deg, 360.*deg);
  G4Tubs *Hole = new G4Tubs("Hole", insideRadius-20.0*cm, floorRadius+20.0*cm, shellHeight/8, 76.0*deg, 5.5*deg);
  
  G4RotationMatrix *Rot = new G4RotationMatrix;
  G4ThreeVector zTrans(+0.0*m, 0.0*m, +3.0*m);
  
  G4SubtractionSolid *HallShellWall = new G4SubtractionSolid("HallShellWall", Wall, Hole, Rot, zTrans);
  
  HallShellWallLV = new G4LogicalVolume(HallShellWall,
     NpolMaterials::GetInstance()->GetMaterial("Concrete"),"HallShellWallLV",0,0,0);
  
  //G4VisAttributes *TopVisAtt= new G4VisAttributes(G4Colour(0.95, 0.9, 0.95)); //0.60,0.80,0.20));
  //HallShellWallLV->SetVisAttributes(TopVisAtt);
  HallShellWallLV->SetVisAttributes(G4VisAttributes::GetInvisible());
}

// Construct the Hall Floor in the world
void NpolHallShell::ConstructHallShellFloor() {
  
  G4Tubs *HallShellFloor = new G4Tubs("HallShellFloor",0.0*m, floorRadius, 
				      (creteThick/2), 0.0*deg, 360.*deg);
  HallShellFloorLV = new G4LogicalVolume(HallShellFloor,
      NpolMaterials::GetInstance()->GetMaterial("Concrete"),"HallShellFloorLV",0,0,0);
  
  //G4VisAttributes *TopVisAtt= new G4VisAttributes(G4Colour( 0.25, 0.5, 0.25));//0.70,0.70,0.90));
  //HallShellFloorLV->SetVisAttributes(TopVisAtt);
  HallShellFloorLV->SetVisAttributes(G4VisAttributes::GetInvisible());
}

// Construct the Hall Roof in the world
void NpolHallShell::ConstructHallShellRoof() {

  G4Sphere *HallShellRoof = new G4Sphere("HallShellRoof",(roofRadius-creteThick),roofRadius, 0.0*deg, 360.0*deg, 0.0*deg, 37.0*deg);
  HallShellRoofLV = new G4LogicalVolume(HallShellRoof,
     NpolMaterials::GetInstance()->GetMaterial("Concrete"),"HallShellRoofLV",0,0,0);
  
  //G4VisAttributes *TopVisAtt= new G4VisAttributes(G4Colour(0.7,1.0,0.5));
  //HallShellRoofLV->SetVisAttributes(TopVisAtt);
  HallShellRoofLV->SetVisAttributes(G4VisAttributes::GetInvisible());
}

void NpolHallShell::Place(G4LogicalVolume *motherLV) {

	PlaceRectangular(HallShellRoofLV, motherLV, "HallShellRoof", 
			-4.700*m, -21.60*m, +4.700*m, -90*deg, 0*deg, 0*deg);
	PlaceRectangular(HallShellFloorLV, motherLV, "HallShellFloor", 
			-4.700*m, -5.00*m, +4.700*m, -90*deg, 0*deg, 0*deg);
	PlaceRectangular(HallShellWallLV, motherLV, "HallShellWall", 
			-4.700*m,+3.00*m,+4.700*m, 90*deg, 0*deg, 0*deg);
}

