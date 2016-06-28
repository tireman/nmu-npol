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
  ConstructNPOLTagger();
  ConstructLeadCurtain();
}

NpolShieldHut::~NpolShieldHut() {}

G4String NpolShieldHut::GetName() {
  return G4String("Shield Hut");
}

// Construct a lead shield for in front of the polarimeter
void NpolShieldHut::ConstructLeadCurtain(){
  G4double xlen = 0.85*m; G4double ylen = 0.55*m; G4double zlen = 0.150*m;

  G4Box *LeadCurtain = new G4Box("LeadCurtain",xlen/2,ylen/2,zlen/2);
  LeadCurtainLV = 
	new G4LogicalVolume(LeadCurtain,NpolMaterials::GetInstance()->GetMaterial("Pb"),"LeadCurtainLV",0,0,0);
  G4VisAttributes *LeadCurtainVisAtt = new G4VisAttributes(G4Colour(1.0, 0.7, 0.2));
  LeadCurtainLV->SetVisAttributes(LeadCurtainVisAtt);
}

// Construct a thin air box so we can tag particles passing through the collimator.  Place it just a millimeter off the front steel wall
void NpolShieldHut::ConstructNPOLTagger(){
  G4double xlen = 0.98*m; G4double ylen = 0.600*m; G4double zlen = 0.010*cm;

  G4Box *NPOLTagger = new G4Box("NPOLTagger",xlen/2,ylen/2,zlen/2);
  NPOLTaggerLV = 
	new G4LogicalVolume(NPOLTagger,NpolMaterials::GetInstance()->GetMaterial("HardVacuum"),"NPOLTaggerLV",0,0,0);
  G4VisAttributes *TaggerVisAtt = new G4VisAttributes(G4Colour(0.2, 0.2, 0.2));
  NPOLTaggerLV->SetVisAttributes(TaggerVisAtt);
}

// Construct the front wall of the shield hut from 4 ft by 4 ft by 3 ft blocks
// will simplfy here to a 3 foot deep wall that is 16 feet wide and 15 feet high
void NpolShieldHut::ConstructHutFrontWall() {
  // constants for sizing and positioning
  G4double xlen = 4.8768*m, ylen = 7.3152*m, zlen = 0.9144*m;
  G4double xlen1 = 0.81854*m, xlen2 = 0.97618*m;
  G4double ylen1 = 0.5012*m, ylen2 = 0.59574*m, VertOffSet = 0.3424*m;

  // Create the necessary solids
  G4Box *Sheet = new G4Box("Sheet",xlen/2, ylen/2, zlen/2);
  G4Trd *Collimator = new G4Trd("Collimator",xlen1/2, xlen2/2, ylen1/2, ylen2/2, (zlen+0.10*m)/2);
  
  // Rotation and translation information for the hole for the beam line
  G4RotationMatrix *yRot = new G4RotationMatrix;
  G4ThreeVector xTrans(+0.0*m, VertOffSet, 0.0*m);
  
  // Create the solid using SubtractionSolid
  G4SubtractionSolid *HutFrontWall = new G4SubtractionSolid("HutFrontWall", Sheet, Collimator, yRot, xTrans);
  
  // Generate the logical volume
  HutFrontWallLV = 
	new G4LogicalVolume(HutFrontWall,NpolMaterials::GetInstance()->GetMaterial("SSteel"),"HutFrontWallLV",0,0,0);
  
  G4VisAttributes *FrontWallVisAtt= new G4VisAttributes(G4Colour(0.0,1.5,0.0));
  HutFrontWallLV->SetVisAttributes(FrontWallVisAtt);
}

// Construct the back wall of the shield hut from concrete blocks here we
// will simplfy to a 3 foot deep wall that is 16 feet wide and 24 feet high
void NpolShieldHut::ConstructHutBackWall() {
  G4double xlen = 4.8768*m, ylen = 7.3152*m, zlen = 0.9144*m;
  
  G4Box *HutBackWall = new G4Box("HutBackWall",xlen/2, ylen/2, zlen/2);
  HutBackWallLV = new G4LogicalVolume(HutBackWall,
				      NpolMaterials::GetInstance()->GetMaterial("SSteel"),"HutBackWallLV",0,0,0);
  G4VisAttributes *BackWallVisAtt= new G4VisAttributes(G4Colour(0.0,1.5,0.0));
  HutBackWallLV->SetVisAttributes(BackWallVisAtt);
}

// Construct the side walls (one construct, two placements) for the shield 
// hut out of concrete.
void NpolShieldHut::ConstructHutSideWall() {
  G4double xlen = 0.9144*m, ylen = 7.3152*m, zlen = 5.50*m-xlen-0.0050*m;
  
  G4Box *HutSideWall = new G4Box("HutSideWall",xlen/2, ylen/2, zlen/2);
  HutSideWallLV = new G4LogicalVolume(HutSideWall,
      NpolMaterials::GetInstance()->GetMaterial("Concrete"),"HutSideWallLV",0,0,0);
  G4VisAttributes *SideWallVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  HutSideWallLV->SetVisAttributes(SideWallVisAtt);
}

// Construct the roof.  The concrete blocks are, if memory serves right, 
// 18 inches in thickness.  In E93-038 we used two layers.  So, we will
// start with 36 inches

void NpolShieldHut::ConstructHutRoof() {
  
  G4double xlen = 4.8768*m, ylen = 0.9144*m, zlen = 6.25*m;
  G4Box *HutRoof = new G4Box("HutRoof", xlen/2, ylen/2, zlen/2);
  HutRoofLV = new G4LogicalVolume(HutRoof,NpolMaterials::GetInstance()->GetMaterial("Concrete"), "HutRoofLV",0,0,0);
  G4VisAttributes *RoofVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  HutRoofLV->SetVisAttributes(RoofVisAtt);
}

void NpolShieldHut::Place(G4LogicalVolume *motherLV) {
  
  G4double NpolAng = 28.0*deg, PosSide = 9.4025*m, AngSide = 14.0*deg;
  G4double VertOffSet = 0.3424*m, PosFront = 6.3739*m, PosBack = 11.8739*m; // PosFront = 6.2739*m, PosBack = 11.7739*m;
  G4double PosRoof = 9.1239*m, OffSetRoof = 3.7776*m;
  G4double PosLead = PosFront - 0.535*m, PosTagger = PosFront + 0.465*m;

  PlaceCylindrical(LeadCurtainLV, motherLV, "LeadCurtain", PosLead,-NpolAng, 0);
  PlaceCylindrical(NPOLTaggerLV, motherLV, "NPOLTagger", PosTagger, -NpolAng, 0);
  PlaceCylindrical(HutFrontWallLV, motherLV, "HutFrontWall", PosFront,-NpolAng,-VertOffSet);
  PlaceCylindrical(HutBackWallLV, motherLV, "HutBackWall", PosBack,-NpolAng,-VertOffSet);
  PlaceRectangular(HutSideWallLV, motherLV, "HutSideWall", -PosSide*sin(AngSide+NpolAng),-VertOffSet, PosSide*cos(AngSide+NpolAng), 0*deg, -NpolAng, 0*deg);
  PlaceRectangular(HutSideWallLV, motherLV, "HutSideWall", -PosSide*sin(AngSide),-VertOffSet, PosSide*cos(AngSide), 0*deg, -NpolAng, 0);
  PlaceCylindrical(HutRoofLV, motherLV, "HutRoof", PosRoof, -NpolAng, OffSetRoof);
}

