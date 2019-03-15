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
// This code currently (March 2018) generates NPOL volumes with
// names of the following scheme av_xxx_impr_yyy_VolName_pv_zzz
// xxx is the assembly volume number (runs from 1 to # of assemblies)
// yyy is the imprint number of the assembly for multiple placements
//     of the same assembly (runs from 1 to # of imprints)
// zzz is the physical volume number for detectors in an assembly
//     (runs from 0 to the # of detectors in the assembly)
// VolName is the physical volume name given when it is created

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

G4double NpolPolarimeter::NDetThickness = 10.0*cm;
G4double NpolPolarimeter::NVetoThickness = 1.00*cm;
G4double NpolPolarimeter::NDetHeight = 10.0*cm;
G4double NpolPolarimeter::NDetStandardLength = 100.0*cm;
G4double NpolPolarimeter::dEArrayVertOffset = 0.42*m; // orig was 0.32*m
G4double NpolPolarimeter::EarrayRotAngle = 45.0*deg;
G4double NpolPolarimeter::FirstAnalyzerLayerPos = 7.0*m;
G4double NpolPolarimeter::FrontDetSpacing = 0.65*m;
G4double NpolPolarimeter::FrontVetoOffset = 7.0*cm;
G4double NpolPolarimeter::BackTaggerzPos = 10.50*m;
G4double NpolPolarimeter::NpolAng = 28.0*deg; // 28.0*deg; nominal

NpolPolarimeter::NpolPolarimeter() {

}

NpolPolarimeter::~NpolPolarimeter() {}

G4String NpolPolarimeter::GetName() {
  return G4String("Polarimeter");
}


