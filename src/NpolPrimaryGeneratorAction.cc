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
// Modified: 26-June-2016  Changed to General Particle Source -- W.T.

#include "G4SystemOfUnits.hh"
#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4Box.hh"
#include "Randomize.hh"

#include "NpolPrimaryGeneratorAction.hh"

NpolPrimaryGeneratorAction::NpolPrimaryGeneratorAction()
  : G4VUserPrimaryGeneratorAction(), fParticleGun(0), worldBox(NULL)
{
  fParticleGun = new G4GeneralParticleSource(); 
}

NpolPrimaryGeneratorAction::~NpolPrimaryGeneratorAction()
{
  delete fParticleGun;
}

// This function is called at the beginning of each event.
void NpolPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  fParticleGun->GeneratePrimaryVertex(anEvent);
}

