//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************

// %% NpolBeamlineDown.cc %%

// Beam line construction file
// Created: William Tireman - December 2014
// Modified: Daniel Wilbern - December 2014
// Updated: W. Tireman - Januray 2015

#include "G4PhysicalConstants.hh"
#include "G4Material.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4LogicalVolume.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4ios.hh"
#include "G4TwoVector.hh"
#include "G4ExtrudedSolid.hh"
#include "G4UnionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4ThreeVector.hh"

#include "NpolMaterials.hh"
#include "NpolScatteringChamber.hh"
#include "NpolHallShell.hh"
#include "NpolBeamlineDown.hh"

G4double NpolBeamlineDown::Flange1Thick = 1.397*cm; 
G4double NpolBeamlineDown::Flange2Thick = 2*Flange1Thick; 
G4double NpolBeamlineDown::Flange3Thick = 2.5*Flange1Thick;
G4double NpolBeamlineDown::BellowLen = 8.3515*cm - 2*Flange1Thick; 
G4double NpolBeamlineDown::BellowOutRadius = 2.8575*cm;
G4double NpolBeamlineDown::SecA1InRadius = 2.067*cm;
G4double NpolBeamlineDown::SecA1OutRadius = 2.375*cm;
G4double NpolBeamlineDown::SecA2InRadius = 4.506*cm;
G4double NpolBeamlineDown::SecA2OutRadius = 5.080*cm;
G4double NpolBeamlineDown::SecA3InRadius = 7.703*cm;
G4double NpolBeamlineDown::SecA3OutRadius = 8.414*cm;
G4double NpolBeamlineDown::SecA1zLen = 20.91*cm + BellowLen + 2*Flange1Thick;
G4double NpolBeamlineDown::SecA2zLen = 420.00*cm;
G4double NpolBeamlineDown::SecA3zLen = 550.00*cm - SecA1zLen - SecA2zLen;
G4double NpolBeamlineDown::Flange1Radius = 4.280*cm;
G4double NpolBeamlineDown::Flange2Radius = SecA2OutRadius + 1.905*cm;
G4double NpolBeamlineDown::Flange3Radius = SecA3OutRadius + 1.905*cm;
G4double NpolBeamlineDown::GateX = 9.246*cm;
G4double NpolBeamlineDown::GateY = 18.275*cm;
G4double NpolBeamlineDown::GateZ = 2.9210*cm;

G4double NpolBeamlineDown::downLen = NpolBeamlineDown::calculateDownBeamLineLen();

NpolBeamlineDown::NpolBeamlineDown() {
  ConstructBeamlineDown();
  ConstructBeamlineDownInner();
  ConstructBeamlineSecA();
  ConstructSecA1Inner();
  ConstructSecA2Inner();
  ConstructSecA3Inner();
  ConstructBeamlineCap();
}

NpolBeamlineDown::~NpolBeamlineDown() {}

G4String NpolBeamlineDown::GetName() {
	return G4String("Down Beamline");
}

// This downstream beamline is a extruded solid consisting of 60 facets for
// circular portion constructed in polygon and two sections in z-axis. 
//  First section is a fixed length and diameter while the section section 
// is scaled by using the scaling in zsections for the last section.  This 
// yields a tappered section that can be adjusted later.
void NpolBeamlineDown::ConstructBeamlineDown(){
  G4double dia = SecA3OutRadius, zlen2 = downLen, tScale = 5.0;
  G4double zlen1 = 2.50*m;
G4int gnum = 60; // # of facets to circle for polygon
  
  std::vector<G4TwoVector> polygon(gnum);
  for (int i = 0; i< gnum; i++) {
    polygon[i] = G4TwoVector(dia*cos(360.0*deg/gnum*i),dia*sin(360.0*deg/gnum*i));
  }
  
  std::vector<G4ExtrudedSolid::ZSection> zsections;
  zsections.push_back(G4ExtrudedSolid::ZSection(0.0*m,
	G4TwoVector(0.0*cm,0.0*cm),1.0));
  zsections.push_back(G4ExtrudedSolid::ZSection(zlen1,
	G4TwoVector(0*cm,0*cm),1.0));
  zsections.push_back(G4ExtrudedSolid::ZSection(zlen2,
	G4TwoVector(0*cm,0*cm),tScale));
  
  G4ExtrudedSolid *BeamlineDown = new G4ExtrudedSolid("BeamlineDown",polygon,
	zsections); 
  BeamlineDownLV = new G4LogicalVolume(BeamlineDown,NpolMaterials::GetInstance()
       ->GetMaterial("SSteel"), "BeamlineDownLV", 0,0,0);
  G4VisAttributes *BeamlineVisAtt= new G4VisAttributes(G4Colour(0.3,0.7,0.2));
  BeamlineDownLV->SetVisAttributes(BeamlineVisAtt);
}

