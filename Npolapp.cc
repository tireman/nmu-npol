// $Id: Npolapp.cc version 1.0 January 03, 2014 $
// --------------------------------------------------------------
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// ********************************************************************


#include "G4RunManager.hh"
#include "G4UImanager.hh"

#include "QGSP_BERT.hh"
#include "PrimaryGeneratorAction.hh"

#include "DetectorConstruction.hh"
#include "EventAction.hh"
#include "TrackingAction.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

int main(int argc,char** argv)
{
		// RunManager construction
		G4RunManager* runManager = new G4RunManager;

#ifdef G4VIS_USE
		// Visualization manager construction
		G4VisManager* visManager = new G4VisExecutive;
		visManager->Initialize();
#endif

		// mandatory user initialization classes
		DetectorConstruction* detector = new DetectorConstruction;
		runManager->SetUserInitialization(detector);

		G4VUserPhysicsList* physics = new QGSP_BERT;
		runManager->SetUserInitialization(physics);

		// initialize Geant4 kernel
		runManager->Initialize();

		// mandatory user action class
		runManager->SetUserAction(new PrimaryGeneratorAction);
		//  runManager->SetUserAction(new ExN02PrimaryGeneratorAction(detector));


		// optional user action classes
		//  runManager->SetUserAction(new A01EventAction);
		runManager->SetUserAction(new EventAction);


		if(argc>1)
				// execute an argument macro file if exist
		{
				G4UImanager* UImanager = G4UImanager::GetUIpointer();
				G4String command = "/control/execute ";
				G4String fileName = argv[1];
				UImanager->ApplyCommand(command+fileName);
		}
		else
				// start interactive session
		{
#ifdef G4UI_USE
				G4UIExecutive* ui = new G4UIExecutive(argc, argv);
				ui->SessionStart();
				delete ui;
#endif
		}

		// Pause the program so I can look at the visual before it closes
		G4cout << "Press Return to continue" << G4endl;
		G4cin.get();

#ifdef G4VIS_USE
		delete visManager;
#endif

		delete runManager;

		return 0;
}

