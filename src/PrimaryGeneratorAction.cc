//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: PrimaryGeneratorAction.cc,v 1.5 2006/06/29 16:33:05 gunter Exp $
// --------------------------------------------------------------
//

#include "PrimaryGeneratorAction.hh"
#include "PrimaryGeneratorMessenger.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "Randomize.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
  G4int n_particle = 1;
  particleGun  = new G4ParticleGun(n_particle);

  //create a messenger for this class
  gunMessenger = new PrimaryGeneratorMessenger(this);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete particleGun;
  delete gunMessenger;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{

  //start new part //
  G4ParticleDefinition* particle;

  // Generate initial position of particle.
  G4double zpos = -2.00*m;
  G4double xpos = 0.0*m;
  G4double ypos = 0.0*m;

  // generate a random uniform x and y starting position
  xpos = 0.50*(2.0*G4UniformRand()-1)*m;
  ypos = 0.30*(2.0*G4UniformRand()-1)*m;

  // set particle kinematics
  G4double Ekin = particleGun->GetParticleEnergy();
  particle = particleGun->GetParticleDefinition();
  particleGun->SetParticleEnergy(Ekin);
  particleGun->SetParticlePosition(G4ThreeVector(xpos,ypos,zpos));
  particleGun->SetParticleMomentumDirection(G4ThreeVector(0.0,0.0,1.0));
  particleGun->GeneratePrimaryVertex(anEvent);


  //end new part //
}


