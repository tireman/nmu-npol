//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//******************************************************************

// %% NpolBeamLineBlocker.cc %%

// Beam Line Blocker construction file
// Created: William Tireman - July 2016

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4ios.hh"

#include "NpolMaterials.hh"
#include "NpolBeamLineBlocker.hh"

G4double NpolBeamLineBlocker::lenX = 0.050*m;
G4double NpolBeamLineBlocker::lenY = 2.00*m;
G4double NpolBeamLineBlocker::lenZ = 20.0*m; 
G4double NpolBeamLineBlocker::offsetX = -0.35*m;
G4double NpolBeamLineBlocker::offsetY = 0.0*m;
G4double NpolBeamLineBlocker::offsetZ = 11.65*m;

NpolBeamLineBlocker::NpolBeamLineBlocker() {
  ConstructBeamLineBlocker();
}

NpolBeamLineBlocker::~NpolBeamLineBlocker() {}

G4String NpolBeamLineBlocker::GetName() {
  return G4String("Beam Line Blocker");
}

// Construct a thin wall of SSteel between beam line and taggers to run without 
// the dipole magnets but approximate they are there to blck secondaries from
// the particle taggers
void NpolBeamLineBlocker::ConstructBeamLineBlocker(){  

  G4Box *BeamLineBlocker = new G4Box("BeamLineBlocker",lenX/2,lenY/2,lenZ/2);
  BeamLineBlockerLV = 
	new G4LogicalVolume(BeamLineBlocker,NpolMaterials::GetInstance()->GetMaterial("SSteel"),"BeamLineBlockerLV",0,0,0);
  G4VisAttributes *TaggerVisAtt = new G4VisAttributes(G4Colour(0.2, 0.2, 0.8));
  BeamLineBlockerLV->SetVisAttributes(TaggerVisAtt);
}

void NpolBeamLineBlocker::Place(G4LogicalVolume *motherLV) {
  
  PlaceRectangular(BeamLineBlockerLV, motherLV, "BeamLineBlocker", offsetX, offsetY, offsetZ,0.0*deg,-1.0*deg, 0.0*deg);

}

