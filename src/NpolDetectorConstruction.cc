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
#include "G4SDManager.hh"
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
  
  //Air
  G4Element* N = new G4Element("Nitrogen", "N", 7., 14.01*g/mole);
  G4Element* O = new G4Element("Oxygen"  , "O", 8., 16.00*g/mole);
  
  G4Material* Air = new G4Material("Air", 1.29*mg/cm3, 2);
  Air->AddElement(N, 70*perCent);
  Air->AddElement(O, 30*perCent);

  //G4Material* Vac= new G4Material("Vacuum", 1.0, 1.01*g/mole, universe_mean_density,kStateGas, 2.73*kelvin, 3.e-18*pascal);
  
  
  // Print all the materials defined.
  //
  G4cout << G4endl << "The materials defined are : " << G4endl << G4endl;
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
  
  //--------- Definitions of Solids, Logical Volumes, Physical Volumes ---------
  
  //------------------------------ 
  // World
  //------------------------------ 
  
  G4Box *solidWorld= new G4Box("World",2.50*m, 3.0*m, 10.0*m);
  G4LogicalVolume *logicWorld= new G4LogicalVolume( solidWorld, Air, "World", 0, 0, 0);
  
  //  Must place the World Physical volume unrotated at (0,0,0).
  // 
  G4VPhysicalVolume *physiWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World", 0, false, 0);

  //---------------------------
  // TOP/BOTTOM are the E arrays for proton total energy
  // TOPa/BOTTOMa are the dE arrays for particle ID (protons versus pions)
  //---------------------------
  
  G4VSolid *TopDet, *TopVeto, *BottomDet, *BottomVeto, *FrontDet, *FrontTag;
  G4AssemblyVolume *TopArray1, *TopArray2, *TopVetoArray1, *TopVetoArray2, *BottomArray1, *BottomArray2, *BottomVetoArray1, *BottomVetoArray2, *FrontArray1, *FrontArray2, *FrontTagger1, *FrontTagger2;
  
  //-------------------------------
  // NEW Analyzer ---- NPOL front scintillator array (2/21/2012)
  // update: 2/24/2012 G4AssemblyVolume ... helper method.
  //-------------------------------
  // Top E detectors
  // Andrei's 2012 Plan B setup using 160 cm long detectors 10cm by 10cm
  //---------------------------
  
  TopDet = new G4Box("TopDet",0.80*m,0.0508*m,0.0508*m);
  TopDetLV = new G4LogicalVolume(TopDet,Scint,"TopDetLV",0,0,0);
  
  TopArray1 = new G4AssemblyVolume();
  TopArray2 = new G4AssemblyVolume();

  // Translation and rotation of plate inside assembly
  G4RotationMatrix Ra; 
  G4ThreeVector Ta, Tm;
  G4Transform3D Tr;

  
  // Rotation of assembly inside the world
  G4RotationMatrix Rm; 
  Rm.rotateX(0.0*deg);
  Rm.rotateY(0.0*deg);
  
  // Create the Top, Front E array (called TopArray1) Plate with this loop 
  // with 13 detectors
  for(unsigned int i=0; i<13; i++){
    Ta.setX(0.0*m);   Ta.setY(0.0*m);   Ta.setZ((0.60-.10*i)*m);
    Tr = G4Transform3D(Ra,Ta);
    TopArray1->AddPlacedVolume( TopDetLV, Tr);
  };
  
  Rm.rotateZ(-45.0*deg);  
  // Place the Plate on the left side of the package
  Tm.setX(+0.615*m); Tm.setY(0.70*m); Tm.setZ(-1.10*m);
  Tr = G4Transform3D(Rm,Tm);
  TopArray1->MakeImprint( logicWorld, Tr);
  
  Rm.rotateZ(+90.0*deg);  
  // Place the Plates on the right side of the package
  Tm.setX(-0.615*m); Tm.setY(0.70*m); Tm.setZ(-1.10*m);
  Tr = G4Transform3D(Rm,Tm);
  TopArray1->MakeImprint( logicWorld, Tr);
  
  // Create the Top, Back E array (called TopArray2) Plate with this loop 
  // with 14 detectors
  for(unsigned int i=0; i<14; i++){
    Ta.setX(0.0*m);   Ta.setY(0.0*m);   Ta.setZ((0.60-.10*i)*m);
    Tr = G4Transform3D(Ra,Ta);
    TopArray2->AddPlacedVolume( TopDetLV, Tr);
  };

  // Place the Plate on the left side of the package
  Rm.rotateZ(-90.0*deg);
  Tm.setX(+0.615*m); Tm.setY(0.80*m); Tm.setZ(+0.30*m);
  Tr = G4Transform3D(Rm,Tm);
  TopArray2->MakeImprint( logicWorld, Tr);
  
  Rm.rotateZ(+90.0*deg);  
  // Place the Plates on the right side of the package
  Tm.setX(-0.615*m); Tm.setY(0.80*m); Tm.setZ(+0.30*m);
  Tr = G4Transform3D(Rm,Tm);
  TopArray2->MakeImprint( logicWorld, Tr);


  //----------------------------
  // Top dE detectors
  // Andrei's 2012 Plan B 160 cm long "taggers" in a horizontal orientation
  //----------------------------
  
  TopVeto = new G4Box("TopVeto",0.80*m,0.0050*m,0.0508*m);
  TopVetoLV = new G4LogicalVolume(TopVeto,Scint,"TopVetoLV",0,0,0);
  
  TopVetoArray1 = new G4AssemblyVolume();
  TopVetoArray2 = new G4AssemblyVolume();

  // Create the Plate for TopVetoArray1 with this loop (13 Detectors)
  for(unsigned int i=0; i<13; i++){
    Ta.setX(0.0*m);   Ta.setY(0.0*m);   Ta.setZ((0.60-.10*i)*m);
    Tr = G4Transform3D(Ra,Ta);
    TopVetoArray1->AddPlacedVolume( TopVetoLV, Tr);
  };

  // Create the Plate for TopVetoArray2 with this loop (14 Detectors)
  for(unsigned int i=0; i<14; i++){
    Ta.setX(0.0*m);   Ta.setY(0.0*m);   Ta.setZ((0.60-.10*i)*m);
    Tr = G4Transform3D(Ra,Ta);
    TopVetoArray2->AddPlacedVolume( TopVetoLV, Tr);
  };
  
  Rm.rotateZ(-45.0*deg);
  
  // Place the Plates in the volume
  Tm.setX(0); Tm.setY(0.32*m); Tm.setZ(-1.15*m);
  Tr = G4Transform3D(Rm,Tm);
  TopVetoArray1->MakeImprint( logicWorld, Tr);
  
  Tm.setX(0); Tm.setY(0.42*m); Tm.setZ(+0.30*m);
  Tr = G4Transform3D(Rm,Tm);
  TopVetoArray2->MakeImprint( logicWorld, Tr);
  
  //------------------------------
  // Bottom E detectors
  // Andrei's 2012 Plan B setup using 160 cm long 10cm by 10cm detectors
  //------------------------------
  
  BottomDet = new G4Box("BottomDet",0.80*m,0.0508*m,0.0508*m);
  BottomDetLV = new G4LogicalVolume(BottomDet,Scint,"BottomDetLV",0,0,0);
  
  BottomArray1 = new G4AssemblyVolume();
  BottomArray2 = new G4AssemblyVolume();

