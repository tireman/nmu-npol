//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************

// %% NpolPolarimeter.cc %%

// Polarimeter construction file
// Modified: Daniel Wilbern - December 2014

#include <string>
#include <cstring>
#include <vector>

#include "G4PhysicalConstants.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4AssemblyVolume.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4ios.hh"

#include "NpolMaterials.hh"
#include "NpolPolarimeter.hh"
#include "NpolAnalysisManager.hh"
#include "NpolDetectorFactory.hh"
#include "NpolDetectorConstruction.hh"

G4double NpolPolarimeter::NpolBoxPos = 8.7*m-2.0*m; //Subtract 2*m if 5m test
G4double NpolPolarimeter::NDetThickness = 10.0*cm;
G4double NpolPolarimeter::NVetoThickness = 1.00*cm;
G4double NpolPolarimeter::NDetHeight = 10.0*cm;
G4double NpolPolarimeter::NDetStandardLength = 100.0*cm;
G4double NpolPolarimeter::EArrayVertOffset = 0.90*m;
G4double NpolPolarimeter::dEArrayVertOffset = 0.42*m; // orig was 0.32*m
G4double NpolPolarimeter::EArrayHoriOffset = 0.602*m; // computed offset 0.601*m
G4double NpolPolarimeter::dEArrayHoriOffset = 0.4288*m; // offset is 0.569*m for angle dEs
G4double NpolPolarimeter::Array1zPos = -1.10*m;
G4double NpolPolarimeter::Array2zPos = +0.300*m;
G4double NpolPolarimeter::EarrayRotAngle = 45.0*deg;
G4double NpolPolarimeter::FrontDetSpacing = 0.65*m;
G4double NpolPolarimeter::FrontDetOffset = 1.70*m;
G4double NpolPolarimeter::FrontVetoOffset = 1.77*m;
G4double NpolPolarimeter::BackTaggerzPos = 1.80*m;
G4double NpolPolarimeter::NpolAng = 28.0*deg; // 28.0*deg; nominal

NpolPolarimeter::NpolPolarimeter() {
  G4VSolid *PolarimeterBox = new G4Box("PolarimeterBox",1.30*m,1.45*m,1.85*m);
  PolarimeterLV = new G4LogicalVolume(PolarimeterBox,
      NpolMaterials::GetInstance()->GetMaterial("HardVacuum"), "PolarimeterLV",0,0,0);
  PolarimeterLV->SetVisAttributes(G4VisAttributes::GetInvisible());
  
  ConstructTopDetArray(PolarimeterLV);
  ConstructTopVetoArray(PolarimeterLV);
  ConstructBottomDetArray(PolarimeterLV);
  ConstructBottomVetoArray(PolarimeterLV);
  ConstructFrontDetArray(PolarimeterLV);
  ConstructFrontVetoArray(PolarimeterLV);
  ConstructBackTagArray(PolarimeterLV);
}

NpolPolarimeter::~NpolPolarimeter() {}

G4String NpolPolarimeter::GetName() {
  return G4String("Polarimeter");
}

G4AssemblyVolume *NpolPolarimeter::MakePlate(G4LogicalVolume *detLV,	
	 G4int numDets, G4double TmX, G4double TmY, G4double TmZ,
	 G4double TmdX, G4double TmdY, G4double TmdZ) {
  
  G4AssemblyVolume *plate = new G4AssemblyVolume();
  
  // Translation and rotation of plate inside assembly
  G4RotationMatrix Ra; 
  G4ThreeVector Ta, Tm;
  G4Transform3D Tr;
  
  int i;
  for(i=0; i<numDets; i++) {
    Tm.setX(TmX-TmdX*i); Tm.setY(TmY-TmdY*i); Tm.setZ(TmZ-TmdZ*i);
    Tr = G4Transform3D(Ra,Tm);
    plate->AddPlacedVolume(detLV,Tr);
  }
  
  return plate;
}

void NpolPolarimeter::ImprintPlate(G4AssemblyVolume *plate, 
	G4LogicalVolume *motherLV, G4double TmX, G4double TmY, 
	G4double TmZ, G4double RmZ) {
  
  G4ThreeVector Tm;
  G4RotationMatrix Rm;
  G4Transform3D Tr;
  
  Tm.setX(TmX); Tm.setY(TmY); Tm.setZ(TmZ);
  Rm.rotateX(0.0*deg);
  Rm.rotateY(0.0*deg);
  Rm.rotateZ(RmZ);
  Tr = G4Transform3D(Rm,Tm);
  
  plate->MakeImprint(motherLV,Tr);
}

