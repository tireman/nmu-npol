
// && NpolHBender Header file %% //

// Created: Daniel Wilbern - March 2015

#ifndef Npol_HBender_h
#define Npol_HBender_h

#include "G4SystemOfUnits.hh"

#include "NpolSubDetector.hh"

class G4LogicalVolume;
class G4AssemblyVolume;
class G4VPhysicalVolume;

class NpolHBender : public NpolSubDetector {
  
public:
  NpolHBender();
  ~NpolHBender();
  
  virtual G4VPhysicalVolume *Construct(G4LogicalVolume *motherLV);
 
private: 
};

#endif

