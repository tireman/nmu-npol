//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************

// %% NpolLeadCurtain.hh file %% //

// Created: William Tireman, May 2017

#ifndef Npol_LeadCurtain_h
#define Npol_LeadCurtain_h

#include "NpolDetectorFactory.hh"

class G4LogicalVolume;
class G4AssemblyVolume;
class G4VPhysicalVolume;

class NpolLeadCurtain : public NpolDetectorFactory {
  
public:
  NpolLeadCurtain();
  ~NpolLeadCurtain();

  static G4double PosLead, leadThickness;

  void ConstructLeadCurtain();
  
  virtual G4String GetName();
  virtual void Place(G4LogicalVolume *motherLV);
  
private: 
  G4LogicalVolume *LeadCurtainLV;

};

#endif
