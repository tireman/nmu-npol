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

#ifndef Npol_SubDetector_h
#define Npol_SubDetector_h

#include "G4SystemOfUnits.hh"

class G4LogicalVolume;

class NpolSubDetector {

	public:
		NpolSubDetector() {}
		virtual ~NpolSubDetector() {}

		virtual G4VPhysicalVolume *Construct(G4LogicalVolume *motherLV) = 0;

		G4VPhysicalVolume *PlaceRectangular(G4LogicalVolume *detLV, G4LogicalVolume *motherLV,
				G4String detName, G4double x, G4double y, G4double z,
				G4double Rx = 0.0*deg, G4double Ry = 0.0*deg, G4double Rz = 0.0*deg);
		G4VPhysicalVolume *PlaceCylindrical(G4LogicalVolume *detLV, G4LogicalVolume *motherLV,
				G4String detName, G4double rho, G4double phi, G4double z);
};

#endif

