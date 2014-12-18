// %% NpolHallShell.cc %%

// Hall C outer wall, floor, and dome ceiling constructor
// Created: William Tireman - December 2014
// Modified: Daniel Wilbern - December 2014

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

NpolHallShell::NpolHallShell() {
	G4cout << "Initializing Hall Shell: Wall, floor, roof" << G4endl;
}

NpolHallShell::~NpolHallShell() {
	G4cout << "Deleting Hall Shell: Wall, floor, roof" << G4endl;
}

// Construct the Hall Wall in the world
void NpolHallShell::ConstructHallShellWall() {

	G4Tubs *HallShellWall = new G4Tubs("HallShellWall",22.860*m, 23.7744*m, 
			15.0*m, 0.0*deg, 360.*deg);
	HallShellWallLV = new G4LogicalVolume(HallShellWall,
			NpolMaterials::GetInstance()->GetConcrete(),"HallShellWallLV",0,0,0);

	//G4VisAttributes *TopVisAtt= new G4VisAttributes(G4Colour(2.0,1.0,0.0));
	//HallShellWallLV->SetVisAttributes(TopVisAtt);
	HallShellWallLV->SetVisAttributes(G4VisAttributes::GetInvisible());
}

// Construct the Hall Floor in the world
void NpolHallShell::ConstructHallShellFloor() {

	G4Tubs *HallShellFloor = new G4Tubs("HallShellFloor",0.0*m, 23.7744*m, 
			1.0*m, 0.0*deg, 360.*deg);
	HallShellFloorLV = new G4LogicalVolume(HallShellFloor,
			NpolMaterials::GetInstance()->GetConcrete(),"HallShellFluiLV",0,0,0);

	//G4VisAttributes *TopVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
	//HallShellFloorLV->SetVisAttributes(TopVisAtt);
	HallShellFloorLV->SetVisAttributes(G4VisAttributes::GetInvisible());
}

// Construct the Hall Roof in the world
void NpolHallShell::ConstructHallShellRoof() {

	G4Sphere *HallShellRoof = new G4Sphere("HallShellRoof",40.0*m, 41.0*m, 0.0*deg, 360.0*deg, 0.0*deg, 37.0*deg);
	HallShellRoofLV = new G4LogicalVolume(HallShellRoof,
			NpolMaterials::GetInstance()->GetConcrete(),"HallShellFluiLV",0,0,0);

	// G4VisAttributes *TopVisAtt= new G4VisAttributes(G4Colour(0.0,1.0,2.0));
	//HallShellRoofLV->SetVisAttributes(TopVisAtt);
	HallShellRoofLV->SetVisAttributes(G4VisAttributes::GetInvisible());
}

G4VPhysicalVolume *NpolHallShell::Construct(G4LogicalVolume *motherLV) {

	ConstructHallShellWall();
	ConstructHallShellFloor();
	ConstructHallShellRoof();

	PlaceRectangular(HallShellRoofLV, motherLV, "HallShellRoof", 0*m, -17.0*m, 0*m, -90*deg, 0*deg, 0*deg);
	PlaceRectangular(HallShellFloorLV, motherLV, "HallShellFloor", 0*m, -16.0*m, 0*m, -90*deg, 0*deg, 0*deg);
	return PlaceRectangular(HallShellWallLV, motherLV, "HallShellWall", 0,0,0, 90*deg, 0*deg, 0*deg);
}

