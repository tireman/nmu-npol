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
#include "NpolPolarimeter6Layer.hh"
#include "NpolAnalysisManager.hh"
#include "NpolDetectorFactory.hh"
#include "NpolDetectorConstruction.hh"

NpolPolarimeter6Layer::NpolPolarimeter6Layer() {
  G4VSolid *PolarimeterBox = new G4Box("PolarimeterBox",1.30*m,1.55*m,2.85*m);
  PolarimeterLV = new G4LogicalVolume(PolarimeterBox,
      NpolMaterials::GetInstance()->GetMaterial("HardVacuum"), "PolarimeterLV",0,0,0);
  PolarimeterLV->SetVisAttributes(G4VisAttributes::GetInvisible());
  
  ConstructTopDetArray(PolarimeterLV);
  ConstructTopVetoArray(PolarimeterLV);
  ConstructBottomDetArray(PolarimeterLV);
  ConstructBottomVetoArray(PolarimeterLV);
  ConstructFrontDetArray(PolarimeterLV);
  ConstructFrontTagArray(PolarimeterLV);
  ConstructBackTagArray(PolarimeterLV);
}

NpolPolarimeter6Layer::~NpolPolarimeter6Layer() {}

G4String NpolPolarimeter6Layer::GetName() {
  return G4String("Polarimeter");
}

