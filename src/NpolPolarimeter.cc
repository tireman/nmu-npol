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

#include <vector>

#include "G4PhysicalConstants.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4AssemblyVolume.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4ios.hh"

#include "NpolMaterials.hh"
#include "NpolPolarimeter.hh"
#include "NpolAnalysisManager.hh"
#include "NpolAnalysis.hh"

NpolPolarimeter::NpolPolarimeter() {
	G4VSolid *PolarimeterBox = new G4Box("PolarimeterBox",1.30*m,1.45*m,1.85*m);
	PolarimeterLV = new G4LogicalVolume(PolarimeterBox,
			NpolMaterials::GetInstance()->GetAir(), "PolarimeterLV",0,0,0);
	//G4VisAttributes* PolarimeterVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,0.0));
	//PolarimeterLV->SetVisAttributes(PolarimeterVisAtt);
	PolarimeterLV->SetVisAttributes(G4VisAttributes::GetInvisible());

	ConstructTopDetArray(PolarimeterLV);
	ConstructTopVetoArray(PolarimeterLV);
	ConstructBottomDetArray(PolarimeterLV);
	ConstructBottomVetoArray(PolarimeterLV);
	ConstructFrontDetArray(PolarimeterLV);
	ConstructFrontTagArray(PolarimeterLV);
}

NpolPolarimeter::~NpolPolarimeter() {}

G4String NpolPolarimeter::GetName() {
	return G4String("Polarimeter");
}

