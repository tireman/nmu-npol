//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#include "G4Threading.hh"
#else
#include "G4RunManager.hh"
#endif
#include <time.h>
#include "G4UImanager.hh"
#include "Randomize.hh"

#include "QGSP_BERT.hh"
#include "QGSP_BERT_HP.hh"
#include "QGSP_INCLXX_HP.hh"
#include "NpolPhysicsList.hh"

#include "NpolActionInitialization.hh"
#include "NpolDetectorConstruction.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif


int main(int argc,char *argv[]) {

  // Choose the Random engine
  //
  G4Random::setTheEngine(new CLHEP::RanecuEngine);
  //set random seed with system time
  //
 
  G4long seeds[2];
  G4long systime = time(NULL);
  if(getenv("JOBNUMBER"))
	systime += atoi(getenv("JOBNUMBER"));
  seeds[0] = (long) systime;
  seeds[1] = (long) (systime*G4UniformRand());
  G4int index = (int) 215*G4UniformRand();
  G4Random::setTheSeeds(seeds, index);
  G4cout << " The Index is = " << index << G4endl;
  G4cout << " Random seed = " << seeds[0] << "    " << seeds[1] <<G4endl;
 
  // RunManager construction
#ifdef G4MULTITHREADED
  G4RunManager *runManager = new G4RunManager;
  //G4MTRunManager *runManager = new G4MTRunManager;
  //runManager->SetNumberOfThreads(4);
#else
  G4RunManager *runManager = new G4RunManager;
#endif

#ifdef G4VIS_USE
  // Visualization manager construction
  G4VisManager *visManager = new G4VisExecutive;
  visManager->Initialize();
#endif
  
  // mandatory user initialization classes
  runManager->SetUserInitialization(new NpolDetectorConstruction);
  //runManager->SetUserInitialization(new NpolPhysicsList);
  runManager->SetUserInitialization(new QGSP_BERT); 
  //runManager->SetUserInitialization(new QGSP_INCLXX_HP);
            // Use _HP version for full runs and non-_HP version for tests
  runManager->SetUserInitialization(new NpolActionInitialization);
  
  // initialize Geant4 kernel
  runManager->Initialize();
  
  // Get the pointer to the User Interface manager
  G4UImanager *UImanager = G4UImanager::GetUIpointer();
 
  if(argc != 1) {
    // batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command + fileName);
    
  } else {
    // interactive mode
#ifdef G4UI_USE
    G4UIExecutive *ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
    UImanager->ApplyCommand("/control/execute macros/init_vis.mac");
#else
    UImanager->ApplyCommand("/control/execute macros/init.mac");
#endif
    ui->SessionStart();
    delete ui;
#endif
  }
  
#ifdef G4VIS_USE
  delete visManager;
#endif
  delete runManager;
  
  return 0;
}

