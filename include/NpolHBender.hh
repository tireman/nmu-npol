//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.							 *
//********************************************************************

// && NpolHBender Header file SHMS H bender magnet %% //

// Created: Daniel Wilbern - March 2015

#ifndef Npol_HBender_h
#define Npol_HBender_h

#include "G4SystemOfUnits.hh"

#include "NpolDetectorFactory.hh"

class G4LogicalVolume;
class G4AssemblyVolume;
class G4VPhysicalVolume;

class NpolHBender : public NpolDetectorFactory {
  
public:
  NpolHBender();
  ~NpolHBender();
  
  virtual G4String GetName();
  virtual void Place(G4LogicalVolume *motherLV);
  
private:
  G4LogicalVolume *HBenderLV;
  G4LogicalVolume *HBCryoBoxLV;
  G4LogicalVolume *HBBoreLogicLV;
  G4LogicalVolume *HBCoilLogicLV;
  G4LogicalVolume *HBYorkLogicLV;
};

#endif

