// && NpoleDipole1 Header file %% //

// Created: William Tireman - January 2015


#ifndef Npol_Dipole2_h
#define Npol_Dipole2_h

#include "G4SystemOfUnits.hh"

#include "NpolSubDetector.hh"

class G4LogicalVolume;
class G4AssemblyVolume;
class G4VPhysicalVolume;

class NpolDipole2 : public NpolSubDetector {
  
public:
  NpolDipole2();
  ~NpolDipole2();
  
  void ConstructDipole2Yoke();
  void ConstructDipole2Coil();
  virtual G4VPhysicalVolume *Construct(G4LogicalVolume *motherLV);
 
private: 
  G4LogicalVolume *Dipole2YokeLV, *CoilLV;
};

#endif

