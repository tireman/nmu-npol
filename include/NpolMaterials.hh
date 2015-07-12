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

class G4NistManager;
class G4Material;

class NpolMaterials {

public:
  static NpolMaterials *GetInstance();
  
  G4Material *GetVacuum() {return Vacuum;};
  G4Material *GetAir() {return Air;};
  G4Material *GetScint() {return Scint;};
  G4Material *GetAl() {return Al;};
  G4Material *GetLH2() {return LH2;};
  G4Material *GetLD2() {return LD2;};
  G4Material *GetConcrete() {return Concrete;};
  G4Material *GetFe() {return Fe;};
  G4Material *GetSSteel() {return SSteel;};
  G4Material *GetCu() {return Cu;};
  G4Material *GetPb() {return Pb;};
  
private:
  NpolMaterials();
  ~NpolMaterials();
  NpolMaterials(const NpolMaterials &);
  NpolMaterials & operator=(const NpolMaterials &);
  
  void CreateMaterials();
  G4Material *GetMaterial(const G4String material);
  
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
  
  G4Material *Vacuum;
  G4Material *Air;
  G4Material *Scint;
  G4Material *Al;
  G4Material *LH2;
  G4Material *LD2;
  G4Material *Concrete;
  G4Material *Fe;
  G4Material *SSteel;
  G4Material *Cu;
  G4Material *Pb;
};

#endif

