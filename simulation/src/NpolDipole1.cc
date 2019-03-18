//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************

// %% NpolDipole1.cc %%

// Dipole 1 'Charybdis' Constructor file.  
// Created: William Tireman - December 2014

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4TwoVector.hh"
#include "G4ExtrudedSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4PVPlacement.hh"
#include "G4UniformMagField.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4ChordFinder.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4ClassicalRK4.hh"

#include "NpolMaterials.hh"
#include "NpolDipole1.hh"
#include "NpolBeamlineDown.hh"
#include "NpolDetectorConstruction.hh"

// Field multiplier; if 2 mags, use value corresponding to desired integrated
// field strength; example: for 1 Tm use FM = 1.0 and for 4.3 Tm use FM = 4.3
// If 1 mag, use 2.0 for integrated 1.0 Tm or 4.0 for 2.0 Tm (double FM for 
// same effect) Note: In general, for 1 mag test use only Dipole 1
G4double NpolDipole1::FM = 4.3; 
G4double NpolDipole1::dipole1FieldY = FM*0.40984*tesla; 

G4double NpolDipole1::NpolAng = 28.0*deg;
G4double NpolDipole1::PosD1 = 2.5096*m; // + 0.15*m; // add in 15 cm of space with particle tagger to fit lead curtain
G4double NpolDipole1::yokeLength = 48.0*2.54*cm; // inches to cm
G4double NpolDipole1::gapWidth = 22.0*2.54*cm; // inches to cm
G4double NpolDipole1::gapLength = 48.0*2.54*cm; // inches to cm
G4double NpolDipole1::gapHeight = 8.25*2.54*cm; // using 8.25 inch or 10.25 inch or 14.75 inch 
G4double NpolDipole1::yokeGap = 2.25*2.54*cm;  // adjust if at 10.25in gap (4.00 inch)
G4double NpolDipole1::fieldClampHeight = 60.0625*2.54*cm;  
G4double NpolDipole1::fieldClampWidth = 92.625*2.54*cm;
G4double NpolDipole1::fieldClampThick = 2.0*2.54*cm ;
G4double NpolDipole1::fieldClampInheight = gapHeight + 2.0*2.54*cm;
G4double NpolDipole1::fieldClampInwidth = gapWidth + 2.0*2.54*cm;

NpolDipole1::NpolDipole1() {
  ConstructDipole1Yoke();
  ConstructDipole1CuBar();
  ConstructDipole1CuEnd();
  ConstructDipole1FieldClamp1();
  ConstructDipole1FieldClamp2();
  ConstructDipole1Field();
}

NpolDipole1::~NpolDipole1() {}

G4String NpolDipole1::GetName() {
  return G4String("Dipole 1");
}

// Construct the yokes using the extruded class
void NpolDipole1::ConstructDipole1Yoke() {
  G4double inch = 2.54*cm;
  // define the polygon to be extruded
  std::vector<G4TwoVector> polygon(12); 
  polygon[0] = G4TwoVector((92.5/2)*inch, 0.0*m);
  polygon[1] = G4TwoVector((92.5/2)*inch, 28.0*inch);
  polygon[2] = G4TwoVector(-(92.5/2)*inch, 28.0*inch);
  polygon[3] = G4TwoVector(-(92.5/2)*inch, 0.0*m);
  polygon[4] = G4TwoVector(-26.0*inch, 0.0*m);
  polygon[5] = G4TwoVector(-26.0*inch, 7.0*inch);
  polygon[6] = G4TwoVector(-gapWidth/2, 7.0*inch);
  polygon[7] = G4TwoVector(-gapWidth/2, 3.0*inch);
  polygon[8] = G4TwoVector(+gapWidth/2, 3.0*inch);
  polygon[9] = G4TwoVector(+gapWidth/2, 7.0*inch);
  polygon[10] = G4TwoVector(26.0*inch, 7.0*inch);
  polygon[11] = G4TwoVector(26.0*inch, 0.0*m);
  
  G4ExtrudedSolid *Dipole1Yoke = 
	new G4ExtrudedSolid("Dipole1Yoke",polygon, yokeLength/2, G4TwoVector(0, 0), 1.0, G4TwoVector(0, 0), 1.0);
  Dipole1YokeLV = new G4LogicalVolume(Dipole1Yoke,
       NpolMaterials::GetInstance()->GetMaterial("Fe"),"Dipole1YokeLV",0,0,0);

  G4VisAttributes *Dipole1YokeVisAtt= 
    new G4VisAttributes(G4Colour(0.0,0.0,1.0));
  Dipole1YokeLV->SetVisAttributes(Dipole1YokeVisAtt);
}

