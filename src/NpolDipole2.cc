//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************

// %% NpolDipole2.cc %%

// Dipole 2 'BNL 48D48' Constructor file.  
// Created: William Tireman - January 2015

#include "G4PhysicalConstants.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4TwoVector.hh"
#include "G4ExtrudedSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4ios.hh"
#include "G4UniformMagField.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4ChordFinder.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4ClassicalRK4.hh"

#include "NpolMaterials.hh"
#include "NpolDipole2.hh"

G4double NpolDipole2::yokeLength = 0.6096*m;
G4double NpolDipole2::gapWidth = 1.22*m;
G4double NpolDipole2::gapLength = 1.22*m;
G4double NpolDipole2::gapHeight = 0.2096*m;

NpolDipole2::NpolDipole2() {
  ConstructDipole2Yoke();
  ConstructDipole2CuBar();
  ConstructDipole2CuEnd();
  ConstructDipole2FieldClamp();
  ConstructDipole2Field();
}

NpolDipole2::~NpolDipole2() {}

G4String NpolDipole2::GetName() {
  return G4String("Dipole 2");
}

// Construct the yokes using the extruded class
void NpolDipole2::ConstructDipole2Yoke() {
  
  std::vector<G4TwoVector> polygon(8); // define the polygon to be extruded
  polygon[0] = G4TwoVector(-1.8669*m, 0.0*m);
  polygon[1] = G4TwoVector(-1.8669*m, 1.3970*m);
  polygon[2] = G4TwoVector(+1.8669*m, 1.3970*m);
  polygon[3] = G4TwoVector(+1.8669*m, 0.0*m);
  polygon[4] = G4TwoVector(+0.9335*m, 0.0*m);
  polygon[5] = G4TwoVector(+0.9335*m, 0.23495*m);
  polygon[6] = G4TwoVector(-0.9335*m, 0.23495*m);
  polygon[7] = G4TwoVector(-0.9335*m, 0.0*m);
  
  G4ExtrudedSolid *Dipole2Yoke = new G4ExtrudedSolid("Dipole2Yoke", 
						     polygon, yokeLength, G4TwoVector(0, 0), 1.0, G4TwoVector(0, 0), 1.0);
  Dipole2YokeLV = new G4LogicalVolume(Dipole2Yoke,
				      NpolMaterials::GetInstance()->GetFe(),"Dipole2YokeLV",0,0,0);
  
  G4VisAttributes *Dipole2YokeVisAtt= 
    new G4VisAttributes(G4Colour(1.0,0.0,2.0));
  Dipole2YokeLV->SetVisAttributes(Dipole2YokeVisAtt);
}

void NpolDipole2::ConstructDipole2CuBar(){
  
  // Define the horizontal pieces of the Dipole 2 coil packs which
  // are made of Cu. Wish I could imagine a way to do this as an extruded
  // solid ... hmmm.  The Dipole 2 will need to place 4 of these.
  G4double xlen = 0.320*m, ylen = 0.231*m, zlen = 1.416*m;
  
  G4Box *Dipole2CuBar = new G4Box("Dipole2CuBar", xlen/2, ylen/2, zlen/2);
  Dipole2CuBarLV = new G4LogicalVolume(Dipole2CuBar, 
				       NpolMaterials::GetInstance()->GetCu(),"Dipole2CuBarLV", 0,0,0);
  G4VisAttributes *CuBar = new G4VisAttributes(G4Colour(0.0,0.50,0.51));
  Dipole2CuBarLV->SetVisAttributes(CuBar);
}

void NpolDipole2::ConstructDipole2CuEnd(){
  // Define the end pieces of the Dipole 2 coil packs which
  // are made of Cu. Wish I could imagine a way to do this differently.
  // The Dipole 2 will need to place 4 of these.
  
  std::vector<G4TwoVector> polygon(8); // define the polygon to be extruded
  polygon[0] = G4TwoVector(-0.933*m, +0.0*m);
  polygon[1] = G4TwoVector(-0.933*m, +0.81275*m);
  polygon[2] = G4TwoVector(+0.933*m, +0.81275*m);
  polygon[3] = G4TwoVector(+0.933*m, +0.0*m);
  polygon[4] = G4TwoVector(+0.613*m, +0.0*m);
  polygon[5] = G4TwoVector(+0.613*m, +0.41275*m);
  polygon[6] = G4TwoVector(-0.613*m, +0.41275*m);
  polygon[7] = G4TwoVector(-0.613*m, +0.0*m);
  
  G4ExtrudedSolid *Dipole2CuEnd = new G4ExtrudedSolid("Dipole2CuEnd", 
						      polygon, 0.231*m/2, G4TwoVector(0, 0), 1.0, G4TwoVector(0, 0), 1.0);
  Dipole2CuEndLV = new G4LogicalVolume(Dipole2CuEnd,
				       NpolMaterials::GetInstance()->GetCu(),"Dipole2CuEndLV",0,0,0);
  
  G4VisAttributes *Dipole2CuEndVisAtt= 
    new G4VisAttributes(G4Colour(0.0,0.50,0.51));
  Dipole2CuEndLV->SetVisAttributes(Dipole2CuEndVisAtt);
}

void NpolDipole2::ConstructDipole2FieldClamp(){
  // Constructor for the Charybdis field clamp in back of the magnet
  
  G4double height = 2.7940*m, width = 3.700*m, thick = 0.0508*m;
  G4double Inheight = 0.4699*m, Inwidth = 1.2192*m;
  
  G4Box *Slab = new G4Box("Slab", width/2, height/2, thick/2);
  G4Box *Hole = new G4Box("Hole", Inwidth/2, Inheight/2, (thick+0.001*m)/2);
  
  G4SubtractionSolid *FieldClamp = new G4SubtractionSolid("FieldClamp", Slab, Hole); 
  
  FieldClampLV = new G4LogicalVolume(FieldClamp, NpolMaterials::GetInstance()->GetFe(),"FieldClampLV", 0,0,0);
  G4VisAttributes *Clamp = new G4VisAttributes(G4Colour(0.5,0.5,0.0));
  FieldClampLV->SetVisAttributes(Clamp);
}


