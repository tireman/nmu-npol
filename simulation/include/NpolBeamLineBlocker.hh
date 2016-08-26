//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************

// %% NpolBeamLineBlocker.hh file %% //

// Created: William Tireman, July 2016

#ifndef Npol_BeamLineBlocker_h
#define Npol_BeamLineBlocker_h

#include "NpolDetectorFactory.hh"

class G4LogicalVolume;
class G4AssemblyVolume;
class G4VPhysicalVolume;

class NpolBeamLineBlocker : public NpolDetectorFactory {
  
public:
  NpolBeamLineBlocker();
  ~NpolBeamLineBlocker();
  
  void ConstructBeamLineBlocker();
  
  virtual G4String GetName();
  virtual void Place(G4LogicalVolume *motherLV);
  
  static G4double lenX;
  static G4double lenY;
  static G4double lenZ; 
  static G4double offsetX;
  static G4double offsetY;
  static G4double offsetZ;

private: 
  G4LogicalVolume *BeamLineBlockerLV;

};

#endif