//---------------------------
// Top E detectors
// Andrei's 2012 Plan B setup using 160 cm long detectors 10cm by 10cm
//---------------------------
void NpolPolarimeter::ConstructTopDetArray(G4LogicalVolume *motherLV) {
  
  G4double VertPos1 = EArrayVertOffset; 
  G4double VertPos2 = EArrayVertOffset + 0.10*m;
 
  G4VSolid *TopDet = new G4Box("TopDet",(NDetStandardLength + 60.0*cm)/2,NDetHeight/2,NDetThickness/2);
  G4LogicalVolume *TopDetLV = new G4LogicalVolume(TopDet,
	  NpolMaterials::GetInstance()->GetMaterial("Scint"),"TopDetLV",0,0,0);

  G4AssemblyVolume *TopDetArray1 = MakePlate(TopDetLV, 13, 0.0*m, 0.0*m, 0.60*m, 0.0*m, 0.0*m, 0.10*m);
  G4AssemblyVolume *TopDetArray2 = MakePlate(TopDetLV, 14, 0.0*m, 0.0*m, 0.60*m, 0.0*m, 0.0*m, 0.10*m);
  
  ImprintPlate(TopDetArray1, motherLV, EArrayHoriOffset, VertPos1, Array1zPos, -EarrayRotAngle);
  ImprintPlate(TopDetArray1, motherLV, -EArrayHoriOffset, VertPos1, Array1zPos, EarrayRotAngle);
  ImprintPlate(TopDetArray2, motherLV, EArrayHoriOffset, VertPos2, Array2zPos, -EarrayRotAngle);
  ImprintPlate(TopDetArray2, motherLV, -EArrayHoriOffset, VertPos2, Array2zPos, EarrayRotAngle);
  
  G4VisAttributes *TopVisAtt= new G4VisAttributes(G4Colour(1.0,0.5,0.0));
  TopDetLV->SetVisAttributes(TopVisAtt);
}

//----------------------------
// Top dE detectors
// Andrei's 2012 Plan B 160 cm long "taggers" in a horizontal orientation
//----------------------------
void NpolPolarimeter::ConstructTopVetoArray(G4LogicalVolume *motherLV) {
  G4double VertPos1 = dEArrayVertOffset; 
  G4double VertPos2 = dEArrayVertOffset + 0.10*m;
  G4VSolid *TopVeto = new G4Box("TopVeto",(NDetStandardLength + 60.0*cm)/2,NVetoThickness/2,NDetHeight/2);
  G4LogicalVolume *TopVetoLV = new G4LogicalVolume(TopVeto,
	NpolMaterials::GetInstance()->GetMaterial("Scint"), "TopVetoLV",0,0,0);
  
  G4AssemblyVolume *TopVetoArray1 = MakePlate(TopVetoLV, 13, 0.0*m, 0.0*m, 0.60*m, 0.0*m, 0.0*m, 0.10*m);
  G4AssemblyVolume *TopVetoArray2 = MakePlate(TopVetoLV, 14, 0.0*m, 0.0*m, 0.60*m, 0.0*m, 0.0*m, 0.10*m);
  
  ImprintPlate(TopVetoArray1, motherLV, 0.0*m, VertPos1, Array1zPos, 0.0*deg); // originals!
  ImprintPlate(TopVetoArray2, motherLV, 0.0*m, VertPos2, Array2zPos, 0.0*deg); 

  // for the new "angled" version 
  /*ImprintPlate(TopVetoArray1, motherLV, dEArrayHoriOffset, VertPos1, Array1zPos, -EarrayRotAngle);
  ImprintPlate(TopVetoArray1, motherLV, -dEArrayHoriOffset, VertPos1, Array1zPos, EarrayRotAngle);
  ImprintPlate(TopVetoArray2, motherLV, dEArrayHoriOffset, VertPos2, Array2zPos, -EarrayRotAngle);
  ImprintPlate(TopVetoArray2, motherLV, -dEArrayHoriOffset, VertPos2, Array2zPos, EarrayRotAngle);*/
  
  G4VisAttributes* TopaVisAtt= new G4VisAttributes(G4Colour(0.0,1.0,1.0));
  TopVetoLV->SetVisAttributes(TopaVisAtt);
}


