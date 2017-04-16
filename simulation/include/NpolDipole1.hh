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
#include "G4UniformMagField.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4ChordFinder.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4PropagatorInField.hh"

#include "NpolDetectorFactory.hh"

class G4LogicalVolume;
class G4AssemblyVolume;
class G4VPhysicalVolume;

class NpolDipole1 : public NpolDetectorFactory {
  
public:
  NpolDipole1();
  ~NpolDipole1();
  
  void ConstructDipole1Yoke();
  void ConstructDipole1CuBar();
  void ConstructDipole1CuEnd();
  void ConstructDipole1FieldClamp1();
  void ConstructDipole1FieldClamp2();
  void ConstructDipole1Field();
  
  virtual G4String GetName();
  virtual void Place(G4LogicalVolume *motherLV);
  virtual void ConstructSDandField();
 
  static G4double yokeLength;
  static G4double gapWidth;
  static G4double gapHeight;
  static G4double gapLength;
  static G4double fieldClampHeight;
  static G4double fieldClampWidth;
  static G4double fieldClampThick;
  static G4double fieldClampInwidth;
  static G4double fieldClampInheight;
  static G4double yokeGap;
  static G4double dipole1FieldY;
  static G4double NpolAng;
  static G4double FM;
private: 
  G4LogicalVolume *Dipole1YokeLV, *Dipole1CuBarLV, *Dipole1CuEndLV;
  G4LogicalVolume *FieldClamp1LV, *FieldClamp2LV, * Dipole1FieldLV;
};

#endif

