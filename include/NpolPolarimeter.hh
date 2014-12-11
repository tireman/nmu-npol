//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************
//* The Geant4 software is used by the Northern Michigan University  *
//* in accordance to the Geant4 software license specified above.    *
//* The NMU Collaboration does not hold any rights to this software  *
//* and gives full permission for its use to others under the limits *
//* imposed by the GEANT4 Collaboration.  The NMU Collaboration      *
//* gives no express or implied warranty and use of our code is at   *
//* the users discretion only.  		    			*
//********************************************************************

#ifndef Npol_Polarimeter_h
#define Npol_Polarimeter_h

#include "G4SystemOfUnits.hh"

#include "NpolSubDetector.hh"

class G4LogicalVolume;
class G4AssemblyVolume;

class NpolPolarimeter : public NpolSubDetector {

	public:
		NpolPolarimeter();
		~NpolPolarimeter();

		void ConstructTopDetArray(G4LogicalVolume *motherLV);
		void ConstructTopVetoArray(G4LogicalVolume *motherLV);
		void ConstructBottomDetArray(G4LogicalVolume *motherLV);
		void ConstructBottomVetoArray(G4LogicalVolume *motherLV);
		void ConstructFrontDetArray(G4LogicalVolume *motherLV);
		void ConstructFrontTagArray(G4LogicalVolume *motherLV);

		virtual void Construct(G4LogicalVolume *motherLV);

};

G4AssemblyVolume *MakePlate(G4LogicalVolume *detLV, G4int numDets, 
		G4double TmX, G4double TmY, G4double TmZ,
			    G4double TmdX, G4double TmdY, G4double TmdZ);

#endif