void NpolDipole1::ConstructDipole1CuBar(){
  
  // Define the horizontal pieces of the Dipole 1 coil packs which
  // are made of Cu. Wish I could imagine a way to do this as an extruded
  // solid ... hmmm.  The Dipole 1 will need to place 4 of these.
  G4double inch = 2.54*cm;
  G4Box *Dipole1CuBar = new G4Box("Dipole1CuBar", 15.0/2*inch, 7.0/2*inch, 52.0/2*inch);
  Dipole1CuBarLV = new G4LogicalVolume(Dipole1CuBar, 
         NpolMaterials::GetInstance()->GetMaterial("Cu"),"Dipole1CuBarLV", 0,0,0);
  G4VisAttributes *CuBar = new G4VisAttributes(G4Colour(0.0,0.50,0.51));
  Dipole1CuBarLV->SetVisAttributes(CuBar);
}

void NpolDipole1::ConstructDipole1CuEnd(){
  // Define the end pieces of the Dipole 1 coil packs which
  // are made of Cu. Wish I could imagine a way to do this differently.
  // The Dipole 1 will need to place 4 of these.
  G4double inch = 2.54*cm;
  std::vector<G4TwoVector> polygon(8); // define the polygon to be extruded
  polygon[0] = G4TwoVector(-26.0*inch, 0.0*m);
  polygon[1] = G4TwoVector(-26.0*inch, 27.5*inch);
  polygon[2] = G4TwoVector(+26.0*inch, 27.5*inch);
  polygon[3] = G4TwoVector(+26.0*inch, 0.0*m);
  polygon[4] = G4TwoVector(+11.0*inch, 0.0*m);
  polygon[5] = G4TwoVector(+11.0*inch, 12.5*inch);
  polygon[6] = G4TwoVector(-11.0*inch, 12.5*inch);
  polygon[7] = G4TwoVector(-11.0*inch, 0.0*m);
  
  G4ExtrudedSolid *Dipole1CuEnd = new G4ExtrudedSolid("Dipole1CuEnd", 
       polygon, 3.0*inch, G4TwoVector(0, 0), 1.0, G4TwoVector(0, 0), 1.0);
  Dipole1CuEndLV = new G4LogicalVolume(Dipole1CuEnd,
       NpolMaterials::GetInstance()->GetMaterial("Cu"),"Dipole1CuEndLV",0,0,0);

  G4VisAttributes *Dipole1CuEndVisAtt= 
    new G4VisAttributes(G4Colour(0.0,0.50,0.51));
  Dipole1CuEndLV->SetVisAttributes(Dipole1CuEndVisAtt);
}

