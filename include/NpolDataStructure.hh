//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************

// Daniel Wilbern, dwilbern@nmu.edu February 2015

#ifndef Npol_Data_Structure_h
#define Npol_Data_Structure_h

#include <map>

#include "G4SystemOfUnits.hh"

class G4VPhysicalVolume;

class NpolDataStructure {

	public:
		static NpolDataStructure *GetInstance();
		void PrepareNewEvent();
		void AddEDep(G4VPhysicalVolume *PV, G4double dep);
		void FillHistograms();

	private:
		NpolDataStructure();
		~NpolDataStructure();

		std::map<G4VPhysicalVolume *, G4double> EDep;
};

#endif

