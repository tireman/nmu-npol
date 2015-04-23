//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//******************************************************************

// %% NpolShieldHut.cc %%

// Polarimeter Shield hut construction file
// Created: William Tireman - January 2015

#include "G4PhysicalConstants.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Trd.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4ios.hh"

#include "NpolMaterials.hh"
#include "NpolShieldHut.hh"

NpolShieldHut::NpolShieldHut() {
	ConstructHutFrontWall();
	ConstructHutBackWall();
	ConstructHutSideWall();
	ConstructHutRoof();
}

NpolShieldHut::~NpolShieldHut() {}

G4String NpolShieldHut::GetName() {
	return G4String("Shield Hut");
}

// Construct the front wall of the shield hut from 4 ft by 4 ft by 3 ft blocks
// will simplfy here to a 3 foot deep wall that is 16 feet wide and 15 feet high
void NpolShieldHut::ConstructHutFrontWall() {
	// constants for sizing and positioning
	G4double xlen = 4.8768*m, ylen = 7.3152*m, zlen = 0.9144*m;
	G4double xlen1 = 0.66854*m, xlen2 = 0.82618*m;
	G4double ylen1 = 0.4012*m, ylen2 = 0.49574*m, VertOffSet = 0.3424*m;

	// Create the necessary solids
	G4Box *Sheet = new G4Box("Sheet",xlen/2, ylen/2, zlen/2);
	G4Trd *Collimator = new G4Trd("Collimator",xlen1/2, xlen2/2, ylen1/2, ylen2/2, (zlen+0.10*m)/2);

	// Rotation and translation information for the hole for the beam line
	G4RotationMatrix *yRot = new G4RotationMatrix;
	G4ThreeVector xTrans(+0.0*m, VertOffSet, 0.0*m);

	// Create the solid using SubtractionSolid
	G4SubtractionSolid *HutFrontWall = new G4SubtractionSolid("HutFrontWall", Sheet, Collimator, yRot, xTrans);

	// Generate the logical volume
	HutFrontWallLV = new G4LogicalVolume(HutFrontWall,NpolMaterials::GetInstance()->GetSSteel(),"HutFrontWallLV",0,0,0);

	G4VisAttributes *FrontWallVisAtt= new G4VisAttributes(G4Colour(0.0,1.5,0.0));
	HutFrontWallLV->SetVisAttributes(FrontWallVisAtt);
}

// Construct the back wall of the shield hut from concrete blocks here we
// will simplfy to a 3 foot deep wall that is 16 feet wide and 24 feet high
void NpolShieldHut::ConstructHutBackWall() {
	G4double xlen = 4.8768*m, ylen = 7.3152*m, zlen = 0.9144*m;

	G4Box *HutBackWall = new G4Box("HutBackWall",xlen/2, ylen/2, zlen/2);
	HutBackWallLV = new G4LogicalVolume(HutBackWall,
			NpolMaterials::GetInstance()->GetSSteel(),"HutBackWallLV",0,0,0);
	G4VisAttributes *BackWallVisAtt= new G4VisAttributes(G4Colour(0.0,1.5,0.0));
	HutBackWallLV->SetVisAttributes(BackWallVisAtt);
}

// Construct the side walls (one construct, two placements) for the shield 
// hut out of concrete.
void NpolShieldHut::ConstructHutSideWall() {
	G4double xlen = 0.9144*m, ylen = 7.3152*m, zlen = 5.50*m-xlen;

	G4Box *HutSideWall = new G4Box("HutSideWall",xlen/2, ylen/2, zlen/2);
	HutSideWallLV = new G4LogicalVolume(HutSideWall,
			NpolMaterials::GetInstance()->GetConcrete(),"HutSideWallLV",0,0,0);
	G4VisAttributes *SideWallVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
	HutSideWallLV->SetVisAttributes(SideWallVisAtt);
}

// Construct the roof.  The concrete blocks are, if memory serves right, 
// 18 inches in thickness.  In E93-038 we used two layers.  So, we will
// start with 36 inches

void NpolShieldHut::ConstructHutRoof() {

	G4double xlen = 4.8768*m, ylen = 0.9144*m, zlen = 6.25*m;
	G4Box *HutRoof = new G4Box("HutRoof", xlen/2, ylen/2, zlen/2);
	HutRoofLV = new G4LogicalVolume(HutRoof,NpolMaterials::GetInstance()->GetConcrete(), "HutRoofLV",0,0,0);
	G4VisAttributes *RoofVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
	HutRoofLV->SetVisAttributes(RoofVisAtt);
}

void NpolShieldHut::Place(G4LogicalVolume *motherLV) {

	G4double NpolAng = 28.0*deg, PosSide = 9.3025*m, AngSide = 14.0*deg, VertOffSet = 0.3424*m;
	G4double PosFront = 6.2739*m, PosBack = 11.7739*m, PosRoof = 9.0239*m, OffSetRoof = 3.7776*m;

	PlaceCylindrical(HutFrontWallLV, motherLV, "HutFrontWall", PosFront,-NpolAng,-VertOffSet);
	PlaceCylindrical(HutBackWallLV, motherLV, "HutBackWall", PosBack,-NpolAng,-VertOffSet);
	PlaceRectangular(HutSideWallLV, motherLV, "HutSideWall", -PosSide*sin(AngSide+NpolAng), -VertOffSet, PosSide*cos(AngSide+NpolAng), 0*deg, -NpolAng, 0*deg);
	PlaceRectangular(HutSideWallLV, motherLV, "HutSideWall", -PosSide*sin(AngSide), -VertOffSet, PosSide*cos(AngSide), 0*deg, -NpolAng, 0);
	PlaceCylindrical(HutRoofLV, motherLV, "HutRoof", PosRoof, -NpolAng, OffSetRoof);
}