G4AssemblyVolume *NpolPolarimeter::MakePlate(G4LogicalVolume *detLV, G4int numDets, G4double TmX, G4double TmY, G4double TmZ, G4double TmdX, G4double TmdY, G4double TmdZ) {
  
  G4AssemblyVolume *plate = new G4AssemblyVolume();
  
  // Translation and rotation of plate inside assembly
  G4RotationMatrix Rm; 
  G4ThreeVector Tm;
  G4Transform3D Tr;

  int i;
  for(i=0; i<numDets; i++) {
    Tm.setX(TmX-TmdX*i); Tm.setY(TmY-TmdY*i); Tm.setZ(TmZ-TmdZ*i);
    Tr = G4Transform3D(Rm,Tm);
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

void NpolPolarimeter::ImprintPlate(G4AssemblyVolume *plate, G4LogicalVolume *motherLV, G4ThreeVector Tm, G4RotationMatrix Rm) {
  
  G4Transform3D Tr;
  Tr = G4Transform3D(Rm,Tm);
  
  plate->MakeImprint(motherLV,Tr);
}

//---------------------------
// Top E detectors
// Andrei's 2012 Plan B setup using 160 cm long detectors 10cm by 10cm
//---------------------------
void NpolPolarimeter::ConstructTopDetArray(G4LogicalVolume *motherLV) {
  G4double EarrayLength = NDetStandardLength + 60.0*cm;
  G4double Earray1RadialOffset = 10.0*cm*6, Earray2RadialOffset = 10.0*cm*19;
  G4double xOffsetArray = (EarrayLength)/2 * cos(EarrayRotAngle);
  G4double xOffsetOverlap = NDetThickness/2 * sin(EarrayRotAngle);
  G4double PxPos = 0.0*m; G4double PyPos = 0.0*m; G4double PzPos = -0.60*m;
  
  G4double xPos1L = -(FirstAnalyzerLayerPos + Earray1RadialOffset) * sin(NpolAng) +  xOffsetArray*cos(NpolAng) + xOffsetOverlap*cos(NpolAng);
  G4double xPos1R = -(FirstAnalyzerLayerPos + Earray1RadialOffset) * sin(NpolAng) - xOffsetArray*cos(NpolAng) - xOffsetOverlap*cos(NpolAng);
  
  G4double xPos2L = -(FirstAnalyzerLayerPos + Earray2RadialOffset) * sin(NpolAng) +  xOffsetArray*cos(NpolAng) + xOffsetOverlap*cos(NpolAng);
  G4double xPos2R = -(FirstAnalyzerLayerPos + Earray2RadialOffset) * sin(NpolAng) - xOffsetArray*cos(NpolAng) - xOffsetOverlap*cos(NpolAng);
   
  G4double yPos1 = (EarrayLength)/2 * sin(EarrayRotAngle) + 40.0*cm;
  G4double yPos2 = (EarrayLength)/2 * sin(EarrayRotAngle) + 50.0*cm;
  
  G4double zPos1L = (FirstAnalyzerLayerPos + Earray1RadialOffset) * cos(NpolAng) + xOffsetArray*sin(NpolAng) + xOffsetOverlap*sin(NpolAng);
  G4double zPos1R = (FirstAnalyzerLayerPos + Earray1RadialOffset) * cos(NpolAng) - xOffsetArray*sin(NpolAng) - xOffsetOverlap*sin(NpolAng);
  
  G4double zPos2L = (FirstAnalyzerLayerPos + Earray2RadialOffset) * cos(NpolAng) + xOffsetArray*sin(NpolAng) + xOffsetOverlap*sin(NpolAng);
  G4double zPos2R = (FirstAnalyzerLayerPos + Earray2RadialOffset) * cos(NpolAng) - xOffsetArray*sin(NpolAng) - xOffsetOverlap*sin(NpolAng);
  
  G4VSolid *TopDet = new G4Box("TopDet",(EarrayLength)/2,NDetHeight/2,NDetThickness/2);
  G4LogicalVolume *TopDetLV = new G4LogicalVolume(TopDet,NpolMaterials::GetInstance()->GetMaterial("Scint"),"TopDetLV",0,0,0);

  G4AssemblyVolume *TopDetArray1 = MakePlate(TopDetLV, 13, PxPos, PyPos, PzPos, 0.0*m, 0.0*m, -0.10*m);
  G4AssemblyVolume *TopDetArray2 = MakePlate(TopDetLV, 14, PxPos, PyPos, PzPos, 0.0*m, 0.0*m, -0.10*m);

  G4RotationMatrix ra,rm,rm2;
  ra.rotateX(0.0*deg); ra.rotateY(-NpolAng); ra.rotateZ(0.0*deg);
  rm.rotateX(0.0*deg); rm.rotateY(0.0*deg); rm.rotateZ(-45.0*deg);
  rm2.rotateX(0.0*deg); rm2.rotateY(0.0*deg); rm2.rotateZ(+45.0*deg);
  
  ImprintPlate(TopDetArray1, motherLV, G4ThreeVector(xPos1L, yPos1, zPos1L),ra*rm);
  ImprintPlate(TopDetArray1, motherLV, G4ThreeVector(xPos1R, yPos1, zPos1R),ra*rm2);
  
  ImprintPlate(TopDetArray2, motherLV, G4ThreeVector(xPos2L, yPos2, zPos2L),ra*rm);
  ImprintPlate(TopDetArray2, motherLV, G4ThreeVector(xPos2R, yPos2, zPos2R),ra*rm2);

  G4VisAttributes *TopVisAtt= new G4VisAttributes(G4Colour(1.0,0.5,0.0));
  TopDetLV->SetVisAttributes(TopVisAtt);
}

//----------------------------
// Top dE detectors
// Andrei's 2012 Plan B 160 cm long "taggers" in a horizontal orientation
//----------------------------
void NpolPolarimeter::ConstructTopVetoArray(G4LogicalVolume *motherLV) {
  G4double PxPos = 0.0*m; G4double PyPos = 0.0*m; G4double PzPos = 0.0*m;
  
  G4double xPos1 = -(FirstAnalyzerLayerPos + 10.0*cm*12) * sin(NpolAng);
  G4double xPos2 = -(FirstAnalyzerLayerPos + 10.0*cm*26) * sin(NpolAng);
  G4double yPos1 = dEArrayVertOffset; 
  G4double yPos2 = dEArrayVertOffset + 0.10*m;
  G4double zPos1 = (FirstAnalyzerLayerPos + 10.0*cm*12) * cos(NpolAng);
  G4double zPos2 = (FirstAnalyzerLayerPos + 10.0*cm*26) * cos(NpolAng);
  G4VSolid *TopVeto = new G4Box("TopVeto",(NDetStandardLength + 60.0*cm)/2,NVetoThickness/2,NDetHeight/2);
  G4LogicalVolume *TopVetoLV = new G4LogicalVolume(TopVeto,NpolMaterials::GetInstance()->GetMaterial("Scint"), "TopVetoLV",0,0,0);
  
  G4AssemblyVolume *TopVetoArray1 = MakePlate(TopVetoLV, 13, PxPos,PyPos,PzPos, 0.0*m, 0.0*m, 0.10*m);
  G4AssemblyVolume *TopVetoArray2 = MakePlate(TopVetoLV, 14, PxPos,PyPos,PzPos, 0.0*m, 0.0*m, 0.10*m);

  G4RotationMatrix ra;
  ra.rotateX(0.0*deg); ra.rotateY(-NpolAng); ra.rotateZ(0.0*deg);
  
  ImprintPlate(TopVetoArray1, motherLV,G4ThreeVector(xPos1, yPos1, zPos1), ra); 
  ImprintPlate(TopVetoArray2, motherLV,G4ThreeVector(xPos2, yPos2, zPos2), ra);
  
  G4VisAttributes* TopaVisAtt= new G4VisAttributes(G4Colour(0.0,1.0,1.0));
  TopVetoLV->SetVisAttributes(TopaVisAtt);
}


//------------------------------
// Bottom E detectors
// Andrei's 2012 Plan B setup using 160 cm long 10cm by 10cm detectors
//------------------------------
void NpolPolarimeter::ConstructBottomDetArray(G4LogicalVolume *motherLV) {
  G4double EarrayLength = NDetStandardLength + 60.0*cm;
  G4double Earray1RadialOffset = 10.0*cm*6, Earray2RadialOffset = 10.0*cm*19;
  G4double xOffsetArray = (EarrayLength)/2 * cos(EarrayRotAngle);
  G4double xOffsetOverlap = NDetThickness/2 * sin(EarrayRotAngle);
  G4double PxPos = 0.0*m; G4double PyPos = 0.0*m; G4double PzPos = -0.60*m;
  
  G4double xPos1L = -(FirstAnalyzerLayerPos + Earray1RadialOffset) * sin(NpolAng) +  xOffsetArray*cos(NpolAng) + xOffsetOverlap*cos(NpolAng);
  G4double xPos1R = -(FirstAnalyzerLayerPos + Earray1RadialOffset) * sin(NpolAng) - xOffsetArray*cos(NpolAng) - xOffsetOverlap*cos(NpolAng);
  
  G4double xPos2L = -(FirstAnalyzerLayerPos + Earray2RadialOffset) * sin(NpolAng) +  xOffsetArray*cos(NpolAng) + xOffsetOverlap*cos(NpolAng);
  G4double xPos2R = -(FirstAnalyzerLayerPos + Earray2RadialOffset) * sin(NpolAng) - xOffsetArray*cos(NpolAng) - xOffsetOverlap*cos(NpolAng);
   
  G4double yPos1 = (EarrayLength)/2 * sin(EarrayRotAngle) + 40.0*cm;
  G4double yPos2 = (EarrayLength)/2 * sin(EarrayRotAngle) + 50.0*cm;
  
  G4double zPos1L = (FirstAnalyzerLayerPos + Earray1RadialOffset) * cos(NpolAng) + xOffsetArray*sin(NpolAng) + xOffsetOverlap*sin(NpolAng);
  G4double zPos1R = (FirstAnalyzerLayerPos + Earray1RadialOffset) * cos(NpolAng) - xOffsetArray*sin(NpolAng) - xOffsetOverlap*sin(NpolAng);
  
  G4double zPos2L = (FirstAnalyzerLayerPos + Earray2RadialOffset) * cos(NpolAng) + xOffsetArray*sin(NpolAng) + xOffsetOverlap*sin(NpolAng);
  G4double zPos2R = (FirstAnalyzerLayerPos + Earray2RadialOffset) * cos(NpolAng) - xOffsetArray*sin(NpolAng) - xOffsetOverlap*sin(NpolAng);
  
  G4VSolid *BottomDet = new G4Box("BottomDet",(NDetStandardLength + 60.0*cm)/2,NDetHeight/2,NDetThickness/2);
  G4LogicalVolume *BottomDetLV = new G4LogicalVolume(BottomDet,NpolMaterials::GetInstance()->GetMaterial("Scint"), "BottomDetLV",0,0,0);

  G4AssemblyVolume *BottomDetArray1 = MakePlate(BottomDetLV, 13, PxPos, PyPos, PzPos, 0.0*m, 0.0*m, -0.10*m);
  G4AssemblyVolume *BottomDetArray2 = MakePlate(BottomDetLV, 14, PxPos, PyPos, PzPos, 0.0*m, 0.0*m, -0.10*m);

  G4RotationMatrix ra,rm,rm2;
  ra.rotateX(0.0*deg); ra.rotateY(-NpolAng); ra.rotateZ(0.0*deg);
  rm.rotateX(0.0*deg); rm.rotateY(0.0*deg); rm.rotateZ(+45.0*deg);
  rm2.rotateX(0.0*deg); rm2.rotateY(0.0*deg); rm2.rotateZ(-45.0*deg);
  
  ImprintPlate(BottomDetArray1, motherLV, G4ThreeVector(xPos1L, -yPos1, zPos1L),ra*rm);
  ImprintPlate(BottomDetArray1, motherLV, G4ThreeVector(xPos1R, -yPos1, zPos1R),ra*rm2);
  
  ImprintPlate(BottomDetArray2, motherLV, G4ThreeVector(xPos2L, -yPos2, zPos2L),ra*rm);
  ImprintPlate(BottomDetArray2, motherLV, G4ThreeVector(xPos2R, -yPos2, zPos2R),ra*rm2);
  
  G4VisAttributes *BotVisAtt= new G4VisAttributes(G4Colour(1.0,0.50,0.0));
  BottomDetLV->SetVisAttributes(BotVisAtt);
}

//-----------------------------
// BOTTOMa dE detectors
// Andrei's 2012 Plan B setup using 160 cm long "tagger" detectors 1cm thick
//-----------------------------
void NpolPolarimeter::ConstructBottomVetoArray(G4LogicalVolume *motherLV) {
  G4double PxPos = 0.0*m; G4double PyPos = 0.0*m; G4double PzPos = 0.0*m;

  G4double xPos1 = -(FirstAnalyzerLayerPos + 10.0*cm*12) * sin(NpolAng);
  G4double xPos2 = -(FirstAnalyzerLayerPos + 10.0*cm*26) * sin(NpolAng);
  G4double yPos1 = -dEArrayVertOffset; 
  G4double yPos2 = -(dEArrayVertOffset + 0.10*m);
  G4double zPos1 = (FirstAnalyzerLayerPos + 10.0*cm*12) * cos(NpolAng);
  G4double zPos2 = (FirstAnalyzerLayerPos + 10.0*cm*26) * cos(NpolAng);

  G4VSolid *BottomVeto = new G4Box("BottomVeto",(NDetStandardLength + 60.0*cm)/2,NVetoThickness/2,NDetHeight/2);
  G4LogicalVolume *BottomVetoLV = new G4LogicalVolume(BottomVeto,
	NpolMaterials::GetInstance()->GetMaterial("Scint"), "BottomVetoLV",0,0,0);
  
  G4RotationMatrix ra;
  ra.rotateX(0.0*deg); ra.rotateY(-NpolAng); ra.rotateZ(0.0*deg);

  G4AssemblyVolume *BottomVetoArray1 = MakePlate(BottomVetoLV,13,PxPos,PyPos,PzPos,0.0*m,0.0*m,0.10*m);
  G4AssemblyVolume *BottomVetoArray2 = MakePlate(BottomVetoLV,14,PxPos,PyPos,PzPos,0.0*m,0.0*m,0.10*m);

  ImprintPlate(BottomVetoArray1, motherLV,G4ThreeVector(xPos1, yPos1, zPos1), ra);
  ImprintPlate(BottomVetoArray2, motherLV,G4ThreeVector(xPos2, yPos2, zPos2), ra); 

  G4VisAttributes* BotaVisAtt= new G4VisAttributes(G4Colour(0.0,1.0,1.0));
  BottomVetoLV->SetVisAttributes(BotaVisAtt);
}

void NpolPolarimeter::ConstructFrontDetArray(G4LogicalVolume *motherLV) {
  G4double xPos, yPos, zPos;

  G4VSolid *FrontDet = new G4Box("FrontDet",NDetStandardLength/2,NDetHeight/2,NDetThickness/2);
  G4LogicalVolume *FrontDetLV = new G4LogicalVolume(FrontDet,NpolMaterials::GetInstance()->GetMaterial("Scint"), "FrontDetLV",0,0,0);
  
  G4AssemblyVolume *FrontDetArray1 = MakePlate(FrontDetLV,6,0.0*m,0.25*m,0.0*m,0.0*m,0.10*m,0.0*m);
  G4AssemblyVolume *FrontDetArray2 = MakePlate(FrontDetLV, 8,0.0*m,0.35*m,0.0*m,0.0*m,0.10*m,0.0*m);
  
  // Rotation of assembly inside the world
  G4RotationMatrix Rm; 
  Rm.rotateX(0.0*deg); Rm.rotateY(-NpolAng); Rm.rotateZ(0.0*deg);
  
  for(unsigned int i=0; i<4; i++){
	xPos = -(FirstAnalyzerLayerPos + FrontDetSpacing*i) * sin(NpolAng);
	yPos = 0.0*m;
	zPos = (FirstAnalyzerLayerPos + FrontDetSpacing*i) * cos(NpolAng);

	if(i < 2) ImprintPlate(FrontDetArray1,motherLV,G4ThreeVector(xPos,yPos,zPos),Rm);
	if(i >= 2) ImprintPlate(FrontDetArray2,motherLV,G4ThreeVector(xPos,yPos,zPos),Rm);
  }
  
  G4VisAttributes* FrontDetVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,0.0));
  FrontDetLV->SetVisAttributes(FrontDetVisAtt);
}

