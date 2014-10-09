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
   //* The NMU Collaboration does not hold any rights to this software  *
   //* and gives full permission for its use to others under the limits *
   //* imposed by the GEANT4 Collaboration.  The NMU Collaboration      *
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

  // insert code to calculate a momentum vector from a point source and
  // restricted in theta and phi

  G4double thmax = 1.4995; //(atan(0.1*25/6950))*deg;
  G4double phimax = 0.042831;  // atan((0.1*50/6950))*deg;
  G4double theta = acos(cos(thmax)*(2*G4UniformRand()-1));
  G4double phi = phimax * (2*G4UniformRand()-1);
  
  G4double pox = -cos(theta);
  G4double poy = sin(theta)*sin(phi);
  G4double poz = sin(theta)*cos(phi);

  particleGun->SetParticleMomentumDirection(G4ThreeVector(pox,poy,poz));
  particleGun->SetParticleEnergy(2000.*MeV);
}

NpolPrimaryGeneratorAction::~NpolPrimaryGeneratorAction()
{
  delete particleGun;
}

// This function is called at the beginning of each event.
void NpolPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  
//G4double x0 = 0.50 * (2.0*G4UniformRand()-1)*m;
// G4double y0 = 0.30 * (2.0*G4UniformRand()-1)*m;
//G4double z0 = -2.0*m;
  
G4double x0 = 0.0*m;
G4double y0 = 0.0*m;
G4double z0 = -8.7*m;

  particleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));
  particleGun->GeneratePrimaryVertex(anEvent);
  
}

