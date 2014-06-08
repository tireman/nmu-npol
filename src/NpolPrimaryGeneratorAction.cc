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

