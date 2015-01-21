// %% NpolShieldHut.cc %%

// Polarimeter Shield hut construction file
// Created: William Tireman - January 2015

#include "G4PhysicalConstants.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4ios.hh"

#include "NpolMaterials.hh"
#include "NpolShieldHut.hh"

NpolShieldHut::NpolShieldHut() {
  G4cout << "Initializing Shield Hut" << G4endl;
  G4cout << "... Front wall" << G4endl;
  G4cout << "... Side walls" << G4endl;
  G4cout << "... Back wall" << G4endl;
  G4cout << "... Roof" << G4endl;
}

NpolShieldHut::~NpolShieldHut() {
  G4cout << "Deleting Shield Hut" << G4endl;
  G4cout << "... Front wall" << G4endl;
  G4cout << "... Side walls" << G4endl;
  G4cout << "... Back wall" << G4endl;
  G4cout << "... Roof" << G4endl;
}

// Construct the front wall of the shield hut from 4 ft by 4 ft by 3 ft blocks
// will simplfy here to a 3 foot deep wall that is 16 feet wide and 15 feet high
void NpolShieldHut::ConstructHutFrontWall() {
  G4double xlen = 4.8768*m, ylen = 7.3152*m, zlen = 0.9144*m;

  G4Box *HutFrontWall = new G4Box("HutFrontWall",xlen/2, ylen/2, zlen/2);
  HutFrontWallLV = new G4LogicalVolume(HutFrontWall,
           NpolMaterials::GetInstance()->GetFe(),"HutFrontWallLV",0,0,0);
  G4VisAttributes *FrontWallVisAtt= new G4VisAttributes(G4Colour(0.0,1.5,0.0));
  HutFrontWallLV->SetVisAttributes(FrontWallVisAtt);
}

// Construct the collimator opening in the front shield wall and fill with air
void NpolShieldHut::ConstructHutCollimator() {
  // Define xlen1, xlen2, ylen1, ylen2, and zlen has the half lengths
  G4double xlen1 = 0.33427*m, xlen2 = 0.41309*m;
  G4double ylen1 = 0.2006*m, ylen2 = 0.24787*m, zlen = 0.4572*m;
  
  G4Trd *HutCollimator = new G4Trd("HutCollimator",xlen1, xlen2, ylen1, ylen2, zlen);  
  HutCollimatorLV = new G4LogicalVolume(HutCollimator, NpolMaterials::GetInstance()->GetAir(),
	"HutCollimatorLV",0,0,0);
  G4VisAttributes *CollimatorVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  HutCollimatorLV->SetVisAttributes(CollimatorVisAtt);
  //HutCollimatorLV->SetVisAttributes(G4VisAttributes::GetInvisible());
}

// Construct the back wall of the shield hut from concrete blocks here we
// will simplfy to a 3 foot deep wall that is 16 feet wide and 24 feet high
void NpolShieldHut::ConstructHutBackWall() {
  G4double xlen = 4.8768*m, ylen = 7.3152*m, zlen = 0.9144*m;

  G4Box *HutBackWall = new G4Box("HutBackWall",xlen/2, ylen/2, zlen/2);
  HutBackWallLV = new G4LogicalVolume(HutBackWall,
           NpolMaterials::GetInstance()->GetFe(),"HutBackWallLV",0,0,0);
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

// Construct the roof.  The concrete blocks are, if memory serves right, 18 
// inches in thickness.  In E93-038 we used two layers.  So, we will start
// with 36 inches

void NpolShieldHut::ConstructHutRoof() {

  G4double xlen = 4.8768*m, ylen = 0.9144*m, zlen = 6.25*m;
  G4Box *HutRoof = new G4Box("HutRoof", xlen/2, ylen/2, zlen/2);
  HutRoofLV = new G4LogicalVolume(HutRoof,NpolMaterials::GetInstance()->GetConcrete(),
       "HutRoofLV",0,0,0);
  G4VisAttributes *RoofVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  HutRoofLV->SetVisAttributes(RoofVisAtt);
}

G4VPhysicalVolume *NpolShieldHut::Construct(G4LogicalVolume *motherLV) {
  
  G4double NpolAng = 28.0*deg, PosSide = 9.3025*m, AngSide = 14.0*deg, VertOffSet = 0.3424*m;
  G4double PosFront = 6.2739*m, PosBack = 11.7739*m, PosRoof = 9.0239*m, OffSetRoof = 3.7776*m;
  ConstructHutFrontWall();
  ConstructHutCollimator();
  ConstructHutBackWall();
  ConstructHutSideWall();
  ConstructHutRoof();

  PlaceCylindrical(HutFrontWallLV, motherLV, "HutFrontWall", PosFront,-NpolAng,-VertOffSet);
  PlaceCylindrical(HutCollimatorLV, HutFrontWallLV, "HutCollimator", 0,0,+VertOffSet);
  PlaceCylindrical(HutBackWallLV, motherLV, "HutBackWall", PosBack,-NpolAng,-VertOffSet);
  PlaceRectangular(HutSideWallLV, motherLV, "HutSideWall", -PosSide*sin(AngSide+NpolAng), -VertOffSet, PosSide*cos(AngSide+NpolAng), 0*deg, -NpolAng, 0*deg);
  PlaceRectangular(HutSideWallLV, motherLV, "HutSideWall", -PosSide*sin(AngSide), -VertOffSet, PosSide*cos(AngSide), 0*deg, -NpolAng, 0);
  return PlaceCylindrical(HutRoofLV, motherLV, "HutRoof", PosRoof, -NpolAng, OffSetRoof);
}