// Downstream beamline "cap".  This volume is just to locate electrons that
// have made it from the target to the end of the downstream beamline and we
// will kill them in stepping action.  No need to transport after they get 
// past the ugly parts.
void NpolBeamlineDown::ConstructBeamlineCap(){

  G4double Radius = SecA3OutRadius * 5.0;
  G4double Thickness = 0.50*cm;

  G4Tubs *Cap = new G4Tubs("Cap", 0.0*cm, Radius, Thickness/2, 0.0*deg, 360.0*deg);

  CapLV = new G4LogicalVolume(Cap, NpolMaterials::GetInstance()->GetMaterial("HardVacuum"),"CapLV", 0,0,0);

  G4VisAttributes *CapVisAtt= new G4VisAttributes(G4Colour(0.80,0.2,0.5));
  CapLV->SetVisAttributes(CapVisAtt);  
}


// This downstream inner part of the beam line is just a vacuum filler for 
// the downstream portion made in the previous method. 
void NpolBeamlineDown::ConstructBeamlineDownInner(){
  G4double dia = SecA3InRadius, zlen1 = +2.50*m, zlen2 = downLen, tScale = 4.0;
  G4int gnum =60;  // # of facets to cirle for polygon
  std::vector<G4TwoVector> polygon(gnum);
  for (int i = 0; i < gnum; i++) {
    polygon[i] = G4TwoVector(dia*cos(360.0*deg/gnum*i), dia*sin(360.0*deg/gnum*i));
  }
  
  std::vector<G4ExtrudedSolid::ZSection> zsections;
  zsections.push_back(G4ExtrudedSolid::ZSection(0.0*m,
	G4TwoVector(0*cm,0*cm),1.0));
  zsections.push_back(G4ExtrudedSolid::ZSection(zlen1,
	G4TwoVector(0*cm,0*cm),1.0));
  zsections.push_back(G4ExtrudedSolid::ZSection(zlen2,
	G4TwoVector(0*cm,0*cm),tScale));
 
  G4ExtrudedSolid *BeamlineDownInner = new G4ExtrudedSolid("BeamlineDownInner",    polygon,zsections); 
  BeamlineDownInnerLV = new G4LogicalVolume(BeamlineDownInner,
    NpolMaterials::GetInstance()->GetMaterial("HardVacuum"), "BeamlineDownInnerLV", 0,0,0);
  G4VisAttributes *BeamlineVisAtt= new G4VisAttributes(G4Colour(100,0.0,1.0));
  BeamlineDownInnerLV->SetVisAttributes(BeamlineVisAtt);  
}

