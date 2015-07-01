//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//******************************************************************

// Primary Generator Action file with a point source and 
// limited in theta and phi angles 

#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4Box.hh"
#include "Randomize.hh"

#include "NpolPrimaryGeneratorActionPS.hh"

NpolPrimaryGeneratorActionPS::NpolPrimaryGeneratorActionPS()
: worldBox(NULL)

{
	G4int n_particle = 1;
	particleGun  = new G4ParticleGun(n_particle);

	// default particle kinematic
	G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
	G4String particleName;
	G4ParticleDefinition *particle = particleTable->FindParticle(particleName="neutron");

	particleGun->SetParticleDefinition(particle);
}

NpolPrimaryGeneratorActionPS::~NpolPrimaryGeneratorActionPS()
{
	delete particleGun;
}

// This function is called at the beginning of each event.
void NpolPrimaryGeneratorActionPS::GeneratePrimaries(G4Event* anEvent)
{
	// Set the initial position.  For this it is at the center of the 
	// coordinate system (target).
	G4double x0 = 0.0*m, y0 = 0.0*m, z0 = 0.0*m;

	// insert code to calculate a momentum vector from a point source and
	// restricted in theta and phi
	// Theta is the azimulthal angle and phi is the rotation angle 

	// Generate random angles between maximum and minimum angles
	G4double NpolAng = 28.0, dTheta = 3.975, dPhi = 3.975;

	G4double theta = -(NpolAng+dTheta*(2*G4UniformRand()-1))*deg;
	G4double phi = (dPhi*(2*G4UniformRand()-1))*deg;

	// Generate the momentum three vector and set the particle momentum
	// direction and energy (energy can be changed from macro file).
	G4ThreeVector momDirection;
	momDirection.setRThetaPhi(1., theta, phi);
	particleGun->SetParticleMomentumDirection(momDirection);
	particleGun->SetParticleEnergy(2000.*MeV);

	// Initialize the particle and generate the event.
	particleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));
	particleGun->GeneratePrimaryVertex(anEvent);
}

