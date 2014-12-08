//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.							 *
//********************************************************************
//* The Geant4 software is used by the Northern Michigan University  *
//* in accordance to the Geant4 software license specified above.    *
//* The NMU Collaboration does not hold any rights to this software  *
//* and gives full permission for its use to others under the limits *
//* imposed by the GEANT4 Collaboration.  The NMU Collaboration      *
//* gives no express or implied warranty and use of our code is at   *
//* the users discretion only.										 *
//********************************************************************

#include <map>

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
#include "G4String.hh"

#include "NpolDetectorConstruction.hh"
#include "NpolSubDetector.hh"
#include "NpolWorld.hh"
#include "NpolPolarimeter.hh"
#include "NpolTarget.hh"
#include "NpolHallShell.hh"
#include "NpolBeamline.hh"

NpolDetectorConstruction::NpolDetectorConstruction() {

	world = new NpolWorld();
	activeDetectors["Polarimeter"] = new NpolPolarimeter();
	activeDetectors["Target"] = new NpolTarget(); 
	activeDetectors["NpolHallShell"] = new NpolHallShell();
	activeDetectors["Beamline"] = new NpolBeamline();
}

NpolDetectorConstruction::~NpolDetectorConstruction() {

	delete world;
	while(!activeDetectors.empty()) {
		delete activeDetectors.begin()->second;
		activeDetectors.erase(activeDetectors.begin());
	}
}

G4VPhysicalVolume* NpolDetectorConstruction::Construct() {

	if(activeDetectors.empty())
		return NULL;
	
	// Construct the world.
	G4cout << "Constructing World." << G4endl;
	world->Construct(NULL);

	// Iterate through the map and construct each detector.
	std::map<G4String, NpolSubDetector *>::iterator it;
	for(it = activeDetectors.begin(); it != activeDetectors.end(); it++) {
		G4cout << "Constructing " << it->first << "." << G4endl;
		it->second->Construct(world->GetWorldLV());
	}

	return world->GetWorldPV();
}

void NpolDetectorConstruction::ConstructSDandField() {
}