G4AssemblyVolume *NpolPolarimeter6Layer::MakePlate(G4LogicalVolume *detLV,	
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

void NpolPolarimeter6Layer::ImprintPlate(G4AssemblyVolume *plate, 
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
void NpolPolarimeter6Layer::ConstructTopDetArray(G4LogicalVolume *motherLV) {
  
  G4VSolid *TopDet = new G4Box("TopDet",0.80*m,0.0508*m,0.0508*m);
  G4LogicalVolume *TopDetLV = new G4LogicalVolume(TopDet,
	  NpolMaterials::GetInstance()->GetMaterial("Scint"),"TopDetLV",0,0,0);

  G4AssemblyVolume *TopDetArray1 = MakePlate(TopDetLV, 13, 0.0*m, 0.0*m, 0.60*m, 0.0*m, 0.0*m, 0.10*m);
  G4AssemblyVolume *TopDetArray2 = MakePlate(TopDetLV, 14, 0.0*m, 0.0*m, 0.60*m, 0.0*m, 0.0*m, 0.10*m);
  
  ImprintPlate(TopDetArray1, motherLV, 0.615*m, 0.70*m, -2.10*m, -45.0*deg);
  ImprintPlate(TopDetArray1, motherLV, -0.615*m, 0.70*m, -2.10*m, 45.0*deg);
  ImprintPlate(TopDetArray1, motherLV, 0.615*m, 0.80*m, -0.80*m, -45.0*deg);
  ImprintPlate(TopDetArray1, motherLV, -0.615*m, 0.80*m, -0.80*m, 45.0*deg);
  ImprintPlate(TopDetArray2, motherLV, 0.615*m, 0.90*m, 0.60*m, -45.0*deg);
  ImprintPlate(TopDetArray2, motherLV, -0.615*m, 0.90*m, 0.60*m, 45.0*deg);
  
  G4VisAttributes *TopVisAtt= new G4VisAttributes(G4Colour(1.0,0.0,1.0));
  TopDetLV->SetVisAttributes(TopVisAtt);
}

//----------------------------
// Top dE detectors
// Andrei's 2012 Plan B 160 cm long "taggers" in a horizontal orientation
//----------------------------
void NpolPolarimeter6Layer::ConstructTopVetoArray(G4LogicalVolume *motherLV) {
  
  G4VSolid *TopVeto = new G4Box("TopVeto",0.80*m,0.0050*m,0.0508*m);
  G4LogicalVolume *TopVetoLV = new G4LogicalVolume(TopVeto,
	NpolMaterials::GetInstance()->GetMaterial("Scint"), "TopVetoLV",0,0,0);
  
  G4AssemblyVolume *TopVetoArray1 = MakePlate(TopVetoLV, 13, 0.0*m, 0.0*m, 0.60*m, 0.0*m, 0.0*m, 0.10*m);
  G4AssemblyVolume *TopVetoArray2 = MakePlate(TopVetoLV, 14, 0.0*m, 0.0*m, 0.60*m, 0.0*m, 0.0*m, 0.10*m);
  
  ImprintPlate(TopVetoArray1, motherLV, 0.0*m, 0.32*m, -2.115*m, 0.0*deg); // -2.13 cm
  ImprintPlate(TopVetoArray1, motherLV, 0.0*m, 0.42*m, -0.815*m, 0.0*deg);  // -0.83 cm
  ImprintPlate(TopVetoArray2, motherLV, 0.0*m, 0.52*m, 0.615*m, 0.0*deg);  // 0.59 cm
  
  G4VisAttributes* TopaVisAtt= new G4VisAttributes(G4Colour(0.0,1.0,1.0));
  TopVetoLV->SetVisAttributes(TopaVisAtt);
}


//------------------------------
// Bottom E detectors
// Andrei's 2012 Plan B setup using 160 cm long 10cm by 10cm detectors
//------------------------------
void NpolPolarimeter6Layer::ConstructBottomDetArray(G4LogicalVolume *motherLV) {
  
  G4VSolid *BottomDet = new G4Box("BottomDet",0.80*m,0.0508*m,0.0508*m);
  G4LogicalVolume *BottomDetLV = new G4LogicalVolume(BottomDet,
	NpolMaterials::GetInstance()->GetMaterial("Scint"), "BottomDetLV",0,0,0);
  
  G4AssemblyVolume *BottomDetArray1 = MakePlate(BottomDetLV,
	13, 0.0*m, 0.0*m, 0.60*m, 0.0*m, 0.0*m, 0.10*m);
  G4AssemblyVolume *BottomDetArray2 = MakePlate(BottomDetLV,
	14, 0.0*m, 0.0*m, 0.60*m, 0.0*m, 0.0*m, 0.10*m);

  ImprintPlate(BottomDetArray1, motherLV, 0.615*m, -0.70*m, -2.10*m, 45.0*deg);
  ImprintPlate(BottomDetArray1, motherLV, -0.615*m, -0.70*m, -2.10*m, 135.0*deg);
  ImprintPlate(BottomDetArray1, motherLV, 0.615*m, -0.80*m, -0.80*m, 45.0*deg);
  ImprintPlate(BottomDetArray1, motherLV, -0.615*m, -0.80*m, -0.80*m, 135.0*deg);
  ImprintPlate(BottomDetArray2, motherLV, 0.615*m, -0.90*m, 0.60*m, 45.0*deg);
  ImprintPlate(BottomDetArray2, motherLV, -0.615*m, -0.90*m, 0.60*m, 135.0*deg);
  
  G4VisAttributes *BotVisAtt= new G4VisAttributes(G4Colour(1.0,0.0,1.0));
  BottomDetLV->SetVisAttributes(BotVisAtt);
}

//-----------------------------
// BOTTOMa dE detectors
// Andrei's 2012 Plan B setup using 160 cm long "tagger" detectors 1cm thick
//-----------------------------
void NpolPolarimeter6Layer::ConstructBottomVetoArray(G4LogicalVolume *motherLV) {
  
  G4VSolid *BottomVeto = new G4Box("BottomVeto",0.80*m,0.0050*m,0.0508*m);
  G4LogicalVolume *BottomVetoLV = new G4LogicalVolume(BottomVeto,
	NpolMaterials::GetInstance()->GetMaterial("Scint"), "BottomVetoLV",0,0,0);

  G4AssemblyVolume *BottomVetoArray1 = MakePlate(BottomVetoLV,
	 13, 0.0*m, 0.0*m, 0.60*m, 0.0*m, 0.0*m, 0.10*m);
  G4AssemblyVolume *BottomVetoArray2 = MakePlate(BottomVetoLV,
	14, 0.0*m, 0.0*m, 0.60*m, 0.0*m, 0.0*m, 0.10*m);

  ImprintPlate(BottomVetoArray1, motherLV, 0.0*m, -0.32*m, -2.115*m, 0.0*deg); // -2.13 cm
  ImprintPlate(BottomVetoArray1, motherLV, 0.0*m, -0.42*m, -0.815*m, 0.0*deg); // -0.83 cm
  ImprintPlate(BottomVetoArray2, motherLV, 0.0*m, -0.52*m, 0.615*m, 0.0*deg);  // 0.59 cm
  
  G4VisAttributes* BotaVisAtt= new G4VisAttributes(G4Colour(0.0,1.0,1.0));
  BottomVetoLV->SetVisAttributes(BotaVisAtt);
}

void NpolPolarimeter6Layer::ConstructFrontDetArray(G4LogicalVolume *motherLV) {
  
  G4ThreeVector Tm;
  G4Transform3D Tr;
  
  G4VSolid *FrontDet = new G4Box("FrontDet",0.50*m,0.0508*m,/*0.0750*m*/ /*0.0508*m*/ 0.0254*m);
  G4LogicalVolume *FrontDetLV = new G4LogicalVolume(FrontDet,
	NpolMaterials::GetInstance()->GetMaterial("Scint"), "FrontDetLV",0,0,0);
  
  G4AssemblyVolume *FrontDetArray1 = MakePlate(FrontDetLV, 6, 0.0*m, 0.25*m, 0.0*m, 0.0*m, 0.10*m, 0.0*m);
  G4AssemblyVolume *FrontDetArray2 = MakePlate(FrontDetLV, 8, 0.0*m, 0.35*m, 0.0*m, 0.0*m, 0.10*m, 0.0*m);
  G4AssemblyVolume *FrontDetArray3 = MakePlate(FrontDetLV, 10, 0.0*m, 0.45*m, 0.0*m, 0.0*m, 0.10*m, 0.0*m);

  // Rotation of assembly inside the world
  G4RotationMatrix Rm; 
  Rm.rotateX(0.0*deg);
  Rm.rotateY(0.0*deg);
  
  for(unsigned int i=0; i<2; i++)
    ImprintPlate(FrontDetArray1, motherLV, 0.0*m, 0.0*m, (-2.6992+0.65*i)*m, 0.0*deg);
  for(unsigned int i=2; i<4; i++)
    ImprintPlate(FrontDetArray2, motherLV, 0.0*m, 0.0*m, (-2.6992+0.65*i)*m, 0.0*deg);
  for(unsigned int i=4; i<6; i++)
    ImprintPlate(FrontDetArray3, motherLV, 0.0*m, 0.0*m, (-2.6992+0.65*i)*m, 0.0*deg);

  
  G4VisAttributes* FrontDetVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,0.0));
  FrontDetLV->SetVisAttributes(FrontDetVisAtt);
}

