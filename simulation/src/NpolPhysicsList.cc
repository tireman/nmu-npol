//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************

// $Id: NpolPhysicsList.cc  2016-06-16 Will Tireman $
//
/// \file NpolPhysicsList.cc
/// \brief Implementation of the NpolPhysicsList class

#include "NpolPhysicsList.hh"

#include "G4ChargeExchangePhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmExtraPhysics.hh"
//#include "G4IonPhysics.hh"
#include "G4IonINCLXXPhysics.hh"
#include "G4StoppingPhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4HadronPhysicsQGSP_BERT_HP.hh"
#include "G4HadronPhysicsINCLXX.hh"
#include "G4NeutronTrackingCut.hh"

#include "G4DataQuestionaire.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NpolPhysicsList::NpolPhysicsList() 
: G4VModularPhysicsList(){
  SetVerboseLevel(1);

  // Set Bools for high energy models and low energy neutron transport
  bool withNeutronHP = true;
  bool withFTFP = false;

  // Setting cut value
  defaultCutValue = 0.7*CLHEP::mm;
  SetCutsWithDefault();

  // Hadron Elastic scattering
  RegisterPhysics(new G4HadronElasticPhysicsHP());

  // Hadron Physics
  //RegisterPhysics(new G4HadronPhysicsQGSP_BERT_HP());
  RegisterPhysics(new G4HadronPhysicsINCLXX(1,true,withNeutronHP,withFTFP));
  
  // Charge Exchange Physics
  RegisterPhysics(new G4ChargeExchangePhysics());

  // Synchroton Radiation & GN Physics
  RegisterPhysics(new G4EmExtraPhysics());

  // Decays
  RegisterPhysics(new G4DecayPhysics());

  // Ion Physics
  //RegisterPhysics(new G4IonPhysics());
  RegisterPhysics(new G4IonINCLXXPhysics());
  
  // Stopping Physics
  RegisterPhysics(new G4StoppingPhysics());

  // Radioactive decay
  //RegisterPhysics(new G4RadioactiveDecayPhysics());

  // EM physics
  RegisterPhysics(new G4EmStandardPhysics());

  // Neutron tracking cut
  RegisterPhysics(new G4NeutronTrackingCut());
 
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NpolPhysicsList::~NpolPhysicsList()
{ 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NpolPhysicsList::SetCuts()
{
  G4VUserPhysicsList::SetCuts();
}  