//------------------------------
// Bottom E detectors
// Andrei's 2012 Plan B setup using 160 cm long 10cm by 10cm detectors
//------------------------------
void NpolPolarimeter::ConstructBottomDetArray(G4LogicalVolume *motherLV) {
  G4double VertPos1 = -1*EArrayVertOffset; 
  G4double VertPos2 = -1*(EArrayVertOffset + 0.10*m);
  G4VSolid *BottomDet = new G4Box("BottomDet",(NDetStandardLength + 60.0*cm)/2,NDetHeight/2,NDetThickness/2);
  G4LogicalVolume *BottomDetLV = new G4LogicalVolume(BottomDet,
	NpolMaterials::GetInstance()->GetMaterial("Scint"), "BottomDetLV",0,0,0);
  
  G4AssemblyVolume *BottomDetArray1 = MakePlate(BottomDetLV,
	13, 0.0*m, 0.0*m, 0.60*m, 0.0*m, 0.0*m, 0.10*m);
  G4AssemblyVolume *BottomDetArray2 = MakePlate(BottomDetLV,
	14, 0.0*m, 0.0*m, 0.60*m, 0.0*m, 0.0*m, 0.10*m);

  ImprintPlate(BottomDetArray1, motherLV, EArrayHoriOffset, VertPos1, Array1zPos, EarrayRotAngle);
  ImprintPlate(BottomDetArray1, motherLV, -EArrayHoriOffset, VertPos1, Array1zPos, EarrayRotAngle + 90*deg);
  ImprintPlate(BottomDetArray2, motherLV, EArrayHoriOffset, VertPos2, Array2zPos, EarrayRotAngle);
  ImprintPlate(BottomDetArray2, motherLV, -EArrayHoriOffset, VertPos2, Array2zPos, EarrayRotAngle + 90*deg);
  
  G4VisAttributes *BotVisAtt= new G4VisAttributes(G4Colour(1.0,0.50,0.0));
  BottomDetLV->SetVisAttributes(BotVisAtt);
}

//-----------------------------
// BOTTOMa dE detectors
// Andrei's 2012 Plan B setup using 160 cm long "tagger" detectors 1cm thick
//-----------------------------
void NpolPolarimeter::ConstructBottomVetoArray(G4LogicalVolume *motherLV) {
  G4double VertPos1 = -1*dEArrayVertOffset; 
  G4double VertPos2 = -1*(dEArrayVertOffset + 0.10*m);
  G4VSolid *BottomVeto = new G4Box("BottomVeto",(NDetStandardLength + 60.0*cm)/2,NVetoThickness/2,NDetHeight/2);
  G4LogicalVolume *BottomVetoLV = new G4LogicalVolume(BottomVeto,
	NpolMaterials::GetInstance()->GetMaterial("Scint"), "BottomVetoLV",0,0,0);

  G4AssemblyVolume *BottomVetoArray1 = MakePlate(BottomVetoLV,
	 13, 0.0*m, 0.0*m, 0.60*m, 0.0*m, 0.0*m, 0.10*m);
  G4AssemblyVolume *BottomVetoArray2 = MakePlate(BottomVetoLV,
	14, 0.0*m, 0.0*m, 0.60*m, 0.0*m, 0.0*m, 0.10*m);

  ImprintPlate(BottomVetoArray1, motherLV, 0.0*m, VertPos1, Array1zPos, 0.0*deg); // originals!
  ImprintPlate(BottomVetoArray2, motherLV, 0.0*m, VertPos2, Array2zPos, 0.0*deg); 
  
  // For new Angled version
  /*ImprintPlate(BottomVetoArray1, motherLV, dEArrayHoriOffset, VertPos1, Array1zPos, EarrayRotAngle);
  ImprintPlate(BottomVetoArray1, motherLV, -dEArrayHoriOffset, VertPos1, Array1zPos, EarrayRotAngle + 90*deg);
  ImprintPlate(BottomVetoArray2, motherLV, dEArrayHoriOffset, VertPos2, Array2zPos, EarrayRotAngle);
  ImprintPlate(BottomVetoArray2, motherLV, -dEArrayHoriOffset, VertPos2, Array2zPos, EarrayRotAngle + 90*deg);*/

  G4VisAttributes* BotaVisAtt= new G4VisAttributes(G4Colour(0.0,1.0,1.0));
  BottomVetoLV->SetVisAttributes(BotaVisAtt);
}

