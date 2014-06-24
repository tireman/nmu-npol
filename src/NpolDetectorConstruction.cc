//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id: DetectorConstruction.cc,v 1.21.4.1 2010/04/01 09:14:35 gcosmo Exp $
// GEANT4 tag $Name: geant4-09-03-patch-01 $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4AssemblyVolume.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4ios.hh"

#include "NpolDetectorConstruction.hh"
#include "NpolSensitiveDetector.hh"

NpolDetectorConstruction::NpolDetectorConstruction()
{}

NpolDetectorConstruction::~NpolDetectorConstruction()
{}

G4VPhysicalVolume* NpolDetectorConstruction::Construct()
{
	//--------- Material definition ---------

	// Scint for NPOL
	G4Element* H = new G4Element("Hydrogen", "H", 1., 1.008*g/mole);
	G4Element* C = new G4Element("Carbon"  , "C", 6., 12.011*g/mole);

	G4Material* Scint = new G4Material("Scint", 1.02*g/cm3, 2);
	Scint->AddElement(H, 8.451*perCent);
	Scint->AddElement(C, 91.549*perCent);

	G4Material* Vac= new G4Material("Vacuum", 1.0, 1.01*g/mole, universe_mean_density,kStateGas, 2.73*kelvin, 3.e-18*pascal);


	// Print all the materials defined.
	//
	G4cout << G4endl << "The materials defined are : " << G4endl << G4endl;
	G4cout << *(G4Material::GetMaterialTable()) << G4endl;

	//--------- Definitions of Solids, Logical Volumes, Physical Volumes ---------

	//------------------------------ 
	// World
	//------------------------------ 

	G4Box *solidWorld= new G4Box("World",2.0*m,2.5*m, 2.5*m);
	G4LogicalVolume *logicWorld= new G4LogicalVolume( solidWorld, Vac, "World", 0, 0, 0);

	//  Must place the World Physical volume unrotated at (0,0,0).
	// 
	G4VPhysicalVolume *physiWorld = new G4PVPlacement(0, // no rotation
			G4ThreeVector(), // at (0,0,0)
			logicWorld,      // its logical volume
			"World",         // its name
			0,               // its mother  volume
			false,           // no boolean operations
			0);              // copy number

	//---------------------------
	// This is just stupid.  It must be cleaned up; if for nothing
	// but my sanity.  W.T. (2012)
	//
	// TOP/BOTTOM are the E arrays for proton total energy
	// TOPa/BOTTOMa are the dE arrays for particle ID (protons versus pions)
	//---------------------------

	G4VSolid *TopDet, *TopVeto, *BottomDet, *BottomVeto, *FrontDet, *FrontTag;
	G4AssemblyVolume *TopArray, *TopVetoArray, *BottomArray, *BottomVetoArray, *FrontArray1, *FrontArray2, *FrontTagger1, *FrontTagger2;

	//--------------------------
	// Top E detectors
	// Andrei's 2012 Plan B setup using 160 cm long detectors 10cm by 10cm
	//---------------------------

	TopDet = new G4Box("TopDet",0.80*m,0.0508*m,0.0508*m);
	TopDetLV = new G4LogicalVolume(TopDet,Scint,"TopDetLV",0,0,0);

	TopArray = new G4AssemblyVolume();

	// Translation and rotation of plate inside assembly
	G4RotationMatrix Ra; 
	G4ThreeVector Ta;
	G4Transform3D Tr;

	// Rotation of assembly inside the world
	G4RotationMatrix Rm; 
	Rm.rotateX(0.0*deg);
	Rm.rotateY(0.0*deg);

	// Create the Plate with this loop
	for(unsigned int i=0; i<13; i++){
		Ta.setX(0.0*m);   Ta.setY(0.0*m);   Ta.setZ((0.60-.10*i)*m);
		Tr = G4Transform3D(Ra,Ta);
		TopArray->AddPlacedVolume( TopDetLV, Tr);
	};

	Rm.rotateZ(+45.0*deg);  
	// Place the Plates in the volume with this loop
	for(unsigned int i=0; i<2;i++){
		G4ThreeVector Tm(-0.615*m,(1.45+0.10*i)*m,(-1.10+1.30*i)*m);
		Tr = G4Transform3D(Rm,Tm);
		TopArray->MakeImprint( logicWorld, Tr);
	};

	Rm.rotateZ(-90.0*deg);  
	// Place the Plates in the volume with this loop
	for(unsigned int i=0; i<2;i++){
		G4ThreeVector Tm(+0.615*m,(1.45+0.10*i)*m,(-1.10+1.30*i)*m);
		Tr = G4Transform3D(Rm,Tm);
		TopArray->MakeImprint( logicWorld, Tr);
	};

	//----------------------------
	// Top dE detectors
	// Andrei's 2012 Plan B 160 cm long "taggers" in a horizontal orientation
	//----------------------------

	TopVeto = new G4Box("TopVeto",0.80*m,0.0050*m,0.0508*m);
	TopVetoLV = new G4LogicalVolume(TopVeto,Scint,"TopVetoLV",0,0,0);

	TopVetoArray = new G4AssemblyVolume();

	// Create the Plate with this loop
	for(unsigned int i=0; i<13; i++){
		Ta.setX(0.0*m);   Ta.setY(0.0*m);   Ta.setZ((0.60-.10*i)*m);
		Tr = G4Transform3D(Ra,Ta);
		TopVetoArray->AddPlacedVolume( TopVetoLV, Tr);
	};

	Rm.rotateZ(+45.0*deg);

	// Place the Plates in the volume with this loop
	for(unsigned int i=0; i<2;i++){
		G4ThreeVector Tm(0,(0.55+0.10*i)*m,(-1.10+1.30*i)*m);
		Tr = G4Transform3D(Rm,Tm);
		TopVetoArray->MakeImprint( logicWorld, Tr);
	};

	//------------------------------
	// Bottom E detectors
	// Andrei's 2012 Plan B setup using 160 cm long 10cm by 10cm detectors
	//------------------------------

	BottomDet = new G4Box("BottomDet",0.80*m,0.0508*m,0.0508*m);
	BottomDetLV = new G4LogicalVolume(BottomDet,Scint,"BottomDetLV",0,0,0);

	BottomArray = new G4AssemblyVolume();

	// Create the Plate with this loop
	for(unsigned int i=0; i<13; i++){
		Ta.setX(0.0*m);   Ta.setY(0.0*m);   Ta.setZ((0.60-.10*i)*m);
		Tr = G4Transform3D(Ra,Ta);
		BottomArray->AddPlacedVolume( BottomDetLV, Tr);
	};

	Rm.rotateZ(-45*deg);
	// Place the Plates in the volume with this loop
	for(unsigned int i=0; i<2;i++){
		G4ThreeVector Tm(-0.615*m,(-1.45-0.10*i)*m,(-1.10+1.30*i)*m);
		Tr = G4Transform3D(Rm,Tm);
		BottomArray->MakeImprint( logicWorld, Tr);
	};

	Rm.rotateZ(+90*deg);
	// Place the Plates in the volume with this loop
	for(unsigned int i=0; i<2;i++){
		G4ThreeVector Tm(+0.615*m,(-1.45-0.10*i)*m,(-1.10+1.30*i)*m);
		Tr = G4Transform3D(Rm,Tm);
		BottomArray->MakeImprint( logicWorld, Tr);
	};

	//-----------------------------
	// BOTTOMa dE detectors
	// Andrei's 2012 Plan B setup using 160 cm long "tagger" detectors 1cm thick
	//-----------------------------

	BottomVeto = new G4Box("BottomVeto",0.80*m,0.0050*m,0.0508*m);
	BottomVetoLV = new G4LogicalVolume(BottomVeto,Scint,"BottomVetoLV",0,0,0);

	BottomVetoArray = new G4AssemblyVolume();

	// Create the Plate with this loop
	for(unsigned int i=0; i<13; i++){
		Ta.setX(0.0*m);   Ta.setY(0.0*m);   Ta.setZ((0.60-.10*i)*m);
		Tr = G4Transform3D(Ra,Ta);
		BottomVetoArray->AddPlacedVolume( BottomVetoLV, Tr);
	};
	Rm.rotateZ(-45.0*deg);
	// Place the Plates in the volume with this loop
	for(unsigned int i=0; i<2;i++){
		G4ThreeVector Tm(0,(-0.55-0.10*i)*m,(-1.10+1.30*i)*m);
		Tr = G4Transform3D(Rm,Tm);
		BottomVetoArray->MakeImprint( logicWorld, Tr);
	};

	//-------------------------------
	// NEW Analyzer ---- NPOL front scintillator array (2/21/2012)
	// What is this? ... seriously! chambers? Trackers? Really?
	// Doesn't work well when different sections have different sizes
	// and differnt numbers of detectors.  I have to admit that the idea of
	// coding 140 individual detectors and 90+ vetos is not pleasant.  
	// Perhaps there is another way. --- update: 2/24/2012 ... there is
	// G4AssemblyVolume ... helper method.
	//-------------------------------

	// Front Array construction using G4AssemblyVolume helper routine

	FrontDet = new G4Box("FrontDet",0.50*m,0.0508*m,0.0508*m);
	FrontDetLV = new G4LogicalVolume(FrontDet,Scint,"FrontDetLV",0,0,0);

	FrontArray1 = new G4AssemblyVolume();
	FrontArray2 = new G4AssemblyVolume();

	// Front 2 sections have 6 detectors each
	for(unsigned int i=0; i<6; i++){
		Ta.setX(0.0*m);   Ta.setY((0.25-0.10*i)*m);   Ta.setZ(0.0*m);
		Tr = G4Transform3D(Ra,Ta);
		FrontArray1->AddPlacedVolume( FrontDetLV, Tr);
	};

	for(unsigned int i=0; i<2;i++){
		G4ThreeVector Tm(0,0,(-1.6992+.65*i)*m);
		Tr = G4Transform3D(Rm,Tm);
		FrontArray1->MakeImprint( logicWorld, Tr);
	};

	// Back 2 sections have 8 detectors each 

	for(unsigned int i=0; i<8; i++){
		Ta.setX(0.0*m);   Ta.setY((0.35-0.10*i)*m);   Ta.setZ(0.0*m);
		Tr = G4Transform3D(Ra,Ta);
		FrontArray2->AddPlacedVolume( FrontDetLV, Tr);
	};

	for(unsigned int i=2; i<4;i++){
		G4ThreeVector Tm(0,0,(-1.6992+.65*i)*m);
		Tr = G4Transform3D(Rm,Tm);
		FrontArray2->MakeImprint( logicWorld, Tr);
	};

	//---------------------------------------------------------------------
	// Taggers for analyzer layers
	// 1.0 cm by 10.0 cm by 100.0 cm
	//----------------------------------------------------------------------

	// Front taggers construction using G4AssemblyVolume helper routine

	FrontTag = new G4Box("FrontTag",0.50*m,0.0508*m,0.00508*m);
	FrontTagLV = new G4LogicalVolume(FrontTag,Scint,"FrontTagLV",0,0,0);

	FrontTagger1 = new G4AssemblyVolume();
	FrontTagger2 = new G4AssemblyVolume();

	// Front 2 sections of taggers have 6 detectors each
	for(unsigned int i=0; i<6; i++){
		Ta.setX(0.0*m);   Ta.setY((0.25-0.10*i)*m);   Ta.setZ(0.0*m);
		Tr = G4Transform3D(Ra,Ta);
		FrontTagger1->AddPlacedVolume( FrontTagLV, Tr);
	};

	for(unsigned int i=0; i<2;i++){
		G4ThreeVector Tm(0,0,(-1.7692+.65*i)*m);
		Tr = G4Transform3D(Rm,Tm);
		FrontTagger1->MakeImprint( logicWorld, Tr);
	};

	// Back 2 sections of taggers have 8 detectors each 

	for(unsigned int i=0; i<8; i++){
		Ta.setX(0.0*m);   Ta.setY((0.35-0.10*i)*m);   Ta.setZ(0.0*m);
		Tr = G4Transform3D(Ra,Ta);
		FrontTagger2->AddPlacedVolume( FrontTagLV, Tr);
	};

	for(unsigned int i=2; i<4;i++){
		G4ThreeVector Tm(0,0,(-1.7692+.65*i)*m);
		Tr = G4Transform3D(Rm,Tm);
		FrontTagger2->MakeImprint( logicWorld, Tr);
	};

	//--------- Visualization attributes -------------------------------

	G4VisAttributes* WorldVisAtt= new G4VisAttributes(0);
	logicWorld->SetVisAttributes(WorldVisAtt);

	G4VisAttributes* FrontDetVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,0.0));
	FrontDetLV->SetVisAttributes(FrontDetVisAtt);

	G4VisAttributes* TopBotVisAtt= new G4VisAttributes(G4Colour(1.0,0.0,1.0));
	TopDetLV->SetVisAttributes(TopBotVisAtt);
	BottomDetLV->SetVisAttributes(TopBotVisAtt);

	G4VisAttributes* TopBotaVisAtt= new G4VisAttributes(G4Colour(0.0,1.0,1.0));
	TopVetoLV->SetVisAttributes(TopBotaVisAtt);
	BottomVetoLV->SetVisAttributes(TopBotaVisAtt);

	return physiWorld;
}

void NpolDetectorConstruction::ConstructSDandField() {

	NpolSensitiveDetector *TopDetSD, *TopVetoSD, *BottomDetSD, *BottomVetoSD, *FrontDetSD, *FrontTagSD;

	TopDetSD = new NpolSensitiveDetector("TopDet");
	SetSensitiveDetector(TopDetLV,TopDetSD);

	TopVetoSD = new NpolSensitiveDetector("TopVeto");
	SetSensitiveDetector(TopVetoLV,TopVetoSD);

	BottomDetSD = new NpolSensitiveDetector("BottomDet");
	SetSensitiveDetector(BottomDetLV,BottomDetSD);

	BottomVetoSD = new NpolSensitiveDetector("BottomVeto");
	SetSensitiveDetector(BottomVetoLV,BottomVetoSD);

	FrontDetSD = new NpolSensitiveDetector("FrontDet");
	SetSensitiveDetector(FrontDetLV,FrontDetSD);

	FrontTagSD = new NpolSensitiveDetector("FrontTag");
	SetSensitiveDetector(FrontTagLV,FrontTagSD);
}

