// %% NpolShieldHut.hh file %% //

// Created: William Tireman, January 2015

#ifndef Npol_ShieldHut_h
#define Npol_ShieldHut_h

#include "G4SystemOfUnits.hh"
#include "NpolSubDetector.hh"

class G4LogicalVolume;
class G4AssemblyVolume;
class G4VPhysicalVolume;

class NpolShieldHut : public NpolSubDetector {
  
public:
  NpolShieldHut();
  ~NpolShieldHut();
  
  void ConstructHutSideWall();
  void ConstructHutFrontWall();
  void ConstructHutBackWall();
  void ConstructHutRoof();
  void ConstructHutCollimator();
  virtual G4VPhysicalVolume *Construct(G4LogicalVolume *motherLV);
 
private: 
  G4LogicalVolume *HutSideWallLV;
  G4LogicalVolume *HutFrontWallLV;
  G4LogicalVolume *HutBackWallLV;
  G4LogicalVolume *HutRoofLV;
  G4LogicalVolume *HutCollimatorLV;
};

#endif

