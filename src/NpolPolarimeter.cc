//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************
//* The Geant4 software is used by the Northern Michigan University  *
//* in accordance to the Geant4 software license specified above.    *
//* The NMU Collaboration does not hold any rights to this software  *
//* and gives full permission for its use to others under the limits *
//* imposed by the GEANT4 Collaboration.  The NMU Collaboration      *
//* gives no express or implied warranty and use of our code is at   *
//* the users discretion only.  		    			*
//********************************************************************

#include "G4PhysicalConstants.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4AssemblyVolume.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4ios.hh"

#include "NpolMaterials.hh"
#include "NpolPolarimeter.hh"

NpolPolarimeter::NpolPolarimeter() {
	G4cout << "Initializing Polarimeter" << G4endl;
}

NpolPolarimeter::~NpolPolarimeter() {
	G4cout << "Deleting Polarimeter" << G4endl;
}

G4AssemblyVolume *MakePlate(G4LogicalVolume *detLV, G4int numDets, 
		G4double TmX, G4double TmY, G4double TmZ,
		G4double TmdX, G4double TmdY, G4double TmdZ) {

	G4AssemblyVolume *plate = new G4AssemblyVolume();

	// Translation and rotation of plate inside assembly
	G4RotationMatrix Ra; 
	G4ThreeVector Tm;
	G4Transform3D Tr;
	
	// Rotation of assembly inside the world 
	Ra.rotateX(0.0*deg);
	Ra.rotateY(0.0*deg);
	Ra.rotateZ(0.0*deg);

	int i;
	for(i=0; i<numDets; i++) {
		Tm.setX(TmX-TmdX*i); Tm.setY(TmY-TmdY*i); Tm.setZ(TmZ-TmdZ*i);
		Tr = G4Transform3D(Ra,Tm);
		plate->AddPlacedVolume(detLV,Tr);
	}

	return plate;
}

//---------------------------
// Top E detectors
// Andrei's 2012 Plan B setup using 160 cm long detectors 10cm by 10cm
//---------------------------
void NpolPolarimeter::ConstructTopDetArray(G4LogicalVolume *motherLV) {
	
	G4ThreeVector Tm;
	G4Transform3D Tr;

	G4VSolid *TopDet = new G4Box("TopDet",0.80*m,0.0508*m,0.0508*m);
	G4LogicalVolume *TopDetLV = new G4LogicalVolume(TopDet,NpolMaterials::GetInstance()->GetScint(),"TopDetLV",0,0,0);

	G4AssemblyVolume *TopArray1 = MakePlate(TopDetLV, 13, 0.0*m, 0.0*m, 0.60*m, 0.0*m, 0.0*m, 0.10*m);
	G4AssemblyVolume *TopArray2 = MakePlate(TopDetLV, 14, 0.0*m, 0.0*m, 0.60*m, 0.0*m, 0.0*m, 0.10*m);

	// Rotation of assembly inside the world
	G4RotationMatrix Rm; 
	Rm.rotateX(0.0*deg);
	Rm.rotateY(0.0*deg);
	Rm.rotateZ(-45.0*deg);

	// Place the Plate on the left side of the package
	Tm.setX(+0.615*m); Tm.setY(0.70*m); Tm.setZ(-1.10*m);
	//Tm.setX(-3.4694*m); Tm.setY(0.70*m); Tm.setZ(+6.582*m);
	Tr = G4Transform3D(Rm,Tm);
	TopArray1->MakeImprint(motherLV, Tr);

	Rm.rotateZ(+90.0*deg);  
	// Place the Plates on the right side of the package
	Tm.setX(-0.615*m); Tm.setY(0.70*m); Tm.setZ(-1.10*m);
	//Tm.setX(-3.4694*m); Tm.setY(0.70*m); Tm.setZ(+6.582*m);
	Tr = G4Transform3D(Rm,Tm);
	TopArray1->MakeImprint(motherLV, Tr);

	// Place the Plate on the left side of the package
	Rm.rotateZ(-90.0*deg);
	Tm.setX(+0.615*m); Tm.setY(0.80*m); Tm.setZ(+0.30*m);
	//Tm.setX(-4.6994*m); Tm.setY(0.70*m); Tm.setZ(+7.982*m);
	Tr = G4Transform3D(Rm,Tm);
	TopArray2->MakeImprint(motherLV, Tr);

	Rm.rotateZ(+90.0*deg);  
	// Place the Plates on the right side of the package
	Tm.setX(-0.615*m); Tm.setY(0.80*m); Tm.setZ(+0.30*m);
	Tr = G4Transform3D(Rm,Tm);
	TopArray2->MakeImprint(motherLV, Tr);
	
	G4VisAttributes *TopVisAtt= new G4VisAttributes(G4Colour(1.0,0.0,1.0));
	TopDetLV->SetVisAttributes(TopVisAtt);
}

