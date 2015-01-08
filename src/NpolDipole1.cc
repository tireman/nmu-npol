// %% NpolDipole1.cc %%

// Dipole 1 'Charybdis' Constructor file.  
// Created: William Tireman - December 2014


#include "G4PhysicalConstants.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4TwoVector.hh"
#include "G4ExtrudedSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4ios.hh"

#include "NpolMaterials.hh"
#include "NpolDipole1.hh"

NpolDipole1::NpolDipole1() {
  G4cout << "Initializing Dipole 1" << G4endl;
}

NpolDipole1::~NpolDipole1() {
  G4cout << "Deleting Dipole 1" << G4endl;
}

// Construct the yokes using the extruded class
void NpolDipole1::ConstructDipole1Yoke() {

  std::vector<G4TwoVector> polygon(12); // define the polygon to be extruded
  polygon[0] = G4TwoVector(-1.17475*m, 0.0*m);
  polygon[1] = G4TwoVector(-1.17475*m, 0.7112*m);
  polygon[2] = G4TwoVector(1.17475*m, 0.7112*m);
  polygon[3] = G4TwoVector(1.17475*m, 0.0*m);
  polygon[4] = G4TwoVector(0.67*m, 0.0*m);
  polygon[5] = G4TwoVector(0.67*m, 0.15*m);
  polygon[6] = G4TwoVector(0.27*m, 0.15*m);
  polygon[7] = G4TwoVector(0.23*m, 0.04*m);
  polygon[8] = G4TwoVector(-0.23*m,0.04*m);
  polygon[9] = G4TwoVector(-0.27*m, 0.15*m);
  polygon[10] = G4TwoVector(-0.67*m, 0.15*m);
  polygon[11] = G4TwoVector(-0.67*m, 0.0*m);

  G4ExtrudedSolid *Dipole1Yoke = new G4ExtrudedSolid("Dipole1Yoke", 
     polygon, 0.6096*m, G4TwoVector(0, 0), 1.0, G4TwoVector(0, 0), 1.0);
  Dipole1YokeLV = new G4LogicalVolume(Dipole1Yoke,
     NpolMaterials::GetInstance()->GetFe(),"Dipole1YokeLV",0,0,0);
  
  G4VisAttributes *Dipole1YokeVisAtt= 
    new G4VisAttributes(G4Colour(1.0,0.0,0.00));
  Dipole1YokeLV->SetVisAttributes(Dipole1YokeVisAtt);
}

void NpolDipole1::ConstructDipole1Coil(){

  std::vector<G4TwoVector> poligon(8);
  poligon[0] = G4TwoVector(-0.30*m,0);
  poligon[1] = G4TwoVector(-0.212*m,0.212*m);
  poligon[2] = G4TwoVector(0,0.30*m);
  poligon[3] = G4TwoVector(0.212*m,0.212*m);
  poligon[4] = G4TwoVector(0.30*m,0);
  poligon[5] = G4TwoVector(0.212*m,-0.212*m);
  poligon[6] = G4TwoVector(0,-0.30*m);
  poligon[7] = G4TwoVector(-0.212*m,-0.212*m);
  
  std::vector<G4ExtrudedSolid::ZSection> zsections;
  zsections.push_back(G4ExtrudedSolid::ZSection(0*cm,0*cm,1.0));
  zsections.push_back(G4ExtrudedSolid::ZSection(50*cm,(0*cm,30*cm),2.0));
  zsections.push_back(G4ExtrudedSolid::ZSection(100*cm,(30*cm,0*cm),0.50));
 
  G4ExtrudedSolid *Coil = new G4ExtrudedSolid("Coil", poligon,zsections);
  CoilLV = new G4LogicalVolume(Coil,NpolMaterials::GetInstance()->GetFe(),
    "CoilLV",0,0,0);
  G4VisAttributes *CoilVisAtt= 
    new G4VisAttributes(G4Colour(0.0,0.0,1.00));
  CoilLV->SetVisAttributes(CoilVisAtt);
}


G4VPhysicalVolume *NpolDipole1::Construct(G4LogicalVolume *motherLV) {
  G4double PosD1 = 2.5096*m, NpolAng = 28.0*deg;

  ConstructDipole1Yoke();
  //ConstructDipole1Coil();
  //PlaceCylindrical(CoilLV, motherLV, "Coil", 2.0*m, +28.0*deg, 0.0*m);
  PlaceCylindrical(Dipole1YokeLV, motherLV, "Dipole1", 
	PosD1,-NpolAng,+5.08*cm);
  return PlaceRectangular(Dipole1YokeLV, motherLV, "Dipole1", 
	(-PosD1*sin(NpolAng)), -5.08*cm, 
	(PosD1*cos(NpolAng)), 0*deg, NpolAng, 180*deg);
 }

