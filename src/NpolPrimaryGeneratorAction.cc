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
  G4ParticleDefinition *particle = particleTable->FindParticle(particleName="neutron");
 
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
  
  G4double x0 = 0.0*m;
  G4double y0 = 0.0*m;
  G4double z0 = -5.0*m;

  particleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));
  particleGun->GeneratePrimaryVertex(anEvent);
  
}