//----------------------------
// Top dE detectors
// Andrei's 2012 Plan B 160 cm long "taggers" in a horizontal orientation
//----------------------------
void NpolPolarimeter::ConstructTopVetoArray(G4LogicalVolume *motherLV) {

	G4ThreeVector Tm;
	G4Transform3D Tr;

	G4VSolid *TopVeto = new G4Box("TopVeto",0.80*m,0.0050*m,0.0508*m);
	G4LogicalVolume *TopVetoLV = new G4LogicalVolume(TopVeto,NpolMaterials::GetInstance()->GetScint(),"TopVetoLV",0,0,0);

	G4AssemblyVolume *TopVetoArray1 = MakePlate(TopVetoLV, 13, 0.0*m, 0.0*m, 0.60*m, 0.0*m, 0.0*m, 0.10*m);
	G4AssemblyVolume *TopVetoArray2 = MakePlate(TopVetoLV, 14, 0.0*m, 0.0*m, 0.60*m, 0.0*m, 0.0*m, 0.10*m);

	// Rotation of assembly inside the world
	G4RotationMatrix Rm; 
	Rm.rotateX(0.0*deg);
	Rm.rotateY(0.0*deg);
	Rm.rotateZ(0.0*deg);

	// Place the Plates in the volume
	Tm.setX(0); Tm.setY(0.32*m); Tm.setZ(-1.15*m);
	Tr = G4Transform3D(Rm,Tm);
	TopVetoArray1->MakeImprint(motherLV,Tr);

	Tm.setX(0); Tm.setY(0.42*m); Tm.setZ(+0.30*m);
	Tr = G4Transform3D(Rm,Tm);
	TopVetoArray2->MakeImprint(motherLV,Tr);
	
	G4VisAttributes* TopaVisAtt= new G4VisAttributes(G4Colour(0.0,1.0,1.0));
	TopVetoLV->SetVisAttributes(TopaVisAtt);
}


//------------------------------
// Bottom E detectors
// Andrei's 2012 Plan B setup using 160 cm long 10cm by 10cm detectors
//------------------------------
void NpolPolarimeter::ConstructBottomDetArray(G4LogicalVolume *motherLV) {

	G4ThreeVector Tm;
	G4Transform3D Tr;

	G4VSolid *BottomDet = new G4Box("BottomDet",0.80*m,0.0508*m,0.0508*m);
	G4LogicalVolume *BottomDetLV = new G4LogicalVolume(BottomDet,NpolMaterials::GetInstance()->GetScint(),"BottomDetLV",0,0,0);

	G4AssemblyVolume *BottomDetArray1 = MakePlate(BottomDetLV, 13, 0.0*m, 0.0*m, 0.60*m, 0.0*m, 0.0*m, 0.10*m);
	G4AssemblyVolume *BottomDetArray2 = MakePlate(BottomDetLV, 14, 0.0*m, 0.0*m, 0.60*m, 0.0*m, 0.0*m, 0.10*m);

	// Rotation of assembly inside the world
	G4RotationMatrix Rm; 
	Rm.rotateX(0.0*deg);
	Rm.rotateY(0.0*deg);
	Rm.rotateZ(+45.0*deg);

	// Place the Plate on the left side of the package
	Tm.setX(+0.615*m); Tm.setY(-0.70*m); Tm.setZ(-1.10*m);
	Tr = G4Transform3D(Rm,Tm);
	BottomDetArray1->MakeImprint(motherLV,Tr);

	Rm.rotateZ(+90.0*deg);  
	// Place the Plates on the right side of the package
	Tm.setX(-0.615*m); Tm.setY(-0.70*m); Tm.setZ(-1.10*m);
	Tr = G4Transform3D(Rm,Tm);
	BottomDetArray1->MakeImprint(motherLV,Tr);

	// Place the Plate on the left side of the package
	Rm.rotateZ(-90.0*deg);
	Tm.setX(+0.615*m); Tm.setY(-0.80*m); Tm.setZ(+0.30*m);
	Tr = G4Transform3D(Rm,Tm);
	BottomDetArray2->MakeImprint(motherLV,Tr);

	Rm.rotateZ(+90.0*deg);  
	// Place the Plates on the right side of the package
	Tm.setX(-0.615*m); Tm.setY(-0.80*m); Tm.setZ(+0.30*m);
	Tr = G4Transform3D(Rm,Tm);
	BottomDetArray2->MakeImprint(motherLV,Tr);

	G4VisAttributes *BotVisAtt= new G4VisAttributes(G4Colour(1.0,0.0,1.0));
	BottomDetLV->SetVisAttributes(BotVisAtt);
}