//---------------------------------------------------------------------
// Taggers for analyzer layers
// 1.0 cm by 10.0 cm by 100.0 cm
//----------------------------------------------------------------------
void NpolPolarimeter::ConstructFrontVetoArray(G4LogicalVolume *motherLV) {
  G4double xPos, yPos, zPos;
   
  G4VSolid *FrontVeto = new G4Box("FrontVeto",NDetStandardLength/2,NDetHeight/2,NVetoThickness/2);
  G4LogicalVolume *FrontVetoLV = new G4LogicalVolume(FrontVeto,
	NpolMaterials::GetInstance()->GetMaterial("Scint"), "FrontVetoLV",0,0,0);
  
  G4AssemblyVolume *FrontVetoArray1 = MakePlate(FrontVetoLV,6,0.0*m,0.25*m,0.0*m,0.0*m,0.10*m,0.0*m);
  G4AssemblyVolume *FrontVetoArray2 = MakePlate(FrontVetoLV,8,0.0*m,0.35*m,0.0*m,0.0*m,0.10*m,0.0*m);

  // Rotation of assembly inside the world
  G4RotationMatrix Rm; 
  Rm.rotateX(0.0*deg); Rm.rotateY(-NpolAng); Rm.rotateZ(0.0*deg);

  for(unsigned int i=0; i<4; i++){
	xPos = -(FirstAnalyzerLayerPos - FrontVetoOffset  + FrontDetSpacing*i) * sin(NpolAng);
	yPos = 0.0*m;
	zPos = (FirstAnalyzerLayerPos - FrontVetoOffset + FrontDetSpacing*i) * cos(NpolAng);
	
    if(i < 2) ImprintPlate(FrontVetoArray1, motherLV, G4ThreeVector(xPos,yPos,zPos),Rm); 
    if(i >= 2) ImprintPlate(FrontVetoArray2, motherLV, G4ThreeVector(xPos,yPos,zPos),Rm);
  }
  
  G4VisAttributes* FrontVetoVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  FrontVetoLV->SetVisAttributes(FrontVetoVisAtt);
}

