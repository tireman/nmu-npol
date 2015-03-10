
//%% NpolDetectorConstruction.cc %% **

// This setups the class and calls all the classes to build the setup 
// Created: Daniel Wilbern November 2014
// Modified: William Tireman December 2014

#include <map>

#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4ios.hh"

#include "NpolDetectorConstruction.hh"
#include "NpolSubDetector.hh"
#include "NpolWorld.hh"
#include "NpolPolarimeter.hh"
#include "NpolScatteringChamber.hh"
#include "NpolHallShell.hh"
#include "NpolBeamline.hh"
#include "NpolDipole1.hh"
#include "NpolDipole2.hh"
#include "NpolShieldHut.hh"
#include "NpolHBender.hh"
#include "NpolBDump.hh"
#include "NpolDataStructure.hh"

NpolDetectorConstruction::NpolDetectorConstruction() {

	world = new NpolWorld();
	activeDetectors["Polarimeter"] = new NpolPolarimeter();
	activeDetectors["Scattering Chamber"] = new NpolScatteringChamber(); 
	activeDetectors["Hall Shell"] = new NpolHallShell();
//	activeDetectors["Beamline"] = new NpolBeamline();
	activeDetectors["Dipole 1"] = new NpolDipole1();
	activeDetectors["Dipole 2"] = new NpolDipole2();
	activeDetectors["Shield Hut"] = new NpolShieldHut();
	activeDetectors["HBender"] = new NpolHBender();
	activeDetectors["Beam Dump"] = new NpolBDump();
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

