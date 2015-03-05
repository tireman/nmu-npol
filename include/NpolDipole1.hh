//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.							 *
//********************************************************************

// && NpoleDipole1 Header file -- Charybdis Magnet

// Created: William Tireman - December 2014

#ifndef Npol_Dipole1_h
#define Npol_Dipole1_h

#include "G4SystemOfUnits.hh"

#include "NpolSubDetector.hh"

class G4LogicalVolume;
class G4AssemblyVolume;
class G4VPhysicalVolume;

class NpolDipole1 : public NpolSubDetector {

	public:
		NpolDipole1();
		~NpolDipole1();

		void ConstructDipole1Yoke();
		void ConstructDipole1CuBar();
		void ConstructDipole1CuEnd();
		void ConstructDipole1FieldClamp1();
		void ConstructDipole1FieldClamp2();
		virtual G4VPhysicalVolume *Construct(G4LogicalVolume *motherLV);

	private: 
		G4LogicalVolume *Dipole1YokeLV, *Dipole1CuBarLV, *Dipole1CuEndLV;
		G4LogicalVolume *FieldClamp1LV, *FieldClamp2LV;
};

#endif

