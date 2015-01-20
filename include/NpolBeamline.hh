// %% NpolBeamline.hh %% 

// Created: William Tireman - December 2014
// Modified: Daniel Wilbern - December 2014

#ifndef Npol_Beamline_h
#define Npol_Beamline_h

#include "G4SystemOfUnits.hh"
#include "NpolSubDetector.hh"

class G4LogicalVolume;
class G4AssemblyVolume;
class G4VPhysicalVolume;

class NpolBeamline : public NpolSubDetector {
  
public:
  NpolBeamline();
  ~NpolBeamline();
  
  void ConstructBeamlineUpper();
  void ConstructBeamlineDown();
  void ConstructBeamlineUpperInner();
  void ConstructBeamlineDownInner();
  virtual G4VPhysicalVolume *Construct(G4LogicalVolume *motherLV);
 
private: 
  G4LogicalVolume *BeamlineUpperLV;
  G4LogicalVolume *BeamlineDownLV;
  G4LogicalVolume *BeamlineUpperInnerLV;
  G4LogicalVolume *BeamlineDownInnerLV;
};

#endif

