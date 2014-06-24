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

//Daniel Wilbern, dwilbern@nmu.edu

#ifndef Npol_Hit_h
#define Npol_Hit_h

#include "G4VHit.hh"
#include "G4Step.hh"
#include "G4Point3D.hh"

class NpolHit : public G4VHit {

	private:	
		G4double totalEnergyDeposit;
		G4Point3D prepos, postpos;
		//From pre-step point:
		G4String volumeName;
		G4String processName;
		//From track:
		G4int TrackID;
		G4int ParentID;
		G4int stepNumber;
		G4String particleName;

	public:
		NpolHit(G4Step *aStep);
		~NpolHit();
		virtual void Draw();
		virtual void Print();
		void FilePrint();

		//Get functions
	public:
		G4int GetTrackID() {return TrackID;}
		G4int GetParentID() {return ParentID;}
		G4int GetStepNumber() {return stepNumber;}
		G4double GetTotalEnergyDeposit() {return totalEnergyDeposit;}
		G4String GetVolumeName() {return volumeName;}
		G4String GetProcessName() {return processName;}
		G4String GetParticleName() {return particleName;}
		G4Point3D GetPreStepPos() {return prepos;}
		G4Point3D GetPostStepPos() {return postpos;}
};

#endif

