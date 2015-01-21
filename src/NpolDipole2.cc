// %% NpolDipole2.cc %%

// Dipole 2 'BNL 48D48' Constructor file.  
// Created: William Tireman - January 2015


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
#include "NpolDipole2.hh"

NpolDipole2::NpolDipole2() {
  G4cout << "Initializing Dipole 2" << G4endl;
}

NpolDipole2::~NpolDipole2() {
  G4cout << "Deleting Dipole 2" << G4endl;
}

// Construct the yokes using the extruded class
void NpolDipole2::ConstructDipole2Yoke() {

  std::vector<G4TwoVector> polygon(8); // define the polygon to be extruded
  polygon[0] = G4TwoVector(-1.86055*m, 0.0*m);
  polygon[1] = G4TwoVector(-1.86055*m, 1.3970*m);
  polygon[2] = G4TwoVector(1.86055*m, 1.3970*m);
  polygon[3] = G4TwoVector(1.86055*m, 0.0*m);
  polygon[4] = G4TwoVector(0.6096*m, 0.0*m);
  polygon[5] = G4TwoVector(0.6096*m, 0.23495*m);
  polygon[6] = G4TwoVector(-0.6096*m, 0.23495*m);
  polygon[7] = G4TwoVector(-0.6096*m, 0.0*m);

  G4ExtrudedSolid *Dipole2Yoke = new G4ExtrudedSolid("Dipole2Yoke", 
     polygon, 0.6096*m, G4TwoVector(0, 0), 1.0, G4TwoVector(0, 0), 1.0);
  Dipole2YokeLV = new G4LogicalVolume(Dipole2Yoke,
     NpolMaterials::GetInstance()->GetFe(),"Dipole2YokeLV",0,0,0);
  
  G4VisAttributes *Dipole2YokeVisAtt= 
    new G4VisAttributes(G4Colour(1.0,0.0,2.0));
  Dipole2YokeLV->SetVisAttributes(Dipole2YokeVisAtt);
}

void NpolDipole2::ConstructDipole2Coil(){

}


G4VPhysicalVolume *NpolDipole2::Construct(G4LogicalVolume *motherLV) {
  G4double PosD2 = 4.6866*m, NpolAng = 28.0*deg;

  ConstructDipole2Yoke();
  PlaceCylindrical(Dipole2YokeLV, motherLV, "Dipole2", 
	PosD2,-NpolAng,+0.0*cm);
  return PlaceRectangular(Dipole2YokeLV, motherLV, "Dipole2", 
        (-PosD2*sin(NpolAng)), 0.0*cm, 
  	(PosD2*cos(NpolAng)), 0*deg, NpolAng, 180*deg);
 }

