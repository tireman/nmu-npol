//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************

#ifndef Npol_PrimaryGeneratorActionPS_h
#define Npol_PrimaryGeneratorActionPS_h

#include "G4VUserPrimaryGeneratorAction.hh"

class G4ParticleGun;
class G4Event;
class G4Box;

class NpolPrimaryGeneratorActionPS : public G4VUserPrimaryGeneratorAction
{
	public:
		NpolPrimaryGeneratorActionPS();
		virtual ~NpolPrimaryGeneratorActionPS();

		virtual void GeneratePrimaries(G4Event*);

	private:
		G4ParticleGun *particleGun;
		G4Box *worldBox;
};

#endif

