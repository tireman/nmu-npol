// %% NpolHallShell.cc %%

// Hall C outer wall, floor, and dome ceiling constructor
// Created: William Tireman - December 2014
// Modified: Daniel Wilbern - December 2014
// Updated: W. Tireman - January 2015 

#include "G4PhysicalConstants.hh"
#include "G4Material.hh"
#include "G4Tubs.hh"
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
G4double inDia = 22.860*m, outDia = 24.7744*m, shellHeight = 7.0*m;
G4double roofDia = 42.0*m, creteThick=2.0*m;

NpolHallShell::NpolHallShell() {

  G4cout << "Initializing Hall Shell: Wall, floor, roof" << G4endl;
}

NpolHallShell::~NpolHallShell() {
  G4cout << "Deleting Hall Shell: Wall, floor, roof" << G4endl;
}

// Construct the Hall Wall in the world; cylinder of concrete
void NpolHallShell::ConstructHallShellWall() {
  
  G4Tubs *HallShellWall = new G4Tubs("HallShellWall", inDia, outDia, 
        shellHeight, 0.0*deg, 360.*deg);
  HallShellWallLV = new G4LogicalVolume(HallShellWall,
	NpolMaterials::GetInstance()->GetConcrete(),"HallShellWallLV",0,0,0);
  
  //G4VisAttributes *TopVisAtt= new G4VisAttributes(G4Colour(2.0,1.0,0.0));
  //HallShellWallLV->SetVisAttributes(TopVisAtt);
  HallShellWallLV->SetVisAttributes(G4VisAttributes::GetInvisible());
}

// Construct the Hall Floor in the world
void NpolHallShell::ConstructHallShellFloor() {
  
  G4Tubs *HallShellFloor = new G4Tubs("HallShellFloor",0.0*m, outDia, 
	 (creteThick/2), 0.0*deg, 360.*deg);
  HallShellFloorLV = new G4LogicalVolume(HallShellFloor,
	 NpolMaterials::GetInstance()->GetConcrete(),"HallShellFloorLV",0,0,0);
  
  //G4VisAttributes *TopVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  //HallShellFloorLV->SetVisAttributes(TopVisAtt);
  HallShellFloorLV->SetVisAttributes(G4VisAttributes::GetInvisible());
}

// Construct the Hall Roof in the world
void NpolHallShell::ConstructHallShellRoof() {
  
  G4Sphere *HallShellRoof = new G4Sphere("HallShellRoof",(roofDia-creteThick), 
        roofDia, 0.0*deg, 360.0*deg, 0.0*deg, 38.0*deg);
  HallShellRoofLV = new G4LogicalVolume(HallShellRoof,
	NpolMaterials::GetInstance()->GetConcrete(),"HallShellRoofLV",0,0,0);
  
  //G4VisAttributes *TopVisAtt= new G4VisAttributes(G4Colour(0.0,1.0,2.0));
  //HallShellRoofLV->SetVisAttributes(TopVisAtt);
  HallShellRoofLV->SetVisAttributes(G4VisAttributes::GetInvisible());
}

G4VPhysicalVolume *NpolHallShell::Construct(G4LogicalVolume *motherLV) {
  
  ConstructHallShellWall();
  ConstructHallShellFloor();
  ConstructHallShellRoof();
  
  PlaceRectangular(HallShellRoofLV, motherLV, "HallShellRoof", 
	   -4.700*m, -21.60*m, +4.700*m, -90*deg, 0*deg, 0*deg);
  PlaceRectangular(HallShellFloorLV, motherLV, "HallShellFloor", 
	   -4.700*m, -5.00*m, +4.700*m, -90*deg, 0*deg, 0*deg);
  return PlaceRectangular(HallShellWallLV, motherLV, "HallShellWall", 
   	   -4.700*m,+3.00*m,+4.700*m, 90*deg, 0*deg, 0*deg);
}

