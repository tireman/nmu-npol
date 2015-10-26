//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************

// %% NpolPrimaryGeneratorAction.cc %%

// Generates the primary particle for each event
// Created: William Tireman

#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4Box.hh"
#include "Randomize.hh"

#include "NpolPrimaryGeneratorAction.hh"

NpolPrimaryGeneratorAction::NpolPrimaryGeneratorAction()
: worldBox(NULL)
{
  G4int n_particle = 1;
  particleGun  = new G4ParticleGun(n_particle);
  
  // default particle kinematic
  G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition *particle = particleTable->FindParticle(particleName="e-");
  
  particleGun->SetParticleDefinition(particle);
  particleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
  particleGun->SetParticleEnergy(3000.*MeV);
}

NpolPrimaryGeneratorAction::~NpolPrimaryGeneratorAction()
{
  delete particleGun;
}

// This function is called at the beginning of each event.
void NpolPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  
  // Right down the middle
  //G4double x0 = 0.0*m;  
  // G4double y0 = 0.0*m;
  
  // Added in a raster pattern.  Circular in shape.  2 mm diameter.
  G4double r = (2.0* (2.0*G4UniformRand()-1))*mm;
  G4double theta = (2*3.14159265 * (2.0*G4UniformRand()-1));
  G4double x0 = r*cos(theta);
  G4double y0 = r*sin(theta);

  // Start 3 meters upstream, just outside the target chamber
  G4double z0 = -3.0*m;  
  
  particleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));
  particleGun->GeneratePrimaryVertex(anEvent);
}