void NpolDipole1::ConstructDipole1FieldClamp1(){
  // Constructor for the Charybdis field clamp in front of the magnet
  // Needs a "hole" on the left side to get the beam pipe through
  
   G4double radius = 1.0*cm + NpolBeamlineDown::SecA2OutRadius, CyLen = 10.0*cm;
  
  // Create the necessary solids
  G4Box *Slab = new G4Box("Slab", fieldClampWidth/2, fieldClampHeight/2, fieldClampThick/2);
  G4Box *Hole = new G4Box("Hole", (fieldClampInwidth+0.10)/2, (fieldClampInheight+0.10)/2, (fieldClampThick+0.01*m)/2);
  G4Tubs *BHole = 
	new G4Tubs("BHole", 0.0*m, radius+0.50*cm, ((CyLen/2)+radius*tan(NpolAng)+10.0*cm), 0.0*deg, 360.*deg);
  
  // Rotation and translation information for the hole for the beam line
  G4RotationMatrix *yRot = new G4RotationMatrix;
  yRot->rotateY(-NpolAng);
  G4ThreeVector xTrans(+0.85*m, 0.0*m, 0.0*m);
  
  
  // Create the subtraction solids
  G4SubtractionSolid *PlateHole = new G4SubtractionSolid("PlateHole", Slab,BHole, yRot, xTrans);
  G4SubtractionSolid *FieldClamp1 = new G4SubtractionSolid("FieldClamp1", PlateHole, Hole); 

  FieldClamp1LV = 
	new G4LogicalVolume(FieldClamp1, NpolMaterials::GetInstance()->GetMaterial("Fe"),"FieldClampLV1", 0,0,0);
  G4VisAttributes *Clamp = new G4VisAttributes(G4Colour(0.5,0.5,0.0));
  FieldClamp1LV->SetVisAttributes(Clamp);
}

void NpolDipole1::ConstructDipole1FieldClamp2(){
  // Constructor for the Charybdis field clamp in back of the magnet
  
  G4Box *Slab = new G4Box("Slab", fieldClampWidth/2, fieldClampHeight/2, fieldClampThick/2);
  G4Box *Hole = new G4Box("Hole", (fieldClampInwidth+0.10)/2, (fieldClampInheight+0.10)/2, (fieldClampThick+0.01*m)/2);

  G4SubtractionSolid *FieldClamp2 = new G4SubtractionSolid("FieldClamp2", Slab, Hole); 
  
  FieldClamp2LV = 
	new G4LogicalVolume(FieldClamp2, NpolMaterials::GetInstance()->GetMaterial("Fe"),"FieldClamp2LV", 0,0,0);
  G4VisAttributes *Clamp = new G4VisAttributes(G4Colour(0.5,0.5,0.0));
  FieldClamp2LV->SetVisAttributes(Clamp);

}

void NpolDipole1::ConstructDipole1Field(){
  // Generate the magnetic field volume  (14-April-2017) subtracted 1.6*cm off height to stop overlap
  G4Box *Dipole1Field = new G4Box("Dipole1Field",gapWidth/2, gapHeight/2,/*-1.6*cm*/ gapLength/2);
  Dipole1FieldLV = 
	new G4LogicalVolume(Dipole1Field, NpolMaterials::GetInstance()->GetMaterial("Vacuum"),"Dipole1FieldLV", 0,0,0);
  //G4VisAttributes *Field = new G4VisAttributes(G4Colour(0.0,1.0,0.0));
  //Dipole1FieldLV->SetVisAttributes(Field);
  Dipole1FieldLV->SetVisAttributes(G4VisAttributes::GetInvisible());

}

