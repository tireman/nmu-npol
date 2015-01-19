// %% NpolMaterials.cc  %%

// Npol materials are created in here
// Created: Daniel Wilbern - November 2014
// Modified: William Tireman - December 2014

#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"
#include "G4Isotope.hh"
#include "G4Material.hh"
#include "G4ios.hh"
#include "NpolMaterials.hh"

NpolMaterials *pInstance = NULL;

NpolMaterials::NpolMaterials() {
	nistMan = G4NistManager::Instance();

	CreateMaterials();
	G4cout << G4endl << "The materials defined are: " << G4endl << G4endl;
	G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

NpolMaterials::~NpolMaterials() {
	delete Vacuum;
	delete Air;
	delete Scint;
	delete Al;
	delete LH2;
	delete LD2;
	delete Concrete;
	delete Fe;
	delete SSteel;
	delete Cu;
}

NpolMaterials *NpolMaterials::GetInstance() {

	if(pInstance == NULL)
		pInstance = new NpolMaterials();

	return pInstance;
}

G4Material *NpolMaterials::GetMaterial(const G4String material) {

	G4Material *mat = nistMan->FindOrBuildMaterial(material);

	if(mat == NULL)
		mat = G4Material::GetMaterial(material);

	if(mat == NULL)
		G4cout << "Material " << material << " not found." << G4endl;

	return mat;
}

void NpolMaterials::CreateMaterials() {

	Vacuum = CreateVacuum();
	Air = CreateAir();
	Scint = CreateScint();
	Al = CreateAl();
	LH2 = CreateLH2();
	LD2 = CreateLD2();
	Concrete = CreateConcrete();
	Fe = CreateFe();
	SSteel = CreateSSteel();
	Cu = CreateCu();
}

G4Material *NpolMaterials::CreateVacuum() {
	return nistMan->FindOrBuildMaterial("G4_Galactic");
}

G4Material *NpolMaterials::CreateAir() {
	return nistMan->FindOrBuildMaterial("G4_AIR");
}

G4Material *NpolMaterials::CreateAl(){
  return nistMan->FindOrBuildMaterial("G4_Al");
}

G4Material *NpolMaterials::CreateScint() {

	G4Element* H = new G4Element("Hydrogen", "H", 1., 1.00794*g/mole);
	G4Element* C = new G4Element("Carbon"  , "C", 6., 12.011*g/mole);

	G4Material* scint = new G4Material("Scint", 1.02*g/cm3, 2);
	scint->AddElement(H, 8.451*perCent);
	scint->AddElement(C, 91.549*perCent);

	return scint;
}

G4Material *NpolMaterials::CreateLH2() {
  
  // liquid hydrogen
  
  // Hydrogen element
  G4Element* H = new G4Element("Hydrogen", "H", 1., 1.00794*g/mole);
  
  // Liquid Hydrogen
  LH2 = new G4Material("LH2", 0.07085*g/cm3, 1, kStateLiquid, 15.0*kelvin);
  LH2->AddElement(H, 2);

  return LH2;
}

G4Material *NpolMaterials::CreateLD2() {
// liquid deuterium
  G4int Z, N;
  G4double a;
  // Deuteron isotope
  G4Isotope* deuteron = new G4Isotope("deuteron", Z=1, N=2, a=2.0141018*g/mole);
 
  // Deuterium element
  G4Element* deuterium = new G4Element("deuterium", "deuterium", 1);
  deuterium->AddIsotope(deuteron, 1);
 
  // Liquid Deuterium
  LD2 = new G4Material("LD2", 0.169*g/cm3, 1, kStateLiquid, 22.0*kelvin);
  LD2->AddElement(deuterium, 2);

  return LD2;
}

G4Material *NpolMaterials::CreateConcrete() {

  return nistMan->FindOrBuildMaterial("G4_CONCRETE");
}

G4Material *NpolMaterials::CreateFe() {
  
  return nistMan->FindOrBuildMaterial("G4_Fe");
}

G4Material *NpolMaterials::CreateSSteel() {

  return nistMan->FindOrBuildMaterial("G4_STAINLESS-STEEL");
}

G4Material *NpolMaterials::CreateCu() {
 
  return nistMan->FindOrBuildMaterial("G4_Cu");
}