// Construct Section A of the downstream beam line.  Section A is immediately 
// after the scattering chamber and contains MDC Gate Valve, support adjuster,
// bellows and a bunch of flanges.
void NpolBeamlineDown::ConstructBeamlineSecA(){
 
  G4double GateVertShift = 4.2863*cm, A2OffSet = 15.0*cm;
  G4ThreeVector translation = G4ThreeVector(0,0,0);
  G4RotationMatrix Rot;
  Rot.rotateX(0*deg); Rot.rotateY(0*deg); Rot.rotateZ(0*deg);
  G4Transform3D transform;

  G4Tubs *SecA1 = new G4Tubs("SecA1",SecA1InRadius, SecA1OutRadius, SecA1zLen/2+0.2*cm, 0*deg, 360*deg);
  G4Tubs *SecA2 = new G4Tubs("SecA2",SecA2InRadius, SecA2OutRadius, SecA2zLen/2+0.2*cm, 0*deg, 360*deg);
  G4Tubs *SecA3 = new G4Tubs("SecA3",SecA3InRadius, SecA3OutRadius, SecA3zLen/2+0.2*cm, 0*deg, 360*deg);
  G4Tubs *Flange1 = new G4Tubs("Flange1", SecA1InRadius+0.1*cm, Flange1Radius, Flange1Thick/2, 0*deg, 360*deg);
  G4Tubs *Flange2 = new G4Tubs("Flange2", SecA2InRadius+0.1*cm, Flange2Radius, Flange2Thick/2, 0*deg, 360*deg);
  G4Tubs *Flange3 = new G4Tubs("Flange3", SecA3InRadius+0.2*cm, Flange3Radius, Flange3Thick/2, 0*deg, 360*deg);
  G4Tubs *Bellow = new G4Tubs("Bellow", SecA1InRadius+0.1*cm, BellowOutRadius, BellowLen/2, 0*deg, 360*deg);
  G4Box *Gate = new G4Box("Gate", GateX/2, GateY/2, GateZ/2);
  G4Tubs *GateHole = new G4Tubs("GateHole", 0*cm, SecA1InRadius+0.1*cm, GateZ/2+1.0*cm, 0*deg, 360*deg);
  G4Box *A2Support = new G4Box("A2Support", 7.0*cm, 7.0*cm, 8.0*cm);
  G4Tubs *A2SupportHole = new G4Tubs("A2SupportHole", 0*cm, SecA2InRadius+0.2*cm, 8.1*cm, 0*deg, 360*deg);


  // By hook and by crook, the beamline immediately after the scat chamber is 
  // built.  This first section contains a 2 inch Aluminium pipe and several
  // flanges, bellows, vacuum gate valve, and more flanges.
  translation.setX(0); translation.setY(0); 
  translation.setZ(0);
  transform = G4Transform3D(Rot, translation);
  G4SubtractionSolid *A2SupportBored = new G4SubtractionSolid("A2SupportBored",A2Support, A2SupportHole, transform);

  translation.setX(0); translation.setY(-GateVertShift); 
  translation.setZ(0);
  transform = G4Transform3D(Rot, translation);
  G4SubtractionSolid *GateWithHole = new G4SubtractionSolid("GateWithHole",Gate,GateHole, transform);

  translation.setX(0); translation.setY(GateVertShift); 
  translation.setZ(SecA1zLen/2-GateZ/2-Flange1Thick);
  transform = G4Transform3D(Rot, translation);
  G4UnionSolid *Pipe1A = new G4UnionSolid("Pipe1A", SecA1, GateWithHole, transform);

  translation.setX(0); translation.setY(0); 
  translation.setZ(SecA1zLen/2-Flange1Thick/2);
  transform = G4Transform3D(Rot, translation);
  G4UnionSolid *Pipe1B = new G4UnionSolid("Pipe1B", Pipe1A, Flange1, transform);

  translation.setX(0); translation.setY(0); 
  translation.setZ(SecA1zLen/2-GateZ-1.5*Flange1Thick);
  transform = G4Transform3D(Rot, translation);
  G4UnionSolid *Pipe1C = new G4UnionSolid("Pipe1C", Pipe1B, Flange1, transform);

  translation.setX(0); translation.setY(0); 
  translation.setZ(SecA1zLen/2-GateZ-2.5*Flange1Thick);
  transform = G4Transform3D(Rot, translation);
  G4UnionSolid *Pipe1D = new G4UnionSolid("Pipe1D", Pipe1C, Flange1, transform);

  translation.setX(0); translation.setY(0); 
  translation.setZ(SecA1zLen/2-GateZ-2.5*Flange1Thick-BellowLen/2);
  transform = G4Transform3D(Rot, translation);
  G4UnionSolid *Pipe1E = new G4UnionSolid("Pipe1E", Pipe1D, Bellow, transform);

  translation.setX(0); translation.setY(0); 
  translation.setZ(SecA1zLen/2-GateZ-3*Flange1Thick-BellowLen);
  transform = G4Transform3D(Rot, translation);
  G4UnionSolid *Pipe1F= new G4UnionSolid("Pipe1F", Pipe1E, Flange1, transform);

  translation.setX(0); translation.setY(0); 
  translation.setZ(SecA1zLen/2-GateZ-4*Flange1Thick-BellowLen);
  transform = G4Transform3D(Rot, translation);
  G4UnionSolid *Pipe1G= new G4UnionSolid("Pipe1G", Pipe1F, Flange1, transform);


  // This segement generates the second segement of Aluminum pipe 3.5 inches 
  // in diameter and unions with it 2 flanges and a large block of Alumimum
  // for a beamline support.
  translation.setX(0); translation.setY(0); 
  translation.setZ(-SecA2zLen/2+Flange2Thick/2 + A2OffSet);
  transform = G4Transform3D(Rot, translation);
  G4UnionSolid *Pipe2A = new G4UnionSolid("Pipe2A", SecA2, A2SupportBored, transform);

  translation.setX(0); translation.setY(0); 
  translation.setZ(-SecA2zLen/2+Flange2Thick/2);
  transform = G4Transform3D(Rot, translation);
  G4UnionSolid *Pipe2B = new G4UnionSolid("Pipe2B", Pipe2A, Flange2, transform);

  translation.setX(0); translation.setY(0); 
  translation.setZ(+SecA2zLen/2-Flange2Thick/2-1.0*cm);
  transform = G4Transform3D(Rot, translation);
  G4UnionSolid *Pipe2C = new G4UnionSolid("Pipe2C", Pipe2B, Flange2, transform);

  // This segement generates the third segement of Aluminum pipe 6.0 inches 
  // in diameter and unions with it 2 flanges.
  translation.setX(0); translation.setY(0); 
  translation.setZ(SecA3zLen/2-Flange3Thick/2);
  transform = G4Transform3D(Rot, translation);
  G4UnionSolid *Pipe3A = new G4UnionSolid("Pipe3A", SecA3, Flange3, transform);

  translation.setX(0); translation.setY(0); 
  translation.setZ(-SecA3zLen/2+Flange3Thick/2+1.5*cm);
  transform = G4Transform3D(Rot, translation);
  G4UnionSolid *Pipe3B = new G4UnionSolid("Pipe3B", Pipe3A, Flange3, transform);

  // This segement Unions the three segements together
  translation.setX(0); translation.setY(0); 
  translation.setZ(SecA1zLen/2+SecA2zLen/2);
  transform = G4Transform3D(Rot, translation);
  G4UnionSolid *PipeFinalA = new G4UnionSolid("PipeFinalA", Pipe1G, Pipe2C, transform);

  translation.setX(0); translation.setY(0); 
  translation.setZ(SecA1zLen/2+SecA2zLen+SecA3zLen/2);
  transform = G4Transform3D(Rot, translation);
  G4UnionSolid *PipeFinalB = new G4UnionSolid("PipeFinalB", PipeFinalA, Pipe3B, transform);

  // Finally ...  create the logical volume and give it a pretty color
 SectionALV = new G4LogicalVolume(PipeFinalB, NpolMaterials::GetInstance()->GetMaterial("Al"), "SectionALV", 0,0,0);
 
 G4VisAttributes *BeamlineVisAtt= new G4VisAttributes(G4Colour(0.9,0.6,.8));
 SectionALV->SetVisAttributes(BeamlineVisAtt);
}


