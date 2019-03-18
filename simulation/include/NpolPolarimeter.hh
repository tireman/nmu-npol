//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.							 *
//********************************************************************

// Header file for the Polarimeter constructor

// Daniel Wilbern December/January 2014/15

#ifndef Npol_Polarimeter_h
#define Npol_Polarimeter_h

#include "G4SystemOfUnits.hh"

#include "NpolDetectorFactory.hh"

class G4LogicalVolume;
class G4AssemblyVolume;
class G4VPhsicalVolume;

class NpolPolarimeter : public NpolDetectorFactory {
  
public:
  NpolPolarimeter();
  ~NpolPolarimeter();

  static G4double NpolAng;
  static G4double NDetThickness;
  static G4double NVetoThickness;
  static G4double NDetHeight;
  static G4double NDetStandardLength;
  static G4double dEArrayVertOffset;
  static G4double EarrayRotAngle;
  static G4double FirstAnalyzerLayerPos;
  static G4double FrontDetSpacing;
  static G4double FrontVetoOffset;
  static G4double BackTaggerzPos;
   
  void ConstructTopDetArray(G4LogicalVolume *motherLV);
  void ConstructTopVetoArray(G4LogicalVolume *motherLV);
  void ConstructBottomDetArray(G4LogicalVolume *motherLV);
  void ConstructBottomVetoArray(G4LogicalVolume *motherLV);
  void ConstructFrontDetArray(G4LogicalVolume *motherLV);
  void ConstructFrontVetoArray(G4LogicalVolume *motherLV);
  void ConstructBackTagArray(G4LogicalVolume *motherLV);

  virtual G4String GetName();
  virtual void Place(G4LogicalVolume *motherLV);
  void TranslateRotateAndPlace(G4LogicalVolume *polarimeterLV, G4LogicalVolume *motherLV,G4double rho, G4double phi, G4double z);
  
  G4AssemblyVolume *MakePlate(G4LogicalVolume *detLV, G4int numDets, G4double TmX, G4double TmY, G4double TmZ, G4double TmdX, G4double TmdY, G4double TmdZ);
  G4AssemblyVolume *MakePlate(G4LogicalVolume *detLV, G4int numDets, G4double TmX, G4double TmY, G4double TmZ, G4double TmdX, G4double TmdY, G4double TmdZ,G4RotationMatrix Rm);
  
  void ImprintPlate(G4AssemblyVolume *plate, G4LogicalVolume *motherLV, G4double TmX, G4double TmY, G4double TmZ, G4double RmZ);
  void ImprintPlate(G4AssemblyVolume *plate, G4LogicalVolume *motherLV,	G4ThreeVector Tm, G4RotationMatrix Rm);

private:
  G4LogicalVolume *PolarimeterLV;
};

#endif

