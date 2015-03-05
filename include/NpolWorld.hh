//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		                 *
//********************************************************************

#ifndef Npol_Polarimiter_h
#define Npol_Polarimiter_h

#include "NpolSubDetector.hh"

class G4LogicalVolume;
class G4AssemblyVolume;
class G4VPhysicalVolume;

class NpolWorld : public NpolSubDetector {

	public:
		NpolWorld();
		~NpolWorld();

		virtual G4VPhysicalVolume *Construct(G4LogicalVolume *motherLV);
		G4VPhysicalVolume *GetWorldPV();
		G4LogicalVolume *GetWorldLV();

	private:
		G4VPhysicalVolume *expHallPV;
		G4LogicalVolume *expHallLV;
};

#endif

