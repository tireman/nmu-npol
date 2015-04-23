//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************

// Created: William Tireman - December 2014
// Modified: Daniel Wilbern - December 2014

#ifndef Npol_BeamlineDown_h
#define Npol_BeamlineDown_h

#include "G4SystemOfUnits.hh"
#include "NpolDetectorFactory.hh"

class G4LogicalVolume;
class G4AssemblyVolume;
class G4VPhysicalVolume;

class NpolBeamlineDown : public NpolDetectorFactory {

	public:
		NpolBeamlineDown();
		~NpolBeamlineDown();

		void ConstructBeamlineDown();
		void ConstructBeamlineDownInner();
		void ConstructBeamlineSecA();
		void ConstructSecA1Inner();
		void ConstructSecA2Inner();
		void ConstructSecA3Inner();

		virtual G4String GetName();
		virtual void Place(G4LogicalVolume *motherLV);

		static G4double Flange1Thick; 
		static G4double Flange2Thick; 
		static G4double Flange3Thick;
		static G4double BellowLen; 
		static G4double BellowOutRadius;
		static G4double SecA1InRadius;
		static G4double SecA1OutRadius;
		static G4double SecA2InRadius;
		static G4double SecA2OutRadius;
		static G4double SecA3InRadius;
		static G4double SecA3OutRadius;
		static G4double SecA1zLen;
		static G4double SecA2zLen;
		static G4double SecA3zLen;
		static G4double Flange1Radius;
		static G4double Flange2Radius;
		static G4double Flange3Radius;
		static G4double GateX;
		static G4double GateY;
		static G4double GateZ;
		static G4double downLen;
	private: 
		static G4double calculateDownBeamLineLen();

		G4LogicalVolume *BeamlineDownLV;
		G4LogicalVolume *BeamlineDownInnerLV;
		G4LogicalVolume *SectionALV, *SecA1InLV, *SecA2InLV, *SecA3InLV;
};

#endif