// vacuum liner for the first section
void NpolBeamlineDown::ConstructSecA1Inner(){

  G4Tubs *SecA1In = new G4Tubs("SecA1In", 0, SecA1InRadius, SecA1zLen/2, 0*deg, 360*deg);

  SecA1InLV = new G4LogicalVolume(SecA1In, NpolMaterials::GetInstance()->GetMaterial("HardVacuum"),"SecA1InLV", 0,0,0);
  
  G4VisAttributes *BeamlineVisAtt= new G4VisAttributes(G4Colour(.8,0.7,.2));
  SecA1InLV->SetVisAttributes(BeamlineVisAtt);  
}

// vacuum liner for the second section
void NpolBeamlineDown::ConstructSecA2Inner(){

  G4Tubs *SecA2In = new G4Tubs("SecA2In", 0, SecA2InRadius, SecA2zLen/2, 0*deg, 360*deg);

  SecA2InLV = new G4LogicalVolume(SecA2In, NpolMaterials::GetInstance()->GetMaterial("HardVacuum"),"SecA2InLV", 0,0,0);
  
  G4VisAttributes *BeamlineVisAtt= new G4VisAttributes(G4Colour(1.0,0.0,1.0));
  SecA2InLV->SetVisAttributes(BeamlineVisAtt);  
}

