//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.							 *
//********************************************************************

// && NpoleDipole2 Header file BNL 48D48 dipole magnet

// Created: William Tireman - January 2015

#ifndef Npol_Dipole2_h
#define Npol_Dipole2_h

#include "G4SystemOfUnits.hh"

#include "NpolDetectorFactory.hh"

class G4LogicalVolume;
class G4AssemblyVolume;
class G4VPhysicalVolume;

class NpolDipole2 : public NpolDetectorFactory {

	public:
		NpolDipole2();
		~NpolDipole2();

		void ConstructDipole2Yoke();
		void ConstructDipole2Coil();
		void ConstructDipole2CuBar();
		void ConstructDipole2CuEnd();
		void ConstructDipole2FieldClamp();

		virtual G4String GetName();
		virtual void Place(G4LogicalVolume *motherLV);

	private: 
		G4LogicalVolume *Dipole2YokeLV, *Dipole2CuBarLV, *Dipole2CuEndLV;
		G4LogicalVolume *FieldClampLV;
};

#endif

