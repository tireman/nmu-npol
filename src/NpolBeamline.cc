// %% NpolBeamline.cc %%

// Beam line construction file
// Created: William Tireman - December 2014

#include "G4PhysicalConstants.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4Cons.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4AssemblyVolume.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4ios.hh"

#include "NpolMaterials.hh"
#include "NpolBeamline.hh"

NpolBeamline::NpolBeamline() {
  G4cout << "Initializing Beamline: Up-stream and Down-Stream" << G4endl;
}

NpolBeamline::~NpolBeamline() {
  G4cout << "Deleting Beamline: Up-stream and Down-Stream" << G4endl;
}

// Construct the Up stream portion of beamline in the world
void NpolBeamline::ConstructBeamlineUpper(G4LogicalVolume *motherLV) {
  G4ThreeVector Ta;
  G4RotationMatrix Ra; 
  // Rotation of assembly inside the world 
  Ra.rotateX(0.0*deg);
  Ra.rotateY(0.0*deg);
  Ra.rotateZ(0.0*deg);
  Ta.setX(0.0*m); Ta.setY(0.0*m); Ta.setZ(-11.25*m);
  G4Transform3D Tr = G4Transform3D(Ra,Ta);

  G4Tubs *BeamlineUpper = new G4Tubs("BeamlineUpper",7.0*cm, 8.0*cm, 
	   11.0*m, 0.0*deg, 360.*deg);
  BeamlineUpperLV = new G4LogicalVolume(BeamlineUpper,
           NpolMaterials::GetInstance()->GetSSteel(),"BeamlineUpperLV",0,0,0);
  new G4PVPlacement(Tr, BeamlineUpperLV,"BeamlineUpper", motherLV, false, 0);
  G4VisAttributes *TopVisAtt= new G4VisAttributes(G4Colour(1.0,1.5,0.5));
  BeamlineUpperLV->SetVisAttributes(TopVisAtt);
}

// Fill the up stream portion with a vacuum
void NpolBeamline::ConstructBeamlineUpperInner(G4LogicalVolume *motherLV) {
  G4ThreeVector Ta;
  G4RotationMatrix Ra; 
  // Rotation of assembly inside the world 
  Ra.rotateX(0.0*deg);
  Ra.rotateY(0.0*deg);
  Ra.rotateZ(0.0*deg);
  Ta.setX(0.0*m); Ta.setY(0.0*m); Ta.setZ(-11.35*m);
  G4Transform3D Tr = G4Transform3D(Ra,Ta);

  G4Tubs *BeamlineUpperInner = new G4Tubs("BeamlineUpperInner",0.0*cm, 7.0*cm, 
	   11.1*m, 0.0*deg, 360.*deg);
  BeamlineUpperInnerLV = new G4LogicalVolume(BeamlineUpperInner,
           NpolMaterials::GetInstance()->GetVacuum(),"BeamlineUpperInnerLV",0,0,0);
  new G4PVPlacement(Tr, BeamlineUpperInnerLV,"BeamlineUpperInner", 
	   motherLV, false, 0);
  BeamlineUpperInnerLV->SetVisAttributes(G4VisAttributes::GetInvisible());
}

// Construct the Down stream portion of beamline in the world
void NpolBeamline::ConstructBeamlineDown(G4LogicalVolume *motherLV) {
  G4ThreeVector Ta;
  G4RotationMatrix Ra; 
  // Rotation of assembly inside the world 
  Ra.rotateX(0.0*deg);
  Ra.rotateY(0.0*deg);
  Ra.rotateZ(0.0*deg);
  Ta.setX(0.0*m); Ta.setY(0.0*m); Ta.setZ(+11.25*m);
  G4Transform3D Tr = G4Transform3D(Ra,Ta);

  G4Cons *BeamlineDown = new G4Cons("BeamlineDown", 7.0*cm, 8.0*cm, 25.0*cm, 
	   26.0*cm, 11.0*m, 0.0*deg, 360.*deg);
  BeamlineDownLV = new G4LogicalVolume(BeamlineDown,
           NpolMaterials::GetInstance()->GetSSteel(),"BeamlineDownLV",0,0,0);
  new G4PVPlacement(Tr, BeamlineDownLV,"BeamlineDown", motherLV, false, 0);
  G4VisAttributes *TopVisAtt= new G4VisAttributes(G4Colour(1.0,1.5,0.5));
  BeamlineDownLV->SetVisAttributes(TopVisAtt);
  // BeamlineDownLV->SetVisAttributes(G4VisAttributes::GetInvisible());
}

// Fill the Down-stream with a vacuum
void NpolBeamline::ConstructBeamlineDownInner(G4LogicalVolume *motherLV) {
  G4ThreeVector Ta;
  G4RotationMatrix Ra; 
  // Rotation of assembly inside the world 
  Ra.rotateX(0.0*deg);
  Ra.rotateY(0.0*deg);
  Ra.rotateZ(0.0*deg);
  Ta.setX(0.0*m); Ta.setY(0.0*m); Ta.setZ(+11.35*m);
  G4Transform3D Tr = G4Transform3D(Ra,Ta);

  G4Cons *BeamlineDownInner = new G4Cons("BeamlineDownInner", 
	0.0*cm, 7.0*cm, 0.0*cm, 25.0*cm, 11.1*m, 0.0*deg, 360.*deg);
  BeamlineDownInnerLV = new G4LogicalVolume(BeamlineDownInner,
	 NpolMaterials::GetInstance()->GetVacuum(),"BeamlineDownInnerLV",0,0,0);
  new G4PVPlacement(Tr, BeamlineDownInnerLV,"BeamlineDownInner", motherLV, false, 0);
  BeamlineDownInnerLV->SetVisAttributes(G4VisAttributes::GetInvisible());
}

void NpolBeamline::Construct(G4LogicalVolume *motherLV) {
  
  ConstructBeamlineUpper(motherLV);
  ConstructBeamlineDown(motherLV);
  ConstructBeamlineUpperInner(BeamlineUpperLV);
  ConstructBeamlineDownInner(BeamlineDownLV);
}
