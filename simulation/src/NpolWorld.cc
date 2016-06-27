//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************

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
  G4Box *expHall = new G4Box("ExpHall", 31.0*m, 30.0*m, 60.0*m); // Normal size
  //G4Box *expHall = new G4Box("ExpHall", 10.0*m, 10.0*m, 10.0*m); // HB test
  //G4Box *expHall = new G4Box("ExpHall", 8.0*m, 5.0*m, 10.0*m); // P/N only
  expHallLV = new G4LogicalVolume(expHall,NpolMaterials::GetInstance()
	 ->GetMaterial("HardVacuum"), "expHallLV", 0, 0, 0);
  expHallLV->SetVisAttributes(G4VisAttributes::GetInvisible());
}

NpolWorld::~NpolWorld() {}

G4String NpolWorld::GetName() {
	return G4String("NpolWorld");
}

void NpolWorld::Place(G4LogicalVolume *motherLV) {

	expHallPV = PlaceRectangular(expHallLV, motherLV, "ExpHall", 0,0,0);
}

G4VPhysicalVolume *NpolWorld::GetWorldPV() {
	return expHallPV;
}

G4LogicalVolume *NpolWorld::GetWorldLV() {
	return expHallLV;
}

