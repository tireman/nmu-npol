//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.							 *
//********************************************************************
//* The Geant4 software is used by the Northern Michigan University  *
//* in accordance to the Geant4 software license specified above.    *
//* The NMU Collaboration does not hold any rights to this software  *
//* and gives full permission for its use to others under the limits *
//* imposed by the GEANT4 Collaboration.  The NMU Collaboration      *
//* gives no express or implied warranty and use of our code is at   *
//* the users discretion only.										 *
//********************************************************************

#ifndef Npol_DetectorConstruction_h
#define Npol_DetectorConstruction_h

#include <map>

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

		G4VPhysicalVolume *ConstructSubDetector(NpolSubDetector *det);

	private:
		
		std::map<G4String, NpolSubDetector *> activeDetectors;
		NpolWorld *world;
};

#endif

