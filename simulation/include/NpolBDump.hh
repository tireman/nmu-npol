// && NpolBDump Header file %% //

// Created: William Tireman - March 2015

#ifndef Npol_BDump_h
#define Npol_BDump_h

#include "G4SystemOfUnits.hh"

#include "NpolDetectorFactory.hh"

class G4LogicalVolume;
class G4AssemblyVolume;
class G4VPhysicalVolume;

class NpolBDump : public NpolDetectorFactory {
  
public:
  NpolBDump();
  ~NpolBDump();
  
  virtual G4String GetName();
  virtual void Place(G4LogicalVolume *motherLV);
  
private:
  G4LogicalVolume *BDumpLV;
};

#endif