void NpolDipole1::Place(G4LogicalVolume *motherLV) {

  G4double inch = 2.54*cm;
  G4double BarOffSet = 18.5*inch;
  G4double BarVertOffset = yokeGap/2 + 3.5*inch;
  G4double GapOffset = yokeGap/2;
  G4double EndOffSet = +29.0*inch, ClampOffSet = 37.0*inch;
   
  // Place 4 of the Copper bars in the magnet
  PlaceRectangular(Dipole1CuBarLV, motherLV, "Dipole1CuBar", (BarOffSet*cos(NpolAng)-PosD1*sin(NpolAng)), 
				   +BarVertOffset, (BarOffSet*sin(NpolAng)+PosD1*cos(NpolAng)), 0*deg, -NpolAng, 0.0); 
  PlaceRectangular(Dipole1CuBarLV, motherLV, "Dipole1CuBar", (BarOffSet*cos(NpolAng)-PosD1*sin(NpolAng)), 
				   -BarVertOffset, (BarOffSet*sin(NpolAng)+PosD1*cos(NpolAng)), 0*deg, -NpolAng, 0.0); 
  PlaceRectangular(Dipole1CuBarLV, motherLV, "Dipole1CuBar", (-BarOffSet*cos(NpolAng)-PosD1*sin(NpolAng)), 
				   +BarVertOffset, (-BarOffSet*sin(NpolAng)+PosD1*cos(NpolAng)), 0*deg, -NpolAng, 0.0); 
  PlaceRectangular(Dipole1CuBarLV, motherLV, "Dipole1CuBar", (-BarOffSet*cos(NpolAng)-PosD1*sin(NpolAng)), 
				   -BarVertOffset, (-BarOffSet*sin(NpolAng)+PosD1*cos(NpolAng)), 0*deg, -NpolAng, 0.0); 
  
  // Place 4 copies of the extruded copper ends for the coil packs
  PlaceCylindrical(Dipole1CuEndLV, motherLV, "Dipole1CuEnd", (PosD1-EndOffSet), -NpolAng, +GapOffset);
  PlaceCylindrical(Dipole1CuEndLV, motherLV, "Dipole1CuEnd", (PosD1+EndOffSet), -NpolAng, +GapOffset);
  PlaceRectangular(Dipole1CuEndLV, motherLV, "Dipole1CuEnd", (-(PosD1-EndOffSet)*sin(NpolAng)), -GapOffset, ((PosD1-EndOffSet)*cos(NpolAng)), 0.0*deg, NpolAng, 180.*deg);
  PlaceRectangular(Dipole1CuEndLV, motherLV, "Dipole1CuEnd", (-(PosD1+EndOffSet)*sin(NpolAng)), -GapOffset, ((PosD1+EndOffSet)*cos(NpolAng)), 0.0*deg, NpolAng, 180.*deg);
  
  // Place 2 copies of the Charybdis field clamps
  PlaceCylindrical(FieldClamp1LV, motherLV, "FieldClamp1", (PosD1-ClampOffSet), -NpolAng, 0.0*m);
  PlaceCylindrical(FieldClamp2LV, motherLV, "FieldClamp2", (PosD1+ClampOffSet), -NpolAng, 0.0*m);
  
  // Place upper yoke piece
  PlaceCylindrical(Dipole1YokeLV, motherLV, "Dipole1", PosD1,-NpolAng,+yokeGap/2); 
  // Place lower yoke piece 
  PlaceRectangular(Dipole1YokeLV, motherLV, "Dipole1", (-PosD1*sin(NpolAng)), -yokeGap/2,(PosD1*cos(NpolAng)),0*deg, NpolAng, 180*deg); 
  
  // Place the magnetic field volume
  PlaceCylindrical(Dipole1FieldLV, motherLV, "Field1", PosD1,-NpolAng,0.0*cm);
   
  // End Dipole 1 (Charybdis) construction.  May the field be with it. 
}

void NpolDipole1::ConstructSDandField() {
  // Generate the Magnetic Field for Charybdis
  G4TransportationManager* tmanMagField = G4TransportationManager::GetTransportationManager();
  tmanMagField -> GetPropagatorInField() -> SetLargestAcceptableStep(1*mm);
  
  G4UniformMagField *magField = new G4UniformMagField(G4ThreeVector(0., dipole1FieldY, 0.));
  G4Mag_EqRhs *fEqMagField = new G4Mag_UsualEqRhs(magField);
  G4double minStepMagneticField = 0.0025*mm;
  G4FieldManager *fieldManMagField = new G4FieldManager(magField);
  
  G4MagIntegratorStepper *stepperMagField = new G4ClassicalRK4(fEqMagField);
  fieldManMagField -> SetDetectorField(magField);
  
  G4ChordFinder *fChordFinder = new G4ChordFinder(magField, minStepMagneticField, stepperMagField);
  fieldManMagField->SetChordFinder(fChordFinder);
  
  Dipole1FieldLV->SetFieldManager(fieldManMagField, true);
}

