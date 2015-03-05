//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************

// %% NpolBeamline.cc %%

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
#include "NpolBeamline.hh"

NpolBeamline::NpolBeamline() {
	G4cout << "Initializing Beamline: Up-stream and Down-Stream" << G4endl;
}

NpolBeamline::~NpolBeamline() {
	G4cout << "Deleting Beamline: Up-stream and Down-Stream" << G4endl;
}

// This downstream beamline is a extruded solid consisting of 60 facets for
// circular portion constructed in polygon and two sections in z-axis. 
//  First section is a fixed length and diameter while the section section 
// is scaled by using the scaling in zsections for the last section.  This 
// yields a tappered section that can be adjusted later. 
void NpolBeamline::ConstructBeamlineDown(){
	G4double dia = 2.5*cm, zlen1 = +5.44*m, zlen2 = 26.78*m, tScale = 5.0;
	G4int gnum = 60; // # of facets to circle for polygon

	std::vector<G4TwoVector> polygon(gnum);
	for (int i = 0; i< gnum-1; i++) {
		polygon[i] = G4TwoVector(dia*cos(360.0*deg/gnum*i),
				dia*sin(360.0*deg/gnum*i));
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
			->GetSSteel(), "BeamlineDownLV", 0,0,0);
	G4VisAttributes *BeamlineVisAtt= new G4VisAttributes(G4Colour(1.0,1.5,0.5));
	BeamlineDownLV->SetVisAttributes(BeamlineVisAtt);
}

// This downstream inner part of the beam line is just a vacuum filler for 
// the downstream portion made in the previous method. 
void NpolBeamline::ConstructBeamlineDownInner(){
	G4double dia = 1.75*cm, zlen1 = +5.44*m, zlen2 = 26.78*m, tScale = 5.0;
	G4int gnum =60;  // # of facets to cirle for polygon
	std::vector<G4TwoVector> polygon(gnum);
	for (int i = 0; i < gnum-1; i++) {
		polygon[i] = G4TwoVector(dia*cos(360.0*deg/gnum*i),
				dia*sin(360.0*deg/gnum*i));
	}

	std::vector<G4ExtrudedSolid::ZSection> zsections;
	zsections.push_back(G4ExtrudedSolid::ZSection(0.0*m,
				G4TwoVector(0*cm,0*cm),1.0));
	zsections.push_back(G4ExtrudedSolid::ZSection(zlen1,
				G4TwoVector(0*cm,0*cm),1.0));
	zsections.push_back(G4ExtrudedSolid::ZSection(zlen2,
				G4TwoVector(0*cm,0*cm),tScale));

	G4ExtrudedSolid *BeamlineDownInner = new G4ExtrudedSolid("BeamlineDownInner",
			polygon,zsections); 
	BeamlineDownInnerLV = new G4LogicalVolume(BeamlineDownInner,
			NpolMaterials::GetInstance()->GetVacuum(), "BeamlineDownInnerLV", 0,0,0);
	G4VisAttributes *BeamlineVisAtt= new G4VisAttributes(G4Colour(100,0.0,1.0));
	BeamlineDownInnerLV->SetVisAttributes(BeamlineVisAtt);  
}

// Construct the Up stream portion of beamline in the world
void NpolBeamline::ConstructBeamlineUpper() {
	G4double len= 9.02*m, inDia = 0.0*cm, outDia = 2.5*cm;
	G4Tubs *BeamlineUpper = new G4Tubs("BeamlineUpper", inDia, outDia, 
			len, 0.0*deg, 360.*deg);
	BeamlineUpperLV = new G4LogicalVolume(BeamlineUpper,
			NpolMaterials::GetInstance()->GetSSteel(),"BeamlineUpperLV",0,0,0);
	G4VisAttributes *UpperVisAtt= new G4VisAttributes(G4Colour(1.0,1.5,0.5));
	BeamlineUpperLV->SetVisAttributes(UpperVisAtt);
}

// Fill the up stream portion with a vacuum
void NpolBeamline::ConstructBeamlineUpperInner() {
	G4double len= 9.02*m, inDia = 0.0*cm, outDia = 1.75*cm;
	G4Tubs *BeamlineUpperInner = new G4Tubs("BeamlineUpperInner", inDia, outDia, 
			len, 0.0*deg, 360.*deg);
	BeamlineUpperInnerLV = new G4LogicalVolume(BeamlineUpperInner,
			NpolMaterials::GetInstance()->GetVacuum(),"BeamlineUpperInnerLV",0,0,0);
	BeamlineUpperInnerLV->SetVisAttributes(G4VisAttributes::GetInvisible());
}

G4VPhysicalVolume *NpolBeamline::Construct(G4LogicalVolume *motherLV) {

	ConstructBeamlineUpper();
	ConstructBeamlineDown();
	ConstructBeamlineUpperInner();
	ConstructBeamlineDownInner();
	PlaceCylindrical(BeamlineUpperLV, motherLV, "BeamLineUpper", -9.125*m,0,0);
	PlaceCylindrical(BeamlineUpperInnerLV, BeamlineUpperLV, "BeamLineUpperInner",
			0,0,0);
	PlaceCylindrical(BeamlineDownLV, motherLV, "BeamLineDown", +0.103*m,0,0);
	return PlaceCylindrical(BeamlineDownInnerLV,BeamlineDownLV,"BeamLineDownInner", 0,0,0);
}

