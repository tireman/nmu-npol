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

#include "NpolRunAction.hh"

#include "G4Run.hh"

NpolRunAction::NpolRunAction()
{}

NpolRunAction::~NpolRunAction()
{}

void NpolRunAction::BeginOfRunAction(const G4Run* aRun)
{
	G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
}

void NpolRunAction::EndOfRunAction(const G4Run*)
{ }

