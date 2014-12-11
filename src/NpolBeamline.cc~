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
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4AssemblyVolume.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4ios.hh"

#include "NpolMaterials.hh"
#include "NpolBeamline.hh"

NpolBeamline::NpolBeamline() {
  G4cout << "Initializing Up Stream Beamline" << G4endl;
}

NpolBeamline::~NpolBeamline() {
  G4cout << "Deleting Up Stream Beamline" << G4endl;
}

// Construct the Up stream portion of beamline in the world
void NpolBeamline::ConstructBeamlineUpper(G4LogicalVolume *motherLV) {
  G4ThreeVector Ta;
  G4RotationMatrix Ra; 
  // Rotation of assembly inside the world 
  Ra.rotateX(0.0*deg);
  Ra.rotateY(0.0*deg);
  Ra.rotateZ(0.0*deg);
  Ta.setX(0.0*m); Ta.setY(0.0*m); Ta.setZ(-16.0*m);
  G4Transform3D Tr = G4Transform3D(Ra,Ta);

  G4Tubs *BeamlineUpper = new G4Tubs("BeamlineUpper",4.0*cm, 5.0*cm, 
	   6.0*m, 0.0*deg, 360.*deg);
  BeamlineUpperLV = new G4LogicalVolume(BeamlineUpper,
           NpolMaterials::GetInstance()->GetSSteel(),"BeamlineUpperLV",0,0,0);
  new G4PVPlacement(Tr, BeamlineUpperLV,"BeamlineUpper", motherLV, false, 0);
  G4VisAttributes *TopVisAtt= new G4VisAttributes(G4Colour(1.0,1.5,0.5));
  BeamlineUpperLV->SetVisAttributes(TopVisAtt);
}


void NpolBeamline::Construct(G4LogicalVolume *motherLV) {
  
  ConstructBeamlineUpper(motherLV);
}

