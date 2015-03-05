//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.							 *
//********************************************************************

// Header file for the Flat Source primary generator action

// William Tireman 2-26-2015

#ifndef Npol_PrimaryGeneratorActionFS_h
#define Npol_PrimaryGeneratorActionFS_h

#include "G4VUserPrimaryGeneratorAction.hh"

class G4ParticleGun;
class G4Event;
class G4Box;

class NpolPrimaryGeneratorActionFS : public G4VUserPrimaryGeneratorAction
{
	public:
		NpolPrimaryGeneratorActionFS();
		virtual ~NpolPrimaryGeneratorActionFS();

		virtual void GeneratePrimaries(G4Event*);

	private:
		G4ParticleGun *particleGun;
		G4Box *worldBox;
};

#endif