G4AssemblyVolume *NpolPolarimeter::MakePlate(G4LogicalVolume *detLV, \
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

void NpolPolarimeter::ActivateImprintedPVs(G4AssemblyVolume *AV, int arrayNo = 0) {

	NpolAnalysisManager *analysisMan = NpolAnalysisManager::GetInstance();
	std::vector<G4VPhysicalVolume *>::iterator AVit = AV->GetVolumesIterator();
	G4int numImprinted = AV->TotalImprintedVolumes();
	G4String LVName = (*AVit)->GetLogicalVolume()->GetName();
	G4String histoName;
	G4String histoTitle;
	char num[5];
	int i;

	if(LVName == "TopDetLV") {
		for(i = 0; i < numImprinted; i++) {
			sprintf(num,"%02d%02d",arrayNo,i+1);
			histoName = G4String("TopDet") + num;
			histoTitle = G4String("Top Array ") + arrayNo + G4String(": Detector ") + num;
			analysisMan->RegisterActiveDetectorEDepHistogram(*(AVit++), histoName, histoTitle, 100, 0., 20*MeV);
		}
	} else if(LVName == "TopVetoLV") {
		for(i = 0; i < numImprinted; i++) {
			sprintf(num,"%02d%02d",arrayNo,i+1);
			histoName = G4String("TopVeto") + num;
			histoTitle = G4String("Top Veto Array ") + arrayNo + G4String(": Detector ") + num;
			analysisMan->RegisterActiveDetectorEDepHistogram(*(AVit++), histoName, histoTitle, 100, 0., 20*MeV);
		}
	} else if(LVName == "BottomDetLV") {
		for(i = 0; i < numImprinted; i++) {
			sprintf(num,"%02d%02d",arrayNo,i+1);
			histoName = G4String("BottomDet") + num;
			histoTitle = G4String("Bottom Array ") + arrayNo + G4String(": Detector ") + num;
			analysisMan->RegisterActiveDetectorEDepHistogram(*(AVit++), histoName, histoTitle, 100, 0., 20*MeV);
		}
	} else if(LVName == "BottomVetoLV") {
		for(i = 0; i < numImprinted; i++) {
			sprintf(num,"%02d%02d",arrayNo,i+1);
			histoName = G4String("BottomVeto") + num;
			histoTitle = G4String("Bottom Veto Array ") + arrayNo + G4String(": Detector ") + num;
			analysisMan->RegisterActiveDetectorEDepHistogram(*(AVit++), histoName, histoTitle, 100, 0., 20*MeV);
		}
	} else if(LVName == "FrontDetLV") {
		for(i = 0; i < numImprinted; i++) {
			sprintf(num,"%02d%02d",arrayNo,i+1);
			histoName = G4String("FrontDet") + num;
			histoTitle = G4String("Front Array ") + arrayNo + G4String(": Detector ") + num;
			analysisMan->RegisterActiveDetectorEDepHistogram(*(AVit++), histoName, histoTitle, 100, 0., 20*MeV);
		}
	} else if(LVName == "FrontTagLV") {
		for(i = 0; i < numImprinted; i++) {
			sprintf(num,"%02d%02d",arrayNo,i+1);
			histoName = G4String("FrontTag") + num;
			histoTitle = G4String("Front Tagger Array ") + arrayNo + G4String(": Detector ") + num;
			analysisMan->RegisterActiveDetectorEDepHistogram(*(AVit++), histoName, histoTitle, 100, 0., 20*MeV);
		}
	}

}

//---------------------------
// Top E detectors
// Andrei's 2012 Plan B setup using 160 cm long detectors 10cm by 10cm
//---------------------------
void NpolPolarimeter::ConstructTopDetArray(G4LogicalVolume *motherLV) {

	G4VSolid *TopDet = new G4Box("TopDet",0.80*m,0.0508*m,0.0508*m);
	G4LogicalVolume *TopDetLV = new G4LogicalVolume(TopDet,
			NpolMaterials::GetInstance()->GetScint(),"TopDetLV",0,0,0);

	G4AssemblyVolume *TopDetArray1 = MakePlate(TopDetLV, 13, 0.0*m, 0.0*m, 0.60*m, 0.0*m, 0.0*m, 0.10*m);
	G4AssemblyVolume *TopDetArray2 = MakePlate(TopDetLV, 14, 0.0*m, 0.0*m, 0.60*m, 0.0*m, 0.0*m, 0.10*m);

	ImprintPlate(TopDetArray1, motherLV, 0.615*m, 0.70*m, -1.10*m, -45.0*deg);
	ImprintPlate(TopDetArray1, motherLV, -0.615*m, 0.70*m, -1.10*m, 45.0*deg);
	ImprintPlate(TopDetArray2, motherLV, 0.615*m, 0.80*m, 0.30*m, -45.0*deg);
	ImprintPlate(TopDetArray2, motherLV, -0.615*m, 0.80*m, 0.30*m, 45.0*deg);

	ActivateImprintedPVs(TopDetArray1, 1);
	ActivateImprintedPVs(TopDetArray2, 2);

	G4VisAttributes *TopVisAtt= new G4VisAttributes(G4Colour(1.0,0.0,1.0));
	TopDetLV->SetVisAttributes(TopVisAtt);
}

//----------------------------
// Top dE detectors
// Andrei's 2012 Plan B 160 cm long "taggers" in a horizontal orientation
//----------------------------
void NpolPolarimeter::ConstructTopVetoArray(G4LogicalVolume *motherLV) {

	G4VSolid *TopVeto = new G4Box("TopVeto",0.80*m,0.0050*m,0.0508*m);
	G4LogicalVolume *TopVetoLV = new G4LogicalVolume(TopVeto,
			NpolMaterials::GetInstance()->GetScint(), "TopVetoLV",0,0,0);

	G4AssemblyVolume *TopVetoArray1 = MakePlate(TopVetoLV, 13, 0.0*m, 0.0*m, 0.60*m, 0.0*m, 0.0*m, 0.10*m);
	G4AssemblyVolume *TopVetoArray2 = MakePlate(TopVetoLV, 14, 0.0*m, 0.0*m, 0.60*m, 0.0*m, 0.0*m, 0.10*m);

	ImprintPlate(TopVetoArray1, motherLV, 0.0*m, 0.32*m, -1.15*m, 0.0*deg);
	ImprintPlate(TopVetoArray2, motherLV, 0.0*m, 0.42*m, 0.30*m, 0.0*deg);

	ActivateImprintedPVs(TopVetoArray1, 1);
	ActivateImprintedPVs(TopVetoArray2, 2);

	G4VisAttributes* TopaVisAtt= new G4VisAttributes(G4Colour(0.0,1.0,1.0));
	TopVetoLV->SetVisAttributes(TopaVisAtt);
}


//------------------------------
// Bottom E detectors
// Andrei's 2012 Plan B setup using 160 cm long 10cm by 10cm detectors
//------------------------------
void NpolPolarimeter::ConstructBottomDetArray(G4LogicalVolume *motherLV) {

	G4VSolid *BottomDet = new G4Box("BottomDet",0.80*m,0.0508*m,0.0508*m);
	G4LogicalVolume *BottomDetLV = new G4LogicalVolume(BottomDet,
			NpolMaterials::GetInstance()->GetScint(), "BottomDetLV",0,0,0);

	G4AssemblyVolume *BottomDetArray1 = MakePlate(BottomDetLV,
			13, 0.0*m, 0.0*m, 0.60*m, 0.0*m, 0.0*m, 0.10*m);
	G4AssemblyVolume *BottomDetArray2 = MakePlate(BottomDetLV,
			14, 0.0*m, 0.0*m, 0.60*m, 0.0*m, 0.0*m, 0.10*m);

	ImprintPlate(BottomDetArray1, motherLV, 0.615*m, -0.70*m, -1.10*m, 45.0*deg);
	ImprintPlate(BottomDetArray1, motherLV, -0.615*m, -0.70*m, -1.10*m, 135.0*deg);
	ImprintPlate(BottomDetArray2, motherLV, 0.615*m, -0.80*m, 0.30*m, 45.0*deg);
	ImprintPlate(BottomDetArray2, motherLV, -0.615*m, -0.80*m, 0.30*m, 135.0*deg);

	ActivateImprintedPVs(BottomDetArray1, 1);
	ActivateImprintedPVs(BottomDetArray2, 2);

	G4VisAttributes *BotVisAtt= new G4VisAttributes(G4Colour(1.0,0.0,1.0));
	BottomDetLV->SetVisAttributes(BotVisAtt);
}

//-----------------------------
// BOTTOMa dE detectors
// Andrei's 2012 Plan B setup using 160 cm long "tagger" detectors 1cm thick
//-----------------------------
void NpolPolarimeter::ConstructBottomVetoArray(G4LogicalVolume *motherLV) {

	G4VSolid *BottomVeto = new G4Box("BottomVeto",0.80*m,0.0050*m,0.0508*m);
	G4LogicalVolume *BottomVetoLV = new G4LogicalVolume(BottomVeto,
			NpolMaterials::GetInstance()->GetScint(), "BottomVetoLV",0,0,0);

	G4AssemblyVolume *BottomVetoArray1 = MakePlate(BottomVetoLV,
			13, 0.0*m, 0.0*m, 0.60*m, 0.0*m, 0.0*m, 0.10*m);
	G4AssemblyVolume *BottomVetoArray2 = MakePlate(BottomVetoLV,
			13, 0.0*m, 0.0*m, 0.60*m, 0.0*m, 0.0*m, 0.10*m);

	ImprintPlate(BottomVetoArray1, motherLV, 0.0*m, -0.32*m, -1.15*m, 0.0*deg);
	ImprintPlate(BottomVetoArray2, motherLV, 0.0*m, -0.42*m, 0.30*m, 0.0*deg);

	ActivateImprintedPVs(BottomVetoArray1, 1);
	ActivateImprintedPVs(BottomVetoArray2, 2);

	G4VisAttributes* BotaVisAtt= new G4VisAttributes(G4Colour(0.0,1.0,1.0));
	BottomVetoLV->SetVisAttributes(BotaVisAtt);
}

void NpolPolarimeter::ConstructFrontDetArray(G4LogicalVolume *motherLV) {

	G4ThreeVector Tm;
	G4Transform3D Tr;

	G4VSolid *FrontDet = new G4Box("FrontDet",0.50*m,0.0508*m,0.0508*m);
	G4LogicalVolume *FrontDetLV = new G4LogicalVolume(FrontDet,
			NpolMaterials::GetInstance()->GetScint(), "FrontDetLV",0,0,0);

	G4AssemblyVolume *FrontDetArray1 = MakePlate(FrontDetLV, 6, 0.0*m, 0.25*m, 0.0*m, 0.0*m, 0.10*m, 0.0*m);
	G4AssemblyVolume *FrontDetArray2 = MakePlate(FrontDetLV, 8, 0.0*m, 0.35*m, 0.0*m, 0.0*m, 0.10*m, 0.0*m);

	// Rotation of assembly inside the world
	G4RotationMatrix Rm; 
	Rm.rotateX(0.0*deg);
	Rm.rotateY(0.0*deg);

	for(unsigned int i=0; i<2; i++)
		ImprintPlate(FrontDetArray1, motherLV, 0.0*m, 0.0*m, (-1.6992+0.65*i)*m, 0.0*deg);
	for(unsigned int i=2; i<4; i++)
		ImprintPlate(FrontDetArray2, motherLV, 0.0*m, 0.0*m, (-1.6992+0.65*i)*m, 0.0*deg);

	ActivateImprintedPVs(FrontDetArray1, 1);
	ActivateImprintedPVs(FrontDetArray2, 2);

	G4VisAttributes* FrontDetVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,0.0));
	FrontDetLV->SetVisAttributes(FrontDetVisAtt);
}

