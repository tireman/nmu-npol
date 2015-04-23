//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************

// %% NpolBeamlineUpper.cc %%

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

#include "NpolMaterials.hh"
#include "NpolScatteringChamber.hh"
#include "NpolHallShell.hh"
#include "NpolBeamlineUpper.hh"

G4double NpolBeamlineUpper::upperInnerRadius = 1.75*cm;
G4double NpolBeamlineUpper::upperOuterRadius = 2.5*cm;
G4double NpolBeamlineUpper::upperLen = NpolBeamlineUpper::calculateUpperBeamLineLen();

NpolBeamlineUpper::NpolBeamlineUpper() {
  ConstructBeamlineUpper();
  ConstructBeamlineUpperInner();
}

NpolBeamlineUpper::~NpolBeamlineUpper() {}

G4String NpolBeamlineUpper::GetName() {
	return G4String("Upper Beamline");
}

// Construct the Up stream portion of beamline in the world
void NpolBeamlineUpper::ConstructBeamlineUpper() {
  
  G4Tubs *BeamlineUpper = new G4Tubs("BeamlineUpper", 0, upperOuterRadius, 
     upperLen/2, 0.0*deg, 360.*deg);
  BeamlineUpperLV = new G4LogicalVolume(BeamlineUpper,
     NpolMaterials::GetInstance()->GetSSteel(),"BeamlineUpperLV",0,0,0);
  G4VisAttributes *UpperVisAtt= new G4VisAttributes(G4Colour(1.0,1.5,0.5));
  
  BeamlineUpperLV->SetVisAttributes(UpperVisAtt);
}

// Fill the up stream portion with a vacuum
void NpolBeamlineUpper::ConstructBeamlineUpperInner() {
  
  G4Tubs *BeamlineUpperInner = new G4Tubs("BeamlineUpperInner", 0, upperOuterRadius, upperLen/2, 0.0*deg, 360.*deg);
  BeamlineUpperInnerLV = new G4LogicalVolume(BeamlineUpperInner,
     NpolMaterials::GetInstance()->GetVacuum(),"BeamlineUpperInnerLV",0,0,0);
  
  BeamlineUpperInnerLV->SetVisAttributes(G4VisAttributes::GetInvisible());
}

void NpolBeamlineUpper::Place(G4LogicalVolume *motherLV) {
  
  PlaceCylindrical(BeamlineUpperLV, motherLV, "BeamLineUpper", -upperLen/2 - NpolScatteringChamber::insideRadius - NpolScatteringChamber::wallThickness ,0,0);
  PlaceCylindrical(BeamlineUpperInnerLV, BeamlineUpperLV, "BeamLineUpperInner",
     0,0,0);
}

G4double NpolBeamlineUpper::calculateUpperBeamLineLen() {
  
  G4double x0 = NpolHallShell::xPlacementOffset;
  G4double z0 = NpolHallShell::zPlacementOffset;
  G4double r = NpolHallShell::insideRadius;
  
  return abs((z0 - sqrt(r*r - x0*x0))) - NpolScatteringChamber::insideRadius - NpolScatteringChamber::wallThickness;
}

