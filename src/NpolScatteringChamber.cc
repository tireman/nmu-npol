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
// Created: William Tireman - December 2014

#include "globals.hh"
#include "G4PhysicalConstants.hh"
#include "G4Material.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4ios.hh"

#include "NpolMaterials.hh"
#include "NpolTarget.hh"
#include "NpolScatteringChamber.hh"

NpolScatteringChamber::NpolScatteringChamber() {
	G4cout << "Initializing Scattering Chamber" << G4endl;
}

NpolScatteringChamber::~NpolScatteringChamber() {
	G4cout << "Deleting Scattering Chamber" << G4endl;
}

void NpolScatteringChamber::ConstructChamber() {

	const G4double insideRadius = 0.5*1.041*m;
	const G4double insideHeight = 1.2*m;
	const G4double wallThickness = 1.02*cm;
	const G4double holeRadius = 2.5*cm;

	G4ThreeVector translation = G4ThreeVector(0,0,0);
	G4RotationMatrix yRot90deg;
	yRot90deg.rotateY(90*deg);
	G4Transform3D transform = G4Transform3D(yRot90deg, translation);

	G4Tubs *innerChamber = new G4Tubs("InnerChamber", 0*m, insideRadius, insideHeight, 0*deg, 360*deg);
	G4Tubs *chamberWall = new G4Tubs("ChamberWall", insideRadius, insideRadius+wallThickness,
			insideHeight+(2*wallThickness), 0*deg, 360*deg);
	G4Tubs *beamPipeHoles = new G4Tubs("BeamPipeHoles", 0*m, holeRadius, insideRadius+1*m, 0*deg, 360*deg);
	G4SubtractionSolid *chamberWallWithHoles = new G4SubtractionSolid("ChamberWallWithHoles",
			chamberWall, beamPipeHoles, transform);

	innerChamberLV = new G4LogicalVolume(innerChamber,
			NpolMaterials::GetInstance()->GetVacuum(), "InnerChamberLV, 0,0,0");
	chamberWallLV = new G4LogicalVolume(chamberWallWithHoles,
			NpolMaterials::GetInstance()->GetAl(), "ChamberWallLV", 0,0,0);

	innerChamberLV->SetVisAttributes(new G4VisAttributes(G4VisAttributes::GetInvisible()));
}

G4VPhysicalVolume *NpolScatteringChamber::Construct(G4LogicalVolume *motherLV) {

	NpolTarget *target = new NpolTarget();

	ConstructChamber();

	target->Construct(innerChamberLV);

	PlaceRectangular(chamberWallLV, motherLV, "ChamberWall", 0,0,0,90*deg,90*deg,0);
	PlaceRectangular(innerChamberLV,  motherLV, "InnerChamber", 0,0,0,90*deg,0,0);
}