//---------------------------------------------------------------------
// Taggers for analyzer layers
// 1.0 cm by 10.0 cm by 100.0 cm
//----------------------------------------------------------------------
void NpolPolarimeter::ConstructFrontTagArray(G4LogicalVolume *motherLV) {


	G4ThreeVector Tm;
	G4Transform3D Tr; 

	G4VSolid *FrontTag = new G4Box("FrontTag",0.50*m,0.0508*m,0.00508*m);
	G4LogicalVolume *FrontTagLV = new G4LogicalVolume(FrontTag,
			NpolMaterials::GetInstance()->GetScint(), "FrontTagLV",0,0,0);

	G4AssemblyVolume *FrontTaggerArray1 = MakePlate(FrontTagLV,
			6, 0.0*m, 0.25*m, 0.0*m, 0.0*m, 0.10*m, 0.0*m);
	G4AssemblyVolume *FrontTaggerArray2 = MakePlate(FrontTagLV,
			8, 0.0*m, 0.35*m, 0.0*m, 0.0*m, 0.10*m, 0.0*m);

	for(unsigned int i=0; i<2; i++)
		ImprintPlate(FrontTaggerArray1, motherLV, 0.0*m, 0.0*m, 
				(-1.7692+0.65*i)*m, 0.0*deg);
	for(unsigned int i=2; i<4; i++)
		ImprintPlate(FrontTaggerArray2, motherLV, 0.0*m, 0.0*m, 
				(-1.7692+0.65*i)*m, 0.0*deg);

	ActivateImprintedPVs(FrontTaggerArray1, 1);
	ActivateImprintedPVs(FrontTaggerArray2, 2);

	G4VisAttributes* FrontTagVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
	FrontTagLV->SetVisAttributes(FrontTagVisAtt);
}

void NpolPolarimeter::Place(G4LogicalVolume *motherLV) {

	PlaceCylindrical(PolarimeterLV,motherLV,"Polarimeter",8.7*m,-28.0*deg,0.0*m);
}

