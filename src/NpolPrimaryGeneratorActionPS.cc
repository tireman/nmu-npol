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

  // insert code to calculate a momentum vector from a point source and
  // restricted in theta and phi
  // Theta is the azimulthal angle and phi is the rotation angle 

  //G4double thmax = 1.4995; //(atan(0.1*25/6950))*deg;
  //G4double phimax = 0.42831;  // atan((0.1*50/6950))*deg;
  //G4double phimax = 0.48869;
  //G4double theta = -28*3.14/180+acos(cos(thmax)*(2*G4UniformRand()-1));
  //G4double phi = phimax * (2*G4UniformRand()-1);
  // AHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH!!! It doesn't work!
  G4double rand1, rand2;
  rand1 = G4UniformRand(); rand2 = G4UniformRand();
  G4cout <<"Random 1 "<< rand1 << "Random 2 " << rand2 << G4endl;

  G4double theta = -(28.0+0.289*(2*G4UniformRand()-1))*deg;
  G4double phi = (30.975*(2*G4UniformRand()-1))*deg;
  
  G4double poz = cos(theta);
  G4double poy = sin(theta)*sin(phi);
  G4double pox = sin(theta)*cos(phi);

  particleGun->SetParticleMomentumDirection(G4ThreeVector(pox,poy,poz));
  particleGun->SetParticleEnergy(2000.*MeV);
}

NpolPrimaryGeneratorActionPS::~NpolPrimaryGeneratorActionPS()
{
  delete particleGun;
}

// This function is called at the beginning of each event.
void NpolPrimaryGeneratorActionPS::GeneratePrimaries(G4Event* anEvent)
{
  
G4double x0 = 0.0*m;
G4double y0 = 0.0*m;
G4double z0 = 0.0*m;

  particleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));
  particleGun->GeneratePrimaryVertex(anEvent);
  
}

