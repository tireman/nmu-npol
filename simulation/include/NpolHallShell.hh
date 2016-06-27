//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************

#ifndef Npol_HallShell_h
#define Npol_HallShell_h

#include "G4SystemOfUnits.hh"
#include "NpolDetectorFactory.hh"

class G4LogicalVolume;
class G4AssemblyVolume;
class G4VPhsycialVolume;

class NpolHallShell : public NpolDetectorFactory {

	public:
		NpolHallShell();
		~NpolHallShell();

		void ConstructHallShellWall();
		void ConstructHallShellFloor();
		void ConstructHallShellRoof();

		virtual G4String GetName();
		virtual void Place(G4LogicalVolume *motherLV);

		static G4double insideRadius;
		static G4double floorRadius;
		static G4double shellHeight;
		static G4double roofRadius;
		static G4double creteThick;
		static G4double xPlacementOffset;
		static G4double zPlacementOffset;

	private: 
		G4LogicalVolume *HallShellWallLV;
		G4LogicalVolume *HallShellFloorLV;
		G4LogicalVolume *HallShellRoofLV;
};

#endif

