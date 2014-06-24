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

#ifndef Npol_SensitiveDetector_h
#define Npol_SensitiveDetector_h

#include "G4VSensitiveDetector.hh"
#include "G4THitsCollection.hh"
#include "G4Step.hh"
#include "G4TouchableHistory.hh"

#include "NpolHit.hh"

typedef G4THitsCollection<NpolHit> NpolHitsCollection;

class NpolSensitiveDetector : public G4VSensitiveDetector {

	private:
		G4int collectionID;
		NpolHitsCollection *hitsCollection;

	public:
		NpolSensitiveDetector(G4String name);
		virtual ~NpolSensitiveDetector();
		virtual void Initialize(G4HCofThisEvent *HCE);
		virtual void EndOfEvent(G4HCofThisEvent *HCE);

		virtual G4bool ProcessHits(G4Step *aStep, G4TouchableHistory *ROHist);
};

#endif