void NpolPolarimeter::ConstructFrontDetArray(G4LogicalVolume *motherLV) {
  
  G4ThreeVector Tm;
  G4Transform3D Tr;
  
  G4VSolid *FrontDet = new G4Box("FrontDet",NDetStandardLength/2,NDetHeight/2,NDetThickness/2);
  G4LogicalVolume *FrontDetLV = new G4LogicalVolume(FrontDet,
	NpolMaterials::GetInstance()->GetMaterial("Scint"), "FrontDetLV",0,0,0);
  
  G4AssemblyVolume *FrontDetArray1 = MakePlate(FrontDetLV, 6, 0.0*m, 0.25*m, 0.0*m, 0.0*m, 0.10*m, 0.0*m);
  G4AssemblyVolume *FrontDetArray2 = MakePlate(FrontDetLV, 8, 0.0*m, 0.35*m, 0.0*m, 0.0*m, 0.10*m, 0.0*m);
  
  // Rotation of assembly inside the world
  G4RotationMatrix Rm; 
  Rm.rotateX(0.0*deg);
  Rm.rotateY(0.0*deg);
  
  for(unsigned int i=0; i<2; i++)
    ImprintPlate(FrontDetArray1, motherLV, 0.0*m, 0.0*m, (-FrontDetOffset+FrontDetSpacing*i), 0.0*deg);
  for(unsigned int i=2; i<4; i++)
    ImprintPlate(FrontDetArray2, motherLV, 0.0*m, 0.0*m, (-FrontDetOffset+FrontDetSpacing*i), 0.0*deg);
  
  G4VisAttributes* FrontDetVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,0.0));
  FrontDetLV->SetVisAttributes(FrontDetVisAtt);
}

//---------------------------------------------------------------------
// Taggers for analyzer layers
// 1.0 cm by 10.0 cm by 100.0 cm
//----------------------------------------------------------------------
void NpolPolarimeter::ConstructFrontVetoArray(G4LogicalVolume *motherLV) {
  
  
  G4ThreeVector Tm;
  G4Transform3D Tr; 
  
  G4VSolid *FrontVeto = new G4Box("FrontVeto",NDetStandardLength/2,NDetHeight/2,NVetoThickness/2);
  G4LogicalVolume *FrontVetoLV = new G4LogicalVolume(FrontVeto,
	NpolMaterials::GetInstance()->GetMaterial("Scint"), "FrontVetoLV",0,0,0);
  
  G4AssemblyVolume *FrontVetoArray1 = MakePlate(FrontVetoLV,
	  6, 0.0*m, 0.25*m, 0.0*m, 0.0*m, 0.10*m, 0.0*m);
  G4AssemblyVolume *FrontVetoArray2 = MakePlate(FrontVetoLV,
	  8, 0.0*m, 0.35*m, 0.0*m, 0.0*m, 0.10*m, 0.0*m);

  for(unsigned int i=0; i<2; i++)
    ImprintPlate(FrontVetoArray1, motherLV, 0.0*m, 0.0*m, 
		 (-FrontVetoOffset+FrontDetSpacing*i), 0.0*deg); // 1.77 for 10cm thick Front
  for(unsigned int i=2; i<4; i++)
    ImprintPlate(FrontVetoArray2, motherLV, 0.0*m, 0.0*m, 
		 (-FrontVetoOffset+FrontDetSpacing*i), 0.0*deg);
  
  G4VisAttributes* FrontVetoVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  FrontVetoLV->SetVisAttributes(FrontVetoVisAtt);
}

void NpolPolarimeter::ConstructBackTagArray(G4LogicalVolume *motherLV) {
  
  
  G4ThreeVector Tm;
  G4Transform3D Tr; 
  
  G4VSolid *BackTag = new G4Box("BackTag",(NDetStandardLength+60.0*cm)/2,NDetHeight/2,NVetoThickness/2);
  G4LogicalVolume *BackTagLV = new G4LogicalVolume(BackTag,
	NpolMaterials::GetInstance()->GetMaterial("Scint"), "BackTagLV",0,0,0);
  
  G4AssemblyVolume *BackTaggerArray = MakePlate(BackTagLV,
	  16, 0.0*m, 0.75*m, 0.0*m, 0.0*m, 0.10*m, 0.0*m);

  ImprintPlate(BackTaggerArray, motherLV, 0.0*m, 0.0*m,BackTaggerzPos, 0.0*deg);
  ImprintPlate(BackTaggerArray, motherLV, 0.0*m, 0.0*m,(BackTaggerzPos + 0.02*m), 90.0*deg);

  G4VisAttributes* BackTagVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  BackTagLV->SetVisAttributes(BackTagVisAtt);
}


void NpolPolarimeter::Place(G4LogicalVolume *motherLV) {

  PlaceCylindrical(PolarimeterLV,motherLV,"Polarimeter", NpolBoxPos, -NpolAng,0.0*m);
}

