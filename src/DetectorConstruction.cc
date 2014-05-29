//
// ********************************************************************
// *
// ********************************************************************
//
//
// $Id: DetectorConstruction.cc,v 1.21.4.1 2010/04/01 09:14:35 gcosmo Exp $
// GEANT4 tag $Name: geant4-09-03-patch-01 $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......d 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"
//#include "ChamberParameterisation.hh"
//#include "MagneticField.hh"
//#include "TrackerSD.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4SDManager.hh"
#include "G4GeometryTolerance.hh"
#include "G4GeometryManager.hh"
//#include "G4Assembly.hh"
#include "G4AssemblyTriplet.hh"
#include "G4AssemblyVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4UserLimits.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
DetectorConstruction::DetectorConstruction()
  : G4VUserDetectorConstruction()
//:solidWorld(0),  logicWorld(0),  physiWorld(0)
 {
   // detectorMessenger = new DetectorMessenger(this);
  }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
DetectorConstruction::~DetectorConstruction()
{
  //  delete fpMagField;
  //delete stepLimit;
  //delete chamberParam;
  //delete detectorMessenger;             
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
G4VPhysicalVolume* DetectorConstruction::Construct()
{
//--------- Material definition ---------

  G4double a, z;
//  G4double density, temperature, pressure;
  G4double density;
  G4int nel;

  //Air
  G4Element* N = new G4Element("Nitrogen", "N", z=7., a= 14.01 * g/mole);
  G4Element* O = new G4Element("Oxygen"  , "O", z=8., a= 16.00*g/mole);
   
  G4Material* Air = new G4Material("Air", density= 1.29*mg/cm3, nel=2);
  Air->AddElement(N, 70*perCent);
  Air->AddElement(O, 30*perCent);

// Scint for NPOL
  G4Element* H = new G4Element("Hydrogen", "H", z=1., a= 1.008*g/mole);
  G4Element* C = new G4Element("Carbon"  , "C", z=6., a= 12.011*g/mole);
   
  G4Material* Scint = new G4Material("Scint", density= 1.02*g/cm3, nel=2);
  Scint->AddElement(H, 8.451*perCent);
  Scint->AddElement(C, 91.549*perCent);

 G4Material* Vac= new G4Material("Vacuum", 1.0, 1.01*g/mole, universe_mean_density,kStateGas, 2.73*kelvin, 3.e-18*pascal);


  // Print all the materials defined.
  //
  G4cout << G4endl << "The materials defined are : " << G4endl << G4endl;
  // G4cout << *(G4Material::GetMaterialTable()) << G4endl;

//--------- Definitions of Solids, Logical Volumes, Physical Volumes ---------
  
  //------------------------------ 
  // World
  //------------------------------ 
  
  G4VSolid*  solidWorld= new G4Box("World",2.0*m,2.5*m, 2.5*m);
  G4LogicalVolume* logicWorld= new G4LogicalVolume( solidWorld, Vac, "World", 0, 0, 0);
  
  //  Must place the World Physical volume unrotated at (0,0,0).
  // 

  G4VPhysicalVolume* physiWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World", 0, false, 0);              

  //---------------------------
  // This is just stupid.  It must be cleaned up; if for nothing
  // but my sanity.  W.T. (2012)
  //
  // TOP/BOTTOM are the E arrays for proton total energy
  // TOPa/BOTTOMa are the dE arrays for particle ID (protons versus pions)
  //---------------------------

  //--------------------------
  // Top E detectors
  // Andrei's 2012 Plan B setup using 160 cm long detectors 10cm by 10cm
  //---------------------------


 G4VSolid* TopDet = new G4Box("TopDet",0.80*m,0.0508*m,0.0508*m);
 G4LogicalVolume* TopDetLV = new G4LogicalVolume(TopDet,Scint,"TopDet",0,0,0);
 G4AssemblyVolume* TopArray = new G4AssemblyVolume();
  
  // Translation and rotation of plate inside assembly
  G4RotationMatrix Ra; 
  G4ThreeVector Ta;
  G4ThreeVector Tm;
  G4Transform3D Tr;
  
  Ra.rotateX(0.0*deg);
  Ra.rotateY(0.0*deg);
  Ra.rotateZ(0.0*deg);

  // Rotation of assembly inside the world
  G4RotationMatrix Rm; 
  Rm.rotateX(0.0*deg);
  Rm.rotateY(0.0*deg);

  // Create the Plate with this loop (13 detectors)
  for(unsigned int i=0; i<13; i++){
    Ta.setX(0.0*m);   Ta.setY(0.0*m);   Ta.setZ((0.60-.10*i)*m);
    G4cout << "Ta = "<< G4endl;
    G4cout << Ta << G4endl;
    Tr = G4Transform3D(Ra,Ta);
    TopArray->AddPlacedVolume( TopDetLV, Tr);
  };

  Rm.rotateZ(+45.0*deg);  
  // Place the Plates in the volume with this loop
  for(unsigned int i=0; i<2;i++){
    G4ThreeVector Tm((-0.615*m),((1.45+0.10*i)*m),((-1.10+1.30*i)*m));
    //Tm.setX(-1*0.615*m); Tm.setY((1.45+0.10*i)*m); Tm.setZ((-1.10+1.50*i)*m);
    G4cout << "Tm = "<< G4endl;
    G4cout << Tm << G4endl;
    Tr = G4Transform3D(Rm,Tm);    
    TopArray->MakeImprint( logicWorld, Tr);
  };
  
  Rm.rotateZ(-90.0*deg);  
  // Place the Plates in the volume with this loop
  for(unsigned int i=0; i<2;i++){
    G4ThreeVector Tm(+0.615*m,(1.45+0.10*i)*m,(-1*1.10+1.30*i)*m);
    Tr = G4Transform3D(Rm,Tm);
    TopArray->MakeImprint( logicWorld, Tr);
  };

  //----------------------------
  // Top dE detectors
  // Andrei's 2012 Plan B 160 cm long "taggers" in a horizontal orientation
  //----------------------------

   G4Box* TopVeto = new G4Box("TopVeto",0.80*m,0.0050*m,0.0508*m);
   G4LogicalVolume* TopVetoLV = new G4LogicalVolume(TopVeto,Scint,"TopVeto",0,0,0);
   G4AssemblyVolume* TopVetoArray = new G4AssemblyVolume();

  // Create the Plate with this loop
  for(unsigned int i=0; i<13; i++){
    Ta.setX(0.0*m);   Ta.setY(0.0*m);   Ta.setZ((0.60-.10*i)*m);
    Tr = G4Transform3D(Ra,Ta);
    TopVetoArray->AddPlacedVolume( TopVetoLV, Tr);
  };
  
  Rm.rotateZ(+45.0*deg);

  // Place the Plates in the volume with this loop
  for(unsigned int i=0; i<2;i++){
    G4ThreeVector Tm(0,(0.55+0.10*i)*m,(-1*1.10+1.30*i)*m);
    Tr = G4Transform3D(Rm,Tm);
    TopVetoArray->MakeImprint( logicWorld, Tr);
  };

   //------------------------------
  // Bottom E detectors
  // Andrei's 2012 Plan B setup using 160 cm long 10cm by 10cm detectors
  //------------------------------
  
   G4Box* BottomDet = new G4Box("BottomDet",0.80*m,0.0508*m,0.0508*m);
   G4LogicalVolume* BottomDetLV = new G4LogicalVolume(BottomDet,Scint,"BottomDet",0,0,0);
   G4AssemblyVolume* BottomArray = new G4AssemblyVolume();

  // Create the Plate with this loop
   for(unsigned int i=0; i<13; i++){
    Ta.setX(0.0*m);   Ta.setY(0.0*m);   Ta.setZ((0.60-.10*i)*m);
    Tr = G4Transform3D(Ra,Ta);
    BottomArray->AddPlacedVolume( BottomDetLV, Tr);
  };
  
   Rm.rotateZ(-45*deg);
  // Place the Plates in the volume with this loop
  for(unsigned int i=0; i<2;i++){
    G4ThreeVector Tm(-1*0.615*m,(-1*1.45-0.10*i)*m,(-1*1.10+1.30*i)*m);
    Tr = G4Transform3D(Rm,Tm);
    BottomArray->MakeImprint( logicWorld, Tr);
  };

   Rm.rotateZ(+90*deg);
  // Place the Plates in the volume with this loop
  for(unsigned int i=0; i<2;i++){
    G4ThreeVector Tm(+0.615*m,(-1*1.45-0.10*i)*m,(-1*1.10+1.30*i)*m);
    Tr = G4Transform3D(Rm,Tm);
    BottomArray->MakeImprint( logicWorld, Tr);
  };
  
  //-----------------------------
  // BOTTOMa dE detectors
  // Andrei's 2012 Plan B setup using 160 cm long "tagger" detectors 1cm thick
  //-----------------------------

  G4Box* BottomVeto = new G4Box("BottomVeto",0.80*m,0.0050*m,0.0508*m);
  G4LogicalVolume* BottomVetoLV = new G4LogicalVolume(BottomVeto,Scint,"BottomVeto",0,0,0);
  G4AssemblyVolume* BottomVetoArray = new G4AssemblyVolume();
  
  // Create the Plate with this loop
  for(unsigned int i=0; i<13; i++){
    Ta.setX(0.0*m);   Ta.setY(0.0*m);   Ta.setZ((0.60-.10*i)*m);
    Tr = G4Transform3D(Ra,Ta);
    BottomVetoArray->AddPlacedVolume( BottomVetoLV, Tr);
  };
    Rm.rotateZ(-1*45.0*deg);
  // Place the Plates in the volume with this loop
  for(unsigned int i=0; i<2;i++){
    G4ThreeVector Tm(0,(-1*0.55-0.10*i)*m,(-1*1.10+1.30*i)*m);
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

  G4Box* FrontDet = new G4Box("FrontDet",0.50*m,0.0508*m,0.0508*m);
  G4LogicalVolume* FrontDetLV = new G4LogicalVolume(FrontDet,Scint,"FrontDet",0,0,0);
  G4AssemblyVolume* FrontArray1 = new G4AssemblyVolume();
  G4AssemblyVolume* FrontArray2 = new G4AssemblyVolume();
  //  G4AssemblyVolume* FrontArray3 = new G4AssemblyVolume();

  // Front 2 sections have 6 detectors each
  for(unsigned int i=0; i<6; i++){
    Ta.setX(0.0*m);   Ta.setY((0.25-0.10*i)*m);   Ta.setZ(0.0*m);
    Tr = G4Transform3D(Ra,Ta);
    FrontArray1->AddPlacedVolume( FrontDetLV, Tr);
  };
  
  for(unsigned int i=0; i<2;i++){
    G4ThreeVector Tm(0,0,(-1*1.6992+.65*i)*meter);
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
    G4ThreeVector Tm(0,0,(-1*1.6992+.65*i)*m);
    Tr = G4Transform3D(Rm,Tm);
    FrontArray2->MakeImprint( logicWorld, Tr);
  };

  //---------------------------------------------------------------------
  // Taggers for analyzer layers
  // 1.0 cm by 10.0 cm by 100.0 cm
  //----------------------------------------------------------------------

// Front taggers construction using G4AssemblyVolume helper routine

  G4Box* FrontTag = new G4Box("FrontTag",0.50*m,0.0508*m,0.00508*m);
  G4LogicalVolume* FrontTagLV = new G4LogicalVolume(FrontTag,Scint,"FrontTag",0,0,0);
  G4AssemblyVolume* FrontTagger1 = new G4AssemblyVolume();
  G4AssemblyVolume* FrontTagger2 = new G4AssemblyVolume();
  //  G4AssemblyVolume* FrontArray3 = new G4AssemblyVolume();

  // Front 2 sections of taggers have 6 detectors each
  for(unsigned int i=0; i<6; i++){
    Ta.setX(0.0*m);   Ta.setY((0.25-0.10*i)*m);   Ta.setZ(0.0*m);
    Tr = G4Transform3D(Ra,Ta);
    FrontTagger1->AddPlacedVolume( FrontTagLV, Tr);
  };
  
  for(unsigned int i=0; i<2;i++){
    G4ThreeVector Tm(0,0,(-1*1.7692+.65*i)*m);
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
    G4ThreeVector Tm(0,0,(-1*1.7692+.65*i)*m);
    Tr = G4Transform3D(Rm,Tm);
    FrontTagger2->MakeImprint( logicWorld, Tr);
    }; 


  // Back 2 sections have 10 detectors each
  // Update 10/24/2013: Dropped in Feb 2012 meeting; Plan B is the go

  /* 
    for(unsigned int i=0; i<10; i++){
    Ta.setX(0.0*m);   Ta.setY((0.45-0.10*i)*m);   Ta.setZ(0.0*m);
    Tr = G4Transform3D(Ra,Ta);
    FrontArray3->AddPlacedVolume( FrontDetLV, Tr);
  };
  
  for(unsigned int i=4; i<6;i++){
    G4ThreeVector Tm(0,0,(-1.6992+.65*i)*m);
    Tr = G4Transform3D(Rm,Tm);
    FrontArray3->MakeImprint( logicWorld, Tr);
    }; */


  //------------------------------------------------ 
  // Sensitive detectors
  //------------------------------------------------ 

  //  G4SDManager* SDman = G4SDManager::GetSDMpointer();

  //G4String trackerChamberSDname = "/TrackerChamberSD";
  //TrackerSD* aTrackerSD = new TrackerSD( trackerChamberSDname );
  //SDman->AddNewDetector( aTrackerSD );
  //logicChamber->SetSensitiveDetector( aTrackerSD ); 


//--------- Visualization attributes -------------------------------

  G4VisAttributes* BoxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  logicWorld  ->SetVisAttributes(BoxVisAtt);  
  G4VisAttributes* FrontDetVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,0.0));
  FrontDetLV  ->SetVisAttributes(FrontDetVisAtt);
  G4VisAttributes* TopBotVisAtt= new G4VisAttributes(G4Colour(1.0,0.0,1.0));
  TopDetLV->SetVisAttributes(TopBotVisAtt);
  BottomDetLV->SetVisAttributes(TopBotVisAtt);
  G4VisAttributes* TopBotaVisAtt= new G4VisAttributes(G4Colour(0.0,1.0,1.0));
  TopVetoLV->SetVisAttributes(TopBotaVisAtt);
  BottomVetoLV->SetVisAttributes(TopBotaVisAtt);
 
  
//--------- example of User Limits -------------------------------

  // below is an example of how to set tracking constraints in a given
  // logical volume(see also in N02PhysicsList how to setup the processes
  // G4StepLimiter or G4UserSpecialCuts).
    
  // Sets a max Step length in the tracker region, with G4StepLimiter
  //
  //  G4double maxStep = 0.5*ChamberWidth;
  //  stepLimit = new G4UserLimits(maxStep);
  //  logicTracker->SetUserLimits(stepLimit);
  
  // Set additional contraints on the track, with G4UserSpecialCuts
  //
  // G4double maxLength = 2*fTrackerLength, maxTime = 0.1*ns, minEkin = 10*MeV;
  // logicTracker->SetUserLimits(new G4UserLimits(maxStep,maxLength,maxTime,
  //                                               minEkin));
  
  return physiWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
/*void DetectorConstruction::setTargetMaterial(G4String materialName)
{
  // search the material by its name 
  G4Material* pttoMaterial = G4Material::GetMaterial(materialName);  
  if (pttoMaterial)
     {TargetMater = pttoMaterial;
      logicTarget->SetMaterial(pttoMaterial); 
      G4cout << "\n----> The target is " << fTargetLength/cm << " cm of "
             << materialName << G4endl;
     }             
     }*/
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/*void DetectorConstruction::setChamberMaterial(G4String materialName)
{
  // search the material by its name 
  G4Material* pttoMaterial = G4Material::GetMaterial(materialName);  
  if (pttoMaterial)
     {ChamberMater = pttoMaterial;
      logicChamber->SetMaterial(pttoMaterial); 
      G4cout << "\n----> The chambers are " << ChamberWidth/cm << " cm of "
             << materialName << G4endl;
     }             
     } */
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
/* void DetectorConstruction::SetMagField(G4double fieldValue)
 {
   // fpMagField->SetMagFieldValue(fieldValue);  
   }*/

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/*void DetectorConstruction::SetMaxStep(G4double maxStep)
{
  if ((stepLimit)&&(maxStep>0.)) stepLimit->SetMaxAllowedStep(maxStep);
  }*/

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
