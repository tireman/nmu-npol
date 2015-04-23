//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.							 *
//********************************************************************

#ifndef Npol_DetectorConstruction_h
#define Npol_DetectorConstruction_h

#include <set>

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"

class G4AssemblyVolume;
class G4VPhysicalVolume;
class G4String;

class NpolSensitiveDetector;
class NpolSubDetector;
class NpolWorld;

class NpolDetectorConstruction : public G4VUserDetectorConstruction
{
	public:
		NpolDetectorConstruction();
		~NpolDetectorConstruction();

		virtual G4VPhysicalVolume* Construct();
		virtual void ConstructSDandField();

	private:
		std::set<NpolSubDetector *> detectors;
		NpolWorld *world;
};

#endif

