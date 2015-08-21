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

#ifndef Npol_BeamlineUpper_h
#define Npol_BeamlineUpper_h

#include "G4SystemOfUnits.hh"
#include "NpolDetectorFactory.hh"

class G4LogicalVolume;
class G4AssemblyVolume;
class G4VPhysicalVolume;

class NpolBeamlineUpper : public NpolDetectorFactory {
  
public:
  NpolBeamlineUpper();
  ~NpolBeamlineUpper();
  
  void ConstructBeamlineUpper();
  void ConstructBeamlineUpperInner();
  
  virtual G4String GetName();
  virtual void Place(G4LogicalVolume *motherLV);
  
  static G4double upperInnerRadius;
  static G4double upperOuterRadius;
  static G4double upperLen;
  
private: 
  static G4double calculateUpperBeamLineLen();
  
  G4LogicalVolume *BeamlineUpperLV;
  G4LogicalVolume *BeamlineUpperInnerLV;
};

#endif

