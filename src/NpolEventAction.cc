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
   //* The NMU Collaboration does not hold any rights to this software  *
   //* and gives full permission for its use to others under the limits *
   //* imposed by the GEANT4 Collaboration.  The NMU Collaboration      *
   //* gives no express or implied warranty and use of our code is at   *
   //* the users discretion only.  		    			*
   //********************************************************************

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4ios.hh"
#include "G4String.hh"
#include "G4THitsCollection.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "NpolEventAction.hh"
#include "NpolHit.hh"

typedef G4THitsCollection<NpolHit> NpolHitsCollection;

NpolEventAction::NpolEventAction()
{
  //G4double energy= GetParticleEnergy();
  //G4cout << "Energy is " << energy ;
  fp=fopen("/tmp/g4Npol1500.out","w");
}

NpolEventAction::~NpolEventAction()
{
  fclose(fp);
}

void NpolEventAction::BeginOfEventAction(const G4Event* evt)
{}

void NpolEventAction::EndOfEventAction(const G4Event* evt) {


	const int numSensitiveDetectors = 6;
	G4int event_id = evt->GetEventID();
	G4SDManager *SDMan = G4SDManager::GetSDMpointer();
	int i, n_hits = 0, CHCIDs[6] = { SDMan->GetCollectionID("TopDet/collection_name"),
		SDMan->GetCollectionID("TopVeto/collection_name"),
		SDMan->GetCollectionID("BottomDet/collection_name"),
		SDMan->GetCollectionID("BottomVeto/collection_name"),
		SDMan->GetCollectionID("FrontDet/collection_name"),
		SDMan->GetCollectionID("FrontTag/collection_name") };
	G4HCofThisEvent *HCE = evt->GetHCofThisEvent();

	for(i=0; i < numSensitiveDetectors; i++)
		n_hits += ProcessAndPrint(HCE,CHCIDs[i]);

	// periodic printing
	if (event_id < 100 || event_id%100 == 0) {
		G4cerr << ">>> Event " << event_id << G4endl;
		G4cout << "    " << n_hits 
			<< " hits stored in this event." << G4endl;
	}
}

//This method is called in a for loop and is intended to run once per event for each sensitive detector.  Returns the total number of hits in the detector.
int NpolEventAction::ProcessAndPrint(G4HCofThisEvent *HCE, int CHCID) {

  NpolHitsCollection *hitsCollection = NULL;
  int i, n_hits = 0;
  
  if(HCE != NULL)
    hitsCollection = (NpolHitsCollection *)(HCE->GetHC(CHCID)); //Cast to custom hits collection type
  
  G4double TotalEnergy = 0.0;
  G4int flag1 = 0;
  G4String A, B;
  
  if(hitsCollection != NULL) {
    n_hits = hitsCollection->entries();
    fprintf(fp,"Total number of hits is %4u .\n", n_hits);
    for(i = 0; i < n_hits; i++) {
      NpolHit *aHit = (*hitsCollection)[i];
      // ...
      //     aHit->Print();
      //aHit->FilePrint();


 // Tireman new code for testing 6/13/2014

      G4double Edeposit = (*hitsCollection)[i]->GetTotalEnergyDeposit()/MeV;
      TotalEnergy = TotalEnergy + Edeposit;
      G4int IDTrack = (*hitsCollection)[i]->GetTrackID();
      G4int IDParent = (*hitsCollection)[i]->GetParentID();
      G4ThreeVector PrePos = (*hitsCollection)[i]->GetPreStepPos()/cm;
      G4ThreeVector PostPos = (*hitsCollection)[i]->GetPostStepPos()/cm;
      G4String PartName = (*hitsCollection)[i]->GetParticleName();
      G4String VolName = (*hitsCollection)[i]->GetVolumeName();
      G4String ProcName = (*hitsCollection)[i]->GetProcessName();
      G4int StepNumber = (*hitsCollection)[i]->GetStepNumber();
      G4ThreeVector particleMom = (*hitsCollection)[i]->GetParticleMomentum()/MeV;
      //G4ThreeVector DeltaMom = (*hitsCollection)[i]->GetDeltaMomentum()/MeV;
      
      if(i == 0 && PartName == "neutron" && IDParent == 0 && IDTrack == 1 && ProcName == "hadElastic"){
	flag1 =1;
	for (int j=0; j< (int)VolName.length(); j++){ // Testing as part of a 
	  A = A+VolName[j];  // parsing routine for detector sorting.
	}
      }

      if(flag1 == 1){
	fprintf(fp,"%s %s %s %3i %3i %3i %10.4f %5.2f %5.2f %5.2f %5.2f %5.2f %5.2f %5.2f %5.2f %5.2f %7.2f\n",PartName.c_str(), VolName.c_str(), ProcName.c_str(), StepNumber, IDParent, IDTrack, Edeposit, PrePos.x(),PrePos.y(),PrePos.z(),PostPos.x(),PostPos.y(),PostPos.z(),particleMom.x(),particleMom.y(),particleMom.z(),TotalEnergy);
      }

      // end new code

    }
  }
  
  return n_hits;
}

