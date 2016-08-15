
//%% NpolDetectorConstruction.cc %% **

// This setups the class and calls all the classes to build the setup 
// Created: Daniel Wilbern November 2014
// Modified: William Tireman December 2014

#include <set>

#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4ios.hh"

#include "NpolDetectorConstruction.hh"
#include "NpolDetectorFactory.hh"
#include "NpolWorld.hh"
#include "NpolPolarimeter.hh"
#include "NpolPolarimeter6Layer.hh"
#include "NpolScatteringChamber.hh"
#include "NpolHallShell.hh"
#include "NpolBeamlineUpper.hh"
#include "NpolBeamlineDown.hh"
#include "NpolDipole1.hh"
#include "NpolDipole2.hh"
#include "NpolShieldHut.hh"
#include "NpolHBender.hh"
#include "NpolBDump.hh"
#include "NpolParticleFluxTagger.hh"
#include "NpolBeamLineBlocker.hh"
#include "NpolAnalysisManager.hh"

NpolDetectorConstruction::NpolDetectorConstruction() {
  
  G4cout << "Constructing volumes." << G4endl;
  world = new NpolWorld();
  //detectors.insert(new NpolPolarimeter());
  //detectors.insert(new NpolPolarimeter6Layer()); // invoke this when studying 6 layers
  //detectors.insert(new NpolScatteringChamber);
  //detectors.insert(new NpolHallShell);
  //detectors.insert(new NpolBeamlineUpper);
  //detectors.insert(new NpolBeamlineDown);
  //detectors.insert(new NpolBeamLineBlocker);
  detectors.insert(new NpolDipole1);
  detectors.insert(new NpolDipole2);
  detectors.insert(new NpolShieldHut);
  //detectors.insert(new NpolHBender);
  detectors.insert(new NpolParticleFluxTagger);
  //detectors.insert(new NpolBDump);
}

NpolDetectorConstruction::~NpolDetectorConstruction() {

  G4cout << "Deleting " << world->GetName() << G4endl;
  delete world;
  while(!detectors.empty()) {
    G4cout << "Deleting " << (*detectors.begin())->GetName() << G4endl;
    delete *(detectors.begin());
    detectors.erase(detectors.begin());
  }
}

G4VPhysicalVolume* NpolDetectorConstruction::Construct() {
  
  if(detectors.empty())
    return NULL;
  
  // Place the world.
  G4cout << "Placing World" << G4endl;
  world->Place(NULL);
  
  // Iterate through the set and construct each detector.
  std::set<NpolDetectorFactory *>::iterator it;
  for(it = detectors.begin(); it != detectors.end(); it++) {
    G4cout << "Placing " << (*it)->GetName() << G4endl;
    (*it)->Place(world->GetWorldLV());
  }
  
  return world->GetWorldPV();
}

void NpolDetectorConstruction::ConstructSDandField() {
  // Iterate through the set and construct each detector's fields and sensitive detectors.
  std::set<NpolDetectorFactory *>::iterator it;
  for(it = detectors.begin(); it != detectors.end(); it++)
	  (*it)->ConstructSDandField();
}

