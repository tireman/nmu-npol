//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.							 *
//********************************************************************
//* The Geant4 software is used by the Northern Michigan University  *
//* in accordance to the Geant4 software license specified above.    *
//* The NMU Collaboration does not hold any rights to this software  *
//* and gives full permission for its use to others under the limits *
//* imposed by the GEANT4 Collaboration.  The NMU Collaboration      *
//* gives no express or implied warranty and use of our code is at   *
//* the users discretion only.										 *
//********************************************************************

#include "G4SystemOfUnits.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"

#include "NpolWorld.hh"
#include "NpolMaterials.hh"

NpolWorld::NpolWorld() {
	G4cout << "Initializing world" << G4endl;
}

NpolWorld::~NpolWorld() {
	G4cout << "Deleting world" << G4endl;
}

void NpolWorld::Construct(G4LogicalVolume *motherLV) {

	G4Box *expHall = new G4Box("ExpHall", 2.50*m, 3.0*m, 10.0*m);
	
	expHallLV = new G4LogicalVolume(expHall, 
		NpolMaterials::GetInstance()->GetAir(), "ExpHallLV", 0, 0, 0);
	expHallPV = new G4PVPlacement(0, G4ThreeVector(), expHallLV, "ExpHall", 0, false, 0);

	expHallLV->SetVisAttributes(G4VisAttributes::GetInvisible());
}

G4VPhysicalVolume *NpolWorld::GetWorldPV() {
	return expHallPV;
}

G4LogicalVolume *NpolWorld::GetWorldLV() {
	return expHallLV;
}

