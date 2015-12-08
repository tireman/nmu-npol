//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//******************************************************************

// Primary Generator Action to produce a "flat source" of particles all 
// with the same intial momentum pointed in the same direction but 
// generated randomly from a point on a rectangular surface defined by 
// NpoAng and the width (xz plane) and height (y axis) maximums.
//
// William Tireman 2/26/2015

#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4Box.hh"
#include "Randomize.hh"

#include "NpolPrimaryGeneratorActionFS.hh"

	NpolPrimaryGeneratorActionFS::NpolPrimaryGeneratorActionFS()
: worldBox(NULL)
{
	G4int n_particle = 1;
	G4double NpolAng = 28.0*deg; // Npol Ang for Neutron/Proton 28.0*deg;
	particleGun  = new G4ParticleGun(n_particle);

	// default particle kinematic
	G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
	G4String particleName;
	G4ParticleDefinition *particle = particleTable->FindParticle(particleName="neutron");

	particleGun->SetParticleDefinition(particle);
	particleGun->SetParticleMomentumDirection(G4ThreeVector(-sin(NpolAng),0.,cos(NpolAng)));
	particleGun->SetParticleEnergy(2000.*MeV);
}

NpolPrimaryGeneratorActionFS::~NpolPrimaryGeneratorActionFS()
{
	delete particleGun;
}

// This function is called at the beginning of each event.
void NpolPrimaryGeneratorActionFS::GeneratePrimaries(G4Event* anEvent)
{
  
  G4double ymax = 0.600*m, xzmax = 1.00*m, NpolAng = 28.0*deg;
  G4double x0 = xzmax/2*cos(NpolAng)*(2.0*G4UniformRand()-1);
  G4double y0 = ymax/2 * (2.0*G4UniformRand()-1);
  G4double z0 = xzmax/2*sin(NpolAng)*(2.0*G4UniformRand()-1);
  
  particleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));
  particleGun->GeneratePrimaryVertex(anEvent);
  
}