void NpolPolarimeter::ConstructBackTagArray(G4LogicalVolume *motherLV) {
  G4double xPos, yPos, zPos;
   
  G4VSolid *BackTag = new G4Box("BackTag",(NDetStandardLength+60.0*cm)/2,NDetHeight/2,NVetoThickness/2);
  G4LogicalVolume *BackTagLV = new G4LogicalVolume(BackTag,
	NpolMaterials::GetInstance()->GetMaterial("Scint"), "BackTagLV",0,0,0);
  
  G4AssemblyVolume *BackTaggerArray = MakePlate(BackTagLV,16,0.0*m,0.75*m,0.0*m,0.0*m,0.10*m,0.0*m);
  // Rotation of assembly inside the world
  G4RotationMatrix Rm,Rm2; 
  Rm.rotateX(0.0*deg); Rm.rotateY(-NpolAng); Rm.rotateZ(0.0*deg);
  Rm2.rotateX(0.0*deg); Rm2.rotateY(0.0*deg); Rm2.rotateZ(90.0*deg);
  
  xPos = -BackTaggerzPos * sin(NpolAng); yPos = 0.0*m; zPos = BackTaggerzPos * cos(NpolAng);
  ImprintPlate(BackTaggerArray, motherLV,G4ThreeVector(xPos,yPos,zPos), Rm);

  xPos = -(BackTaggerzPos + 3.0*cm) * sin(NpolAng); zPos = (BackTaggerzPos + 3.0*cm) * cos(NpolAng);
  ImprintPlate(BackTaggerArray, motherLV,G4ThreeVector(xPos,yPos,zPos), Rm*Rm2);

  G4VisAttributes* BackTagVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  BackTagLV->SetVisAttributes(BackTagVisAtt);
}


void NpolPolarimeter::Place(G4LogicalVolume *motherLV) {

  ConstructTopDetArray(motherLV);
  ConstructTopVetoArray(motherLV);
  ConstructBottomDetArray(motherLV);
  ConstructBottomVetoArray(motherLV);
  ConstructFrontDetArray(motherLV);
  ConstructFrontVetoArray(motherLV);
  ConstructBackTagArray(motherLV);
  
}

