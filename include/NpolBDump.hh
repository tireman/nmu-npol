// && NpolBDump Header file %% //

// Created: William Tireman - March 2015

#ifndef Npol_BDump_h
#define Npol_BDump_h

#include "G4SystemOfUnits.hh"

#include "NpolSubDetector.hh"

class G4LogicalVolume;
class G4AssemblyVolume;
class G4VPhysicalVolume;

class NpolBDump : public NpolSubDetector {
  
public:
  NpolBDump();
  ~NpolBDump();
  
  virtual G4VPhysicalVolume *Construct(G4LogicalVolume *motherLV);
 
private: 
};

#endif

