//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.                             *
//********************************************************************

#ifndef Npol_Materials_h
#define Npol_Materials_h

#include <map>
#include <string>

class G4NistManager;
class G4Material;

class NpolMaterials {

public:
  static NpolMaterials *GetInstance();
  G4Material *GetMaterial(const G4String material);
  
private:
  NpolMaterials();
  ~NpolMaterials();
  NpolMaterials(const NpolMaterials &);
  NpolMaterials & operator=(const NpolMaterials &);
  
  void CreateMaterials();
  
  G4Material *CreateVacuum();
  G4Material *CreateAir();
  G4Material *CreateScint();
  G4Material *CreateAl();
  G4Material *CreateLH2();
  G4Material *CreateLD2();
  G4Material *CreateConcrete();
  G4Material *CreateFe();
  G4Material *CreateSSteel();
  G4Material *CreateCu();
  G4Material *CreatePb();

private:
  G4NistManager *nistMan;
  std::map<std::string,G4Material *> materials;
};

#endif

