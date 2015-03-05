//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************

// %% NpolShieldHut.hh file %% //

// Created: William Tireman, January 2015

#ifndef Npol_ShieldHut_h
#define Npol_ShieldHut_h

#include "NpolSubDetector.hh"

class G4LogicalVolume;
class G4AssemblyVolume;
class G4VPhysicalVolume;

class NpolShieldHut : public NpolSubDetector {

	public:
		NpolShieldHut();
		~NpolShieldHut();

		void ConstructHutSideWall();
		void ConstructHutFrontWall();
		void ConstructHutBackWall();
		void ConstructHutRoof();

		virtual G4VPhysicalVolume *Construct(G4LogicalVolume *motherLV);

	private: 
		G4LogicalVolume *HutSideWallLV;
		G4LogicalVolume *HutFrontWallLV;
		G4LogicalVolume *HutBackWallLV;
		G4LogicalVolume *HutRoofLV;
};

#endif