void NpolDipole2::ConstructDipole2Field(){
  // Generate the magnetic field volume
  G4Box *Dipole2Field = new G4Box("Dipole2Field",gapWidth/2, gapHeight/2, gapLength/2);
  Dipole2FieldLV = new G4LogicalVolume(Dipole2Field, NpolMaterials::GetInstance()->GetAir(),"Dipole2FieldLV", 0,0,0);
  G4VisAttributes *Field = new G4VisAttributes(G4Colour(0.5,0.7,0.2));
  Dipole2FieldLV->SetVisAttributes(Field);

  // Generate the Magnetic Field for Charybdis
  G4TransportationManager* tmanMagField = G4TransportationManager::GetTransportationManager();
  tmanMagField -> GetPropagatorInField() -> SetLargestAcceptableStep(1*mm);
  
  dipole2FieldY = 1.5*tesla;
  
  magField = new G4UniformMagField(G4ThreeVector(0., dipole2FieldY, 0.));
  fEqMagField = new G4Mag_UsualEqRhs(magField);
  minStepMagneticField = 0.0025*mm;
  G4FieldManager* fieldManMagField = new G4FieldManager(magField);
  
  stepperMagField = new G4ClassicalRK4(fEqMagField);
  fieldManMagField -> SetDetectorField(magField);
  
  fChordFinder = new G4ChordFinder(magField, minStepMagneticField, stepperMagField);
  
  Dipole2FieldLV->SetFieldManager(fieldManMagField, true);
}
void NpolDipole2::Place(G4LogicalVolume *motherLV) {
  G4double PosD2 = 4.6866*m, NpolAng = 28.0*deg, BarOffSet = +0.7730*m;
  G4double VertOffSet = 0.231/2*m, EndOffSet = +0.8235*m;
  G4double ClampOffSet = 1.039*m;
  
  // Place 4 copes of the Copper bars in the magnet
  PlaceRectangular(Dipole2CuBarLV, motherLV, "Dipole2CuBar", (BarOffSet*cos(NpolAng)-PosD2*sin(NpolAng)), VertOffSet, (BarOffSet*sin(NpolAng)+PosD2*cos(NpolAng)), 0*deg, -NpolAng, 0.0); 
  PlaceRectangular(Dipole2CuBarLV, motherLV, "Dipole2CuBar", (-BarOffSet*cos(NpolAng)-PosD2*sin(NpolAng)), VertOffSet, (-BarOffSet*sin(NpolAng)+PosD2*cos(NpolAng)), 0*deg, -NpolAng, 0.0);
  PlaceRectangular(Dipole2CuBarLV, motherLV, "Dipole2CuBar", (BarOffSet*cos(NpolAng)-PosD2*sin(NpolAng)), -VertOffSet, (BarOffSet*sin(NpolAng)+PosD2*cos(NpolAng)), 0*deg, -NpolAng, 0.0); 
  PlaceRectangular(Dipole2CuBarLV, motherLV, "Dipole2CuBar", (-BarOffSet*cos(NpolAng)-PosD2*sin(NpolAng)), -VertOffSet, (-BarOffSet*sin(NpolAng)+PosD2*cos(NpolAng)), 0*deg, -NpolAng, 0.0);
  
  // Place 4 copies of the extruded copper ends for the coil packs
  PlaceCylindrical(Dipole2CuEndLV, motherLV, "Dipole2CuEnd", (PosD2-EndOffSet), -NpolAng, 0.0*m);
  PlaceCylindrical(Dipole2CuEndLV, motherLV, "Dipole2CuEnd", (PosD2+EndOffSet), -NpolAng, 0.0*m);
  PlaceRectangular(Dipole2CuEndLV, motherLV, "Dipole2CuEnd", (-(PosD2-EndOffSet)*sin(NpolAng)), 0.0*m, ((PosD2-EndOffSet)*cos(NpolAng)), 0.0*deg, NpolAng, 180.*deg);
  PlaceRectangular(Dipole2CuEndLV, motherLV, "Dipole2CuEnd", (-(PosD2+EndOffSet)*sin(NpolAng)), 0.0*m, ((PosD2+EndOffSet)*cos(NpolAng)), 0.0*deg, NpolAng, 180.*deg);

  // Place 2 copies of the field clamps for BNL 48D48
  PlaceCylindrical(FieldClampLV, motherLV, "FieldClamp", (PosD2-ClampOffSet), -NpolAng, 0.0*m);
  PlaceCylindrical(FieldClampLV, motherLV, "FieldClamp", (PosD2+ClampOffSet), -NpolAng, 0.0*m);

  // Place the two copies of the yokes with 2nd one flipped over
  PlaceCylindrical(Dipole2YokeLV, motherLV, "Dipole2", 
		   PosD2,-NpolAng,+0.0*cm);
  PlaceRectangular(Dipole2YokeLV, motherLV, "Dipole2", 
		   (-PosD2*sin(NpolAng)), 0.0*cm, 
		   (PosD2*cos(NpolAng)), 0*deg, NpolAng, 180*deg);
  
  // Place the magnetic field volume
  PlaceCylindrical(Dipole2FieldLV, motherLV, "Field2", PosD2,-NpolAng,
		   +0.0*cm);

  // End of Dipole 2 (BNL 48D48) construction.  May it rest in place. 
}

