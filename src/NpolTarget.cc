//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************
//* The Geant4 software is used by the Northern Michigan University  *
//* in accordance to the Geant4 software license specified above.    *
//* The NMU Collaboration does not hold any rights to this software  *
//* and gives full permission for its use to others under the limits *
//* imposed by the GEANT4 Collaboration.  The NMU Collaboration      *
//* gives no express or implied warranty and use of our code is at   *
//* the users discretion only.  		    			*
//********************************************************************

#include "G4PhysicalConstants.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4AssemblyVolume.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4ios.hh"

#include "NpolMaterials.hh"
#include "NpolTarget.hh"

NpolTarget::NpolTarget() {
	G4cout << "Initializing Target" << G4endl;
}

NpolTarget::~NpolTarget() {
	G4cout << "Deleting Target" << G4endl;
}

// Construct the target can in which we place the fluid
void NpolTarget::ConstructTargetCan(G4LogicalVolume *motherLV) {

	G4Tubs *TargetCan = new G4Tubs("TargetCan",0.0*cm, 2.2*cm, 10.2*cm, 0.0*deg, 360.*deg);
	TargetCanLV = new G4LogicalVolume(TargetCan,NpolMaterials::GetInstance()->GetAl(),"TargetCanLV",0,0,0);
	new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, -8.7*m), TargetCanLV,"TargetCan", motherLV, false, 0);

	G4VisAttributes *TopVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,0.50));
	TargetCanLV->SetVisAttributes(TopVisAtt);
}

// Construct the target fluid and place inside the target can
void NpolTarget::ConstructTargetFluid(G4LogicalVolume *motherLV) {

	G4Tubs *TargetFluid = new G4Tubs("TargetFluid",0.0*cm, 2.0*cm, 10.0*cm, 0.0*deg, 360.*deg);
	TargetFluidLV = new G4LogicalVolume(TargetFluid,NpolMaterials::GetInstance()->GetLD2(),"TargetFluidLV",0,0,0);
	new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, 0.0*m), TargetFluidLV,"TargetFluid", motherLV, false, 0);
	G4VisAttributes *TopVisAtt= new G4VisAttributes(G4Colour(0.0,1.0,0.0));
	TargetFluidLV->SetVisAttributes(TopVisAtt);
}

void NpolTarget::Construct(G4LogicalVolume *motherLV) {

	ConstructTargetCan(motherLV);
	ConstructTargetFluid(TargetCanLV);
}