// Create the Bottom, Front E array (called BottomArray1) Plate with this loop 
  // with 13 detectors
  for(unsigned int i=0; i<13; i++){
    Ta.setX(0.0*m);   Ta.setY(0.0*m);   Ta.setZ((0.60-.10*i)*m);
    Tr = G4Transform3D(Ra,Ta);
    BottomArray1->AddPlacedVolume( BottomDetLV, Tr);
  };
  
  Rm.rotateZ(+45.0*deg);  
  // Place the Plate on the left side of the package
  Tm.setX(+0.615*m); Tm.setY(-0.70*m); Tm.setZ(-1.10*m);
  Tr = G4Transform3D(Rm,Tm);
  BottomArray1->MakeImprint( logicWorld, Tr);
  
  Rm.rotateZ(+90.0*deg);  
  // Place the Plates on the right side of the package
  Tm.setX(-0.615*m); Tm.setY(-0.70*m); Tm.setZ(-1.10*m);
  Tr = G4Transform3D(Rm,Tm);
  BottomArray1->MakeImprint( logicWorld, Tr);
  
  // Create the Top, Back E array (called TopArray2) Plate with this loop 
  // with 14 detectors
  for(unsigned int i=0; i<14; i++){
    Ta.setX(0.0*m);   Ta.setY(0.0*m);   Ta.setZ((0.60-.10*i)*m);
    Tr = G4Transform3D(Ra,Ta);
    BottomArray2->AddPlacedVolume( BottomDetLV, Tr);
  };

  // Place the Plate on the left side of the package
  Rm.rotateZ(-90.0*deg);
  Tm.setX(+0.615*m); Tm.setY(-0.80*m); Tm.setZ(+0.30*m);
  Tr = G4Transform3D(Rm,Tm);
  BottomArray2->MakeImprint( logicWorld, Tr);
  
  Rm.rotateZ(+90.0*deg);  
  // Place the Plates on the right side of the package
  Tm.setX(-0.615*m); Tm.setY(-0.80*m); Tm.setZ(+0.30*m);
  Tr = G4Transform3D(Rm,Tm);
  BottomArray2->MakeImprint( logicWorld, Tr);
  
  
  //-----------------------------
  // BOTTOMa dE detectors
  // Andrei's 2012 Plan B setup using 160 cm long "tagger" detectors 1cm thick
  //-----------------------------
  
  BottomVeto = new G4Box("BottomVeto",0.80*m,0.0050*m,0.0508*m);
  BottomVetoLV = new G4LogicalVolume(BottomVeto,Scint,"BottomVetoLV",0,0,0);
  
  BottomVetoArray1 = new G4AssemblyVolume();
  BottomVetoArray2 = new G4AssemblyVolume();

  // Create the Plate for the Bottom Front plate with this loop
  for(unsigned int i=0; i<13; i++){
    Ta.setX(0.0*m);   Ta.setY(0.0*m);   Ta.setZ((0.60-.10*i)*m);
    Tr = G4Transform3D(Ra,Ta);
    BottomVetoArray1->AddPlacedVolume( BottomVetoLV, Tr);
  };

 // Create the Plate for the Bottom back plate with this loop
  for(unsigned int i=0; i<13; i++){
    Ta.setX(0.0*m);   Ta.setY(0.0*m);   Ta.setZ((0.60-.10*i)*m);
    Tr = G4Transform3D(Ra,Ta);
    BottomVetoArray2->AddPlacedVolume( BottomVetoLV, Tr);
  };

  Rm.rotateZ(+45.0*deg);
  // Place the Plates in the volume
  Tm.setX(0); Tm.setY(-0.32*m); Tm.setZ(-1.15*m);
  Tr = G4Transform3D(Rm,Tm);
  BottomVetoArray1->MakeImprint( logicWorld, Tr);
  
  Tm.setX(0); Tm.setY(-0.42*m); Tm.setZ(+0.30*m);
  Tr = G4Transform3D(Rm,Tm);
  BottomVetoArray2->MakeImprint( logicWorld, Tr);

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
    Tm.setX(0); Tm.setY(0); Tm.setZ((-1.6992+.65*i)*m);
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
    Tm.setX(0); Tm.setY(0); Tm.setZ((-1.6992+.65*i)*m);
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
    Tm.setX(0); Tm.setY(0); Tm.setZ((-1.7692+.65*i)*m);
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
    Tm.setX(0); Tm.setY(0); Tm.setZ((-1.7692+.65*i)*m);
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
  //G4SDManager* SDman = G4SDManager::GetSDMpointer();

  TopDetSD = new NpolSensitiveDetector("TopDet");
  //SDman->AddNewDetector(TopDetSD);
  SetSensitiveDetector(TopDetLV,TopDetSD);


  TopVetoSD = new NpolSensitiveDetector("TopVeto");
  //SDman->AddNewDetector(TopVetoSD);
  SetSensitiveDetector(TopVetoLV,TopVetoSD);
  
  BottomDetSD = new NpolSensitiveDetector("BottomDet");
  //SDman->AddNewDetector(BottomDetSD);
  SetSensitiveDetector(BottomDetLV,BottomDetSD);
  
  BottomVetoSD = new NpolSensitiveDetector("BottomVeto");
  //SDman->AddNewDetector(BottomVetoSD);
  SetSensitiveDetector(BottomVetoLV,BottomVetoSD);
  
  FrontDetSD = new NpolSensitiveDetector("FrontDet");
  //SDman->AddNewDetector(FrontDetSD);
  SetSensitiveDetector(FrontDetLV,FrontDetSD);
  
  FrontTagSD = new NpolSensitiveDetector("FrontTag");
  //SDman->AddNewDetector(FrontTagSD);
  SetSensitiveDetector(FrontTagLV,FrontTagSD);
}