// vacuum liner for the third section
void NpolBeamlineDown::ConstructSecA3Inner(){

  G4Tubs *SecA3In = new G4Tubs("SecA3In", 0, SecA3InRadius, SecA3zLen/2, 0*deg, 360*deg);

  SecA3InLV = new G4LogicalVolume(SecA3In, NpolMaterials::GetInstance()->GetMaterial("HardVacuum"),"SecA3InLV", 0,0,0);
  
  G4VisAttributes *BeamlineVisAtt= new G4VisAttributes(G4Colour(1.0,0.0,1.0));
  SecA3InLV->SetVisAttributes(BeamlineVisAtt);  
}

void NpolBeamlineDown::Place(G4LogicalVolume *motherLV) {
  
  PlaceCylindrical(SectionALV, motherLV, "SectionA", SecA1zLen/2 + NpolScatteringChamber::insideRadius + NpolScatteringChamber::wallThickness, 0, 0);
  PlaceCylindrical(SecA1InLV, motherLV, "SecA1In", SecA1zLen/2 + NpolScatteringChamber::insideRadius + NpolScatteringChamber::wallThickness, 0, 0);
  PlaceCylindrical(SecA2InLV, motherLV, "SecA2In", SecA2zLen/2 + SecA1zLen + NpolScatteringChamber::insideRadius + NpolScatteringChamber::wallThickness, 0, 0);
  PlaceCylindrical(SecA3InLV, motherLV, "SecA3In", 0.2*cm + SecA3zLen/2 + SecA2zLen + SecA1zLen + NpolScatteringChamber::insideRadius + NpolScatteringChamber::wallThickness, 0, 0);
  PlaceCylindrical(BeamlineDownLV, motherLV, "BeamLineDown", -2.1*cm + SecA1zLen + 2*NpolScatteringChamber::insideRadius + 2*NpolScatteringChamber::wallThickness + NpolHallShell::zPlacementOffset,0,0);
  PlaceCylindrical(BeamlineDownInnerLV,BeamlineDownLV,"BeamLineDownInner", 0,0,0);
  PlaceCylindrical(CapLV,motherLV,"Cap", -1.8*cm + downLen + SecA1zLen + 2*NpolScatteringChamber::insideRadius + 2*NpolScatteringChamber::wallThickness + NpolHallShell::zPlacementOffset, 0, 0);
}

G4double NpolBeamlineDown::calculateDownBeamLineLen() {
  
  G4double x0 = NpolHallShell::xPlacementOffset;
  G4double z0 = NpolHallShell::zPlacementOffset;
  G4double r = NpolHallShell::insideRadius;
  
  return abs((z0 - sqrt(r*r - x0*x0))) + NpolScatteringChamber::insideRadius + NpolScatteringChamber::wallThickness + NpolHallShell::zPlacementOffset - NpolHallShell::creteThick/2;
}

