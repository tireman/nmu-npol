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

// Construct the yokes
void NpolDipole1::ConstructDipole1Yoke() {

  std::vector<G4TwoVector> polygon(12);
  polygon[0] = G4TwoVector(-1.17475*m, 0.0*m);
  polygon[1] = G4TwoVector(-1.17475*m, 0.3556*m);
  polygon[2] = G4TwoVector(1.17475*m, 0.3556*m);
  polygon[3] = G4TwoVector(1.17475*m, 0.0*m);
  polygon[4] = G4TwoVector(0.87*m, 0.0*m);
  polygon[5] = G4TwoVector(0.87*m, 0.15*m);
  polygon[6] = G4TwoVector(0.47*m, 0.15*m);
  polygon[7] = G4TwoVector(0.43*m, 0.04*m);
  polygon[8] = G4TwoVector(-0.43*m,0.04*m);
  polygon[9] = G4TwoVector(-0.47*m, 0.15*m);
  polygon[10] = G4TwoVector(-0.87*m, 0.15*m);
  polygon[11] = G4TwoVector(-0.87*m, 0.0*m);

  G4ExtrudedSolid *Dipole1Yoke = new G4ExtrudedSolid("Dipole1Yoke", polygon, 0.6096*m, G4TwoVector(0, 0), 1.0, G4TwoVector(0, 0), 1.0);
  Dipole1YokeLV = new G4LogicalVolume(Dipole1Yoke,
     NpolMaterials::GetInstance()->GetFe(),"Dipole1YokeLV",0,0,0);
  
  G4VisAttributes *Dipole1YokeVisAtt= 
    new G4VisAttributes(G4Colour(1.0,0.0,0.00));
  Dipole1YokeLV->SetVisAttributes(Dipole1YokeVisAtt);
}


G4VPhysicalVolume *NpolDipole1::Construct(G4LogicalVolume *motherLV) {
  
  ConstructDipole1Yoke();
  //ConstructTargetFluid();

  PlaceCylindrical(Dipole1YokeLV, motherLV, "Dipole1", 1.90*m,-28.0*deg,+5.08*cm);
  return PlaceRectangular(Dipole1YokeLV, motherLV, "Dipole1", (-1.90*sin(28.0*deg))*m, -5.08*cm, (1.90*cos(28.0*deg))*m, 0*deg, 28.0*deg, 180*deg);
 }