//-----------------------------
// BOTTOMa dE detectors
// Andrei's 2012 Plan B setup using 160 cm long "tagger" detectors 1cm thick
//-----------------------------
void NpolPolarimeter::ConstructBottomVetoArray(G4LogicalVolume *motherLV) {

	G4ThreeVector Tm;
	G4Transform3D Tr;

	G4VSolid *BottomVeto = new G4Box("BottomVeto",0.80*m,0.0050*m,0.0508*m);
	G4LogicalVolume *BottomVetoLV = new G4LogicalVolume(BottomVeto,NpolMaterials::GetInstance()->GetScint(),"BottomVetoLV",0,0,0);

	G4AssemblyVolume *BottomVetoArray1 = MakePlate(BottomVetoLV, 13, 0.0*m, 0.0*m, 0.60*m, 0.0*m, 0.0*m, 0.10*m);
	G4AssemblyVolume *BottomVetoArray2 = MakePlate(BottomVetoLV, 13, 0.0*m, 0.0*m, 0.60*m, 0.0*m, 0.0*m, 0.10*m);

	// Rotation of assembly inside the world
	G4RotationMatrix Rm; 
	Rm.rotateX(0.0*deg);
	Rm.rotateY(0.0*deg);
	Rm.rotateZ(0.0*deg);

	// Place the Plates in the volume
	Tm.setX(0); Tm.setY(-0.32*m); Tm.setZ(-1.15*m);
	Tr = G4Transform3D(Rm,Tm);
	BottomVetoArray1->MakeImprint(motherLV,Tr);

	Tm.setX(0); Tm.setY(-0.42*m); Tm.setZ(+0.30*m);
	Tr = G4Transform3D(Rm,Tm);
	BottomVetoArray2->MakeImprint(motherLV,Tr);
	
	G4VisAttributes* BotaVisAtt= new G4VisAttributes(G4Colour(0.0,1.0,1.0));
	BottomVetoLV->SetVisAttributes(BotaVisAtt);
}

void NpolPolarimeter::ConstructFrontDetArray(G4LogicalVolume *motherLV) {
	
	G4ThreeVector Tm;
	G4Transform3D Tr;

	G4VSolid *FrontDet = new G4Box("FrontDet",0.50*m,0.0508*m,0.0508*m);
	G4LogicalVolume *FrontDetLV = new G4LogicalVolume(FrontDet,NpolMaterials::GetInstance()->GetScint(),"FrontDetLV",0,0,0);

	G4AssemblyVolume *FrontDetArray1 = MakePlate(FrontDetLV, 6, 0.0*m, 0.25*m, 0.0*m, 0.0*m, 0.10*m, 0.0*m);
	G4AssemblyVolume *FrontDetArray2 = MakePlate(FrontDetLV, 8, 0.0*m, 0.35*m, 0.0*m, 0.0*m, 0.10*m, 0.0*m);

	// Rotation of assembly inside the world
	G4RotationMatrix Rm; 
	Rm.rotateX(0.0*deg);
	Rm.rotateY(0.0*deg);

	for(unsigned int i=0; i<2;i++){
		Tm.setX(0); Tm.setY(0); Tm.setZ((-1.6992+.65*i)*m);
		Tr = G4Transform3D(Rm,Tm);
		FrontDetArray1->MakeImprint(motherLV,Tr);
	};

	for(unsigned int i=2; i<4;i++){
		Tm.setX(0); Tm.setY(0); Tm.setZ((-1.6992+.65*i)*m);
		Tr = G4Transform3D(Rm,Tm);
		FrontDetArray2->MakeImprint(motherLV,Tr);
	};
	
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
	G4LogicalVolume *FrontTagLV = new G4LogicalVolume(FrontTag,NpolMaterials::GetInstance()->GetScint(),"FrontTagLV",0,0,0);

	G4AssemblyVolume *FrontTaggerArray1 = MakePlate(FrontTagLV, 6, 0.0*m, 0.25*m, 0.0*m, 0.0*m, 0.10*m, 0.0*m);
	G4AssemblyVolume *FrontTaggerArray2 = MakePlate(FrontTagLV, 8, 0.0*m, 0.35*m, 0.0*m, 0.0*m, 0.10*m, 0.0*m);

	// Rotation of assembly inside the world
	G4RotationMatrix Rm; 
	Rm.rotateX(0.0*deg);
	Rm.rotateY(0.0*deg);

	for(unsigned int i=0; i<2;i++){
		Tm.setX(0); Tm.setY(0); Tm.setZ((-1.7692+.65*i)*m);
		Tr = G4Transform3D(Rm,Tm);
		FrontTaggerArray1->MakeImprint(motherLV,Tr);
	};

	for(unsigned int i=2; i<4;i++){
		Tm.setX(0); Tm.setY(0); Tm.setZ((-1.7692+.65*i)*m);
		Tr = G4Transform3D(Rm,Tm);
		FrontTaggerArray2->MakeImprint(motherLV,Tr);
	};
}

void NpolPolarimeter::Construct(G4LogicalVolume *motherLV) {

	ConstructTopDetArray(motherLV);
	ConstructTopVetoArray(motherLV);
	ConstructBottomDetArray(motherLV);
	ConstructBottomVetoArray(motherLV);
	ConstructFrontDetArray(motherLV);
	ConstructFrontTagArray(motherLV);

}

