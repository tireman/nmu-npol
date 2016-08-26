//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************

// && NpoleTarget Header file %% //

// Created: William Tireman - December 2014

#ifndef Npol_Target_h
#define Npol_Target_h

#include "NpolDetectorFactory.hh"

class G4LogicalVolume;
class G4AssemblyVolume;
class G4VPhysicalVolume;

class NpolTarget : public NpolDetectorFactory {
  
public:
  NpolTarget();
  ~NpolTarget();

  void ConstructTargetCan();
  void ConstructTargetFluid();
  
  virtual G4String GetName();
  virtual void Place(G4LogicalVolume *motherLV);
  
  static G4double TargetCellDiameter;
  static G4double TargetCellLength;
  static G4double TargetCellUpStreamWindow;
  static G4double TargetCellDownStreamWindow;
  static G4double TargetCellSideWindow;
  
private: 
  G4LogicalVolume *TargetCanLV;
  G4LogicalVolume *TargetFluidLV;
};

#endif

