
// && NpolSHMSHeader file %% //

// Created: Daniel Wilbern - March 2015

#ifndef Npol_SHMS_h
#define Npol_SHMS_h

#include "G4SystemOfUnits.hh"

#include "NpolSubDetector.hh"

class G4LogicalVolume;
class G4AssemblyVolume;
class G4VPhysicalVolume;

class NpolSHMS : public NpolSubDetector {
  
public:
  NpolSHMS();
  ~NpolSHMS();
  
  virtual G4VPhysicalVolume *Construct(G4LogicalVolume *motherLV);
 
private: 
};

#endif

