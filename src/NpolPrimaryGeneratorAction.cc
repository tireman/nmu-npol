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
   //* The NMU software is not to be distributed in whole or modified   *
   //* form without including the Geant4 license.  Use of the NMU code  *
   //* for development of other code is permitted as long as the work   *
   //* is not a direct copy of the work being performed here. If it is  *
   //* such then we ask that you give credit to our work out of  	*
   //* professional courtesy and acknowledgment.  The NMU Collaboration *
   //* gives no express or implied warranty and use of our code is at   *
   //* the users discretion only.  		    			*
   //********************************************************************

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
	G4ParticleDefinition *particle = particleTable->FindParticle(particleName="neutron");
	particleGun->SetParticleDefinition(particle);
	particleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
	particleGun->SetParticleEnergy(7000.*MeV);
}

NpolPrimaryGeneratorAction::~NpolPrimaryGeneratorAction()
{
	delete particleGun;
}

// This function is called at the beginning of each event.
void NpolPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{

	G4double x0 = 0.50 * (2.0*G4UniformRand()-1)*m;
	G4double y0 = 0.30 * (2.0*G4UniformRand()-1)*m;
	G4double z0 = -2.0*m;

	particleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));
	particleGun->GeneratePrimaryVertex(anEvent);

}