//---------------------------------------------------------------------
// Taggers for analyzer layers
// 1.0 cm by 10.0 cm by 100.0 cm
//----------------------------------------------------------------------
void NpolPolarimeter6Layer::ConstructFrontTagArray(G4LogicalVolume *motherLV) {
  
  
  G4ThreeVector Tm;
  G4Transform3D Tr; 
  
  G4VSolid *FrontTag = new G4Box("FrontTag",0.50*m,0.0508*m,0.00508*m);
  G4LogicalVolume *FrontTagLV = new G4LogicalVolume(FrontTag,
	NpolMaterials::GetInstance()->GetMaterial("Scint"), "FrontTagLV",0,0,0);
  
  G4AssemblyVolume *FrontTaggerArray1 = MakePlate(FrontTagLV,6, 0.0*m, 0.25*m, 0.0*m, 0.0*m, 0.10*m, 0.0*m);
  G4AssemblyVolume *FrontTaggerArray2 = MakePlate(FrontTagLV,8, 0.0*m, 0.35*m, 0.0*m, 0.0*m, 0.10*m, 0.0*m);
  G4AssemblyVolume *FrontTaggerArray3 = MakePlate(FrontTagLV,10, 0.0*m, 0.45*m, 0.0*m, 0.0*m, 0.10*m, 0.0*m);

  for(unsigned int i=0; i<2; i++)
    ImprintPlate(FrontTaggerArray1, motherLV, 0.0*m, 0.0*m,(-2.7442+0.65*i)*m, 0.0*deg); // 1.7692 for 10cm thick Front
  for(unsigned int i=2; i<4; i++)
    ImprintPlate(FrontTaggerArray2, motherLV, 0.0*m, 0.0*m,(-2.7442+0.65*i)*m, 0.0*deg);
  for(unsigned int i=4; i<6; i++)
    ImprintPlate(FrontTaggerArray3, motherLV, 0.0*m, 0.0*m,(-2.7442+0.65*i)*m, 0.0*deg);
  

  G4VisAttributes* FrontTagVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  FrontTagLV->SetVisAttributes(FrontTagVisAtt);
}

void NpolPolarimeter6Layer::ConstructBackTagArray(G4LogicalVolume *motherLV) {
  
  
  G4ThreeVector Tm;
  G4Transform3D Tr; 
  
  G4VSolid *BackTag = new G4Box("BackTag",0.80*m,0.0508*m,0.00508*m);
  G4LogicalVolume *BackTagLV = new G4LogicalVolume(BackTag,
	NpolMaterials::GetInstance()->GetMaterial("Scint"), "BackTagLV",0,0,0);
  
  G4AssemblyVolume *BackTaggerArray = MakePlate(BackTagLV,
	  16, 0.0*m, 0.75*m, 0.0*m, 0.0*m, 0.10*m, 0.0*m);

  //for(unsigned int i=0; i<2; i++)
 
  ImprintPlate(BackTaggerArray, motherLV, 0.0*m, 0.0*m,1.60*m, 0.0*deg);
  ImprintPlate(BackTaggerArray, motherLV, 0.0*m, 0.0*m,1.62*m, 90.0*deg);
 
  G4VisAttributes* BackTagVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  BackTagLV->SetVisAttributes(BackTagVisAtt);
}


void NpolPolarimeter6Layer::Place(G4LogicalVolume *motherLV) {
  
  PlaceCylindrical(PolarimeterLV,motherLV,"Polarimeter",9.7*m,-28.0*deg /*0.0*deg*/,0.0*m);
}

