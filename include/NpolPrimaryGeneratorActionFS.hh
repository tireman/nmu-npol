// ********************************************************************
// Header file for the Flat Source primary generator action
// William Tireman 2-26-2015

#ifndef Npol_PrimaryGeneratorActionFS_h
#define Npol_PrimaryGeneratorActionFS_h

#include "G4VUserPrimaryGeneratorAction.hh"
//#include "globals.hh"
#include "G4SystemOfUnits.hh"

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

