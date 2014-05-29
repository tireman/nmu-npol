//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id: EventAction.cc,v 1.11 2006/06/29 17:48:05 gunter Exp $
// GEANT4 tag $Name: geant4-09-03-patch-01 $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "EventAction.hh"
#include "NMUHit.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4THitsCollection.hh"
typedef G4THitsCollection<NMUHit> NMUHitsCollection;
#include "G4ios.hh"
#include "G4String.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction()
{
		fp=fopen("g4Npol7000.out","w");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{
		fclose(fp);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event* evt)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* evt) {

		G4int event_id = evt->GetEventID();
		G4SDManager *SDMan = G4SDManager::GetSDMpointer();
		int i, n_hits = 0, CHCIDs[6] = { SDMan->GetCollectionID("TopDet/collection_name"),
				SDMan->GetCollectionID("TopVeto/collection_name"),
				SDMan->GetCollectionID("BottomDet/collection_name"),
				SDMan->GetCollectionID("BottomVeto/collection_name"),
				SDMan->GetCollectionID("FrontDet/collection_name"),
				SDMan->GetCollectionID("FrontTag/collection_name") };
		G4HCofThisEvent *HCE = evt->GetHCofThisEvent();

		for(i=0; i < 6; i++)
				n_hits += ProcessAndPrint(HCE,CHCIDs[i]);

		// periodic printing
		if (event_id < 100 || event_id%100 == 0) {
				G4cerr << ">>> Event " << event_id << G4endl;
				G4cout << "    " << n_hits 
						<< " hits stored in this event." << G4endl;
		}
}

//This method is called in a for loop and is intended to run once per event for each sensitive detector.  Returns the total number of hits in the detector.
int EventAction::ProcessAndPrint(G4HCofThisEvent *HCE, int CHCID) {

		NMUHitsCollection *hitsCollection = NULL;
		int i, n_hits = 0;

		if(HCE != NULL)
				hitsCollection = (NMUHitsCollection *)(HCE->GetHC(CHCID)); //Cast to custom hits collection type

		if(hitsCollection != NULL) {
				n_hits = hitsCollection->entries();

				for(i = 0; i < n_hits; i++) {
						NMUHit *aHit = (*hitsCollection)[i];
						aHit->Print();
						aHit->FilePrint();
				}
		}

		return n_hits;
}

