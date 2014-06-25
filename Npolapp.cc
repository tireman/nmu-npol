   //********************************************************************
   //* License and Disclaimer: From GEANT Collaboration                 *
   //*                                                                  *
   //* The  Geant4 software  is  copyright of the Copyright Holders  of *
   //* the Geant4 Collaboration.  It is provided  under  the terms  and *
   //* conditions of the Geant4 Software License,  included in the file *
   //* LICENSE and available at  http://cern.ch/geant4/license .  These *
   //* include a list of copyright holders.     		      	*
   //********************************************************************
   //* The Geant4 software is used by the Northern Michigan University  *
   //* in accordance to the Geant4 software license specified above.    *
   //* The NMU software is not to be distributed in whole or modified   *
   //* form without including the Geant4 license.  Use of the NMU code  *
   //* for development of other code is permitted as long as the work   *
   //* is not a direct copy of the work being performed here. If it is  *
   //* such then we ask that you give credit to our work out of  	*
   //* professional courtesy and acknowledgment.  The NMU Collaboration *
   //* gives no express or implied warranty and use of our code is at   *
   //* the users discretion only.  		    			*
   //********************************************************************

#ifdef G4MULTITHREADED
 #include "G4MTRunManager.hh"
 #include "G4Threading.hh"
#else
#include "G4RunManager.hh"
 #endif

#include "G4UImanager.hh"

#include "QGSP_BERT.hh"

#include "NpolActionInitialization.hh"
#include "NpolDetectorConstruction.hh"

#ifdef G4VIS_USE
 #include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
 #include "G4UIExecutive.hh"
#endif


int main(int argc,char *argv[])
{

	// RunManager construction
#ifdef G4MULTITHREADED
  G4MTRunManager *runManager = new G4MTRunManager;
  //	G4RunManager *runManager = new G4RunManager;
  runManager->SetNumberOfThreads(2);
  //runManager->SetNumberOfThreads(G4Threading::G4GetNumberOfCores());
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
	runManager->SetUserInitialization(new QGSP_BERT);
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

		// Pause the program so I can look at the visual before it closes
		G4cout << "Press Return to continue" << G4endl;
		G4cin.get();
	} else {
		// interactive mode
#ifdef G4UI_USE
		G4UIExecutive *ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
		UImanager->ApplyCommand("/control/execute init_vis.mac");
#else
		UImanager->ApplyCommand("/control/execute init.mac");
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

