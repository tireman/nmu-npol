//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//******************************************************************

// %% NpolTarget.cc %%

// Target Constructor file.  Currently only as target and target can.
// Created: William Tireman - December 2014
// Modified: Daniel Wilbern - December 2014

#include "G4PhysicalConstants.hh"
#include "G4Material.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4ios.hh"

#include "NpolMaterials.hh"
#include "NpolTarget.hh"

G4double NpolTarget::TargetCellDiameter = 6.350*cm;
G4double NpolTarget::TargetCellLength = 40.0*cm;
G4double NpolTarget::TargetCellUpStreamWindow = 0.01016*cm;
G4double NpolTarget::TargetCellDownStreamWindow = 0.01016*cm;
G4double NpolTarget::TargetCellSideWindow = 0.01778*cm;

NpolTarget::NpolTarget() {
	ConstructTargetCan();
	ConstructTargetFluid();
}

NpolTarget::~NpolTarget() {}

G4String NpolTarget::GetName() {
	return G4String("Target");
}

// Construct the target can in which we place the fluid
void NpolTarget::ConstructTargetCan() {
  
  G4Tubs *TargetCan = new G4Tubs("TargetCan",0.0*cm,(TargetCellDiameter/2),
 (TargetCellLength/2), 0.0*deg, 360.*deg);
  TargetCanLV = new G4LogicalVolume(TargetCan, 
        NpolMaterials::GetInstance()->GetAl(),"TargetCanLV",0,0,0);

  G4VisAttributes *TopVisAtt= new G4VisAttributes(G4Colour(1.0,0.0,0.0));
  TargetCanLV->SetVisAttributes(TopVisAtt);
}

// Construct the target fluid and place inside the target can
void NpolTarget::ConstructTargetFluid() {

  G4Tubs *TargetFluid = new G4Tubs("TargetFluid",0.0*cm, (TargetCellDiameter/2-TargetCellSideWindow), (TargetCellLength/2-TargetCellUpStreamWindow), 0.0*deg, 360.*deg);
  TargetFluidLV = new G4LogicalVolume(TargetFluid,
	NpolMaterials::GetInstance()->GetLD2(),"TargetFluidLV",0,0,0);
  
  G4VisAttributes *TopVisAtt= new G4VisAttributes(G4Colour(0.0,1.0,0.0));
  TargetFluidLV->SetVisAttributes(TopVisAtt);
}

void NpolTarget::Place(G4LogicalVolume *motherLV) {

  PlaceRectangular(TargetCanLV, motherLV, "TargetCan", 0,0,0,90*deg,0,0);
  PlaceRectangular(TargetFluidLV, TargetCanLV, "TargetFluid", 0,0,0); 
}

