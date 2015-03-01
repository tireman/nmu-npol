// && NpoleDipole2 Header file BNL 48D48 dipole magnet

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
  void ConstructDipole2CuBar();
  void ConstructDipole2CuEnd();
  void ConstructDipole2FieldClamp();
  virtual G4VPhysicalVolume *Construct(G4LogicalVolume *motherLV);
 
private: 
  G4LogicalVolume *Dipole2YokeLV, *Dipole2CuBarLV, *Dipole2CuEndLV;
  G4LogicalVolume *FieldClampLV;
};

#endif

