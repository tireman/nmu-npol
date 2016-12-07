//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************

// %% NpolShieldHut.hh file %% //

// Created: William Tireman, January 2015

#ifndef Npol_ShieldHut_h
#define Npol_ShieldHut_h

#include "NpolDetectorFactory.hh"

class G4LogicalVolume;
class G4AssemblyVolume;
class G4VPhysicalVolume;

class NpolShieldHut : public NpolDetectorFactory {
  
public:
  NpolShieldHut();
  ~NpolShieldHut();

  static G4double NpolAng;
  static G4double vertAngle, horAngle;
  static G4double PosFront, PosLead, PosTagger;
  static G4double leadThickness;
  
  void ConstructHutSideWall();
  void ConstructHutFrontWall();
  void ConstructHutBackWall();
  void ConstructHutRoof();
  void ConstructLeadCurtain();
  void ConstructNPOLTagger();
  
  virtual G4String GetName();
  virtual void Place(G4LogicalVolume *motherLV);
  
private: 
  G4LogicalVolume *HutSideWallLV, *HutFrontWallLV;
  G4LogicalVolume *HutBackWallLV, *HutRoofLV;
  G4LogicalVolume *LeadCurtainLV, *NPOLTaggerLV;

};

#endif

