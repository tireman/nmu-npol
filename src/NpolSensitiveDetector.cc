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

#include "G4SDManager.hh"

#include "NpolSensitiveDetector.hh"

NpolSensitiveDetector::NpolSensitiveDetector(G4String name)
	:G4VSensitiveDetector(name),
	collectionID(-1) {

		G4SDManager *SDMan = G4SDManager::GetSDMpointer();

		collectionName.insert("collection_name");
		SDMan->AddNewDetector(this);
	}

NpolSensitiveDetector::~NpolSensitiveDetector() {}

//Initialize is invoked at the beginning of each event.
void NpolSensitiveDetector::Initialize(G4HCofThisEvent *HCE) {

	if(collectionID < 0) collectionID = GetCollectionID(0);

	hitsCollection = new NpolHitsCollection(SensitiveDetectorName,collectionName[0]);
	HCE->AddHitsCollection(collectionID,hitsCollection);
}

//EndOfEvent is invoked at the end of each event.
void NpolSensitiveDetector::EndOfEvent(G4HCofThisEvent *HCE) {;}

//ProcessHits is invoked every time a step occurs in the sensitive volume.
G4bool NpolSensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist) {

	NpolHit *aHit = new NpolHit(aStep);
	//...

	hitsCollection->insert(aHit);
	return true;  //G4bool return value is currently unused.
}

