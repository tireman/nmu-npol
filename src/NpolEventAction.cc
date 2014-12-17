//%% NpolEventAction.cc %% 

// This is were the event level actions are placed
// Created: Daniel Wilber November 2014
// Modified: William Tireman December 2014

#include <cstring>

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4ios.hh"
#include "G4String.hh"
#include "G4THitsCollection.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4AutoLock.hh"

#include "NpolEventAction.hh"
#include "NpolAnalysis.hh"

typedef G4THitsCollection<NpolHit> NpolHitsCollection;

class NpolHit {};

G4Mutex aMutex = G4MUTEX_INITIALIZER; // Have to use a mutex for calls to strtok()

NpolEventAction::NpolEventAction()
{
  // set printing per each 10 events
  G4RunManager::GetRunManager()->SetPrintProgress(10);     
}

NpolEventAction::~NpolEventAction()
{}

void NpolEventAction::BeginOfEventAction(const G4Event* evt) {
	memset(Edep, 0, sizeof(Edep));
	memset(EdepdEoverE, 0, sizeof(EdepdEoverE));
}

void NpolEventAction::EndOfEventAction(const G4Event* evt) {

  /*	  G4int event_id = evt->GetEventID();
		G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
		G4SDManager *SDMan = G4SDManager::GetSDMpointer();
		G4HCofThisEvent *HCE = evt->GetHCofThisEvent();
		int i, dEoverEflag = 0;

		for(i=0; i < numSensitiveDetectors; i++)
		ProcessHitsInASensitiveDetector(HCE, CHCIDs[i]);

		for(i=1; i <= NUM_DETECTORS; i++)
		if(Edep[i] != 0.0)
		analysisManager->FillH1(i,Edep[i]);

		if(EdepdEoverE[0] >= 2.0 && EdepdEoverE[1] >= 1.0 && EdepdEoverE[4] >= 1.0 && EdepdEoverE[5] < 1.0) {
		dEoverEflag = 1;
		analysisManager->FillH2(1, EdepdEoverE[0], EdepdEoverE[1]);
		}
		if(EdepdEoverE[2] >= 2.0 && EdepdEoverE[3] >= 1.0 && EdepdEoverE[4] >= 1.0 && EdepdEoverE[5] < 1.0) {
		dEoverEflag = 1;
		analysisManager->FillH2(2, EdepdEoverE[2], EdepdEoverE[3]);
		}
		if(dEoverEflag == 1)
		analysisManager->FillH2(3, EdepdEoverE[0] + EdepdEoverE[2], EdepdEoverE[1] + EdepdEoverE[3]);

	 */  
}

// Given a pointer to the G4HCofThisEvent, HCE, and a hit collection ID, CHCID, process the hit objects stored the NpolHitsCollection identified by CHCID
void NpolEventAction::ProcessHitsInASensitiveDetector(G4HCofThisEvent *HCE, int CHCID) {
/*
	NpolHitsCollection *hitsCollection = NULL;
	int i;
	int n_hits = 0;

	if(HCE != NULL) {
		hitsCollection = (NpolHitsCollection *)(HCE->GetHC(CHCID)); // Cast to custom hits collection type
	}

	if(hitsCollection != NULL) {

		n_hits = hitsCollection->entries();
		for(i = 0; i < n_hits; i++) {
			NpolHit *aHit = (*hitsCollection)[i];
			char *volname = (char *)calloc(strlen("BottomVetoLV")+1,sizeof(char));

			int *detectorInfo = ParseAssemblyVolumeName(aHit->GetVolumeName().data(), &volname); // have to cast from (const char *) to (char *)

			fillEdepArray(aHit, detectorInfo);
			filldEoverEArray(aHit, volname);
			free(volname);	
			
		}
	}
*/
}

// Get the correct historgram ID from a parsed assembly volume name and add the energy deposited value from aHit to the Edep array.
void NpolEventAction::fillEdepArray(NpolHit *aHit, int *detectorInfo) {
/*
	int histoID = GetOffsetFromAssemblyVolumeNumber(detectorInfo[0]) + GetOffsetFromImprintNumber(detectorInfo[0], detectorInfo[1]) + detectorInfo[2];
	Edep[histoID] += aHit->GetTotalEnergyDeposit();
	*/
}

// Fill the correct spot in the dEoverE array depending on the logical volume name of the detector
void NpolEventAction::filldEoverEArray(NpolHit *aHit, char *volname) {
/*
	if(strcmp(volname, "TopDetLV") == 0)
		EdepdEoverE[0] += aHit->GetTotalEnergyDeposit();
	else if(strcmp(volname, "TopVetoLV") == 0)
		EdepdEoverE[1] += aHit->GetTotalEnergyDeposit();
	else if(strcmp(volname, "BottomDetLV") == 0)
		EdepdEoverE[2] += aHit->GetTotalEnergyDeposit();
	else if(strcmp(volname,"BottomVetoLV") == 0)
		EdepdEoverE[3] += aHit->GetTotalEnergyDeposit();
	else if (strcmp(volname,"FrontDetLV") == 0)
		EdepdEoverE[4] += aHit->GetTotalEnergyDeposit();
	else if (strcmp(volname,"FrontTagLV") == 0)
		EdepdEoverE[5] += aHit->GetTotalEnergyDeposit();
		*/
}

// Parse an assembly volume's name and return an array containing the assembly volume number, imprint number, and volume number.
// Valid values for VolumeName are C strings containing an assembly volume name created by Geant4
// The string pointed to by LV_name is written to with the logical volume name of the detector, pass NULL if you aren't interested.
int *NpolEventAction::ParseAssemblyVolumeName(const char *VolumeName, char **LV_name) {

	static int detectorInfo[3];
	char volname[strlen("av_00_impr_0_BottomDetLV_pv_0")+1];

	memcpy(volname,VolumeName,sizeof(char)*strlen(VolumeName)); // Copy argument into disposable local string variable since strtok modifies the string it works on

	G4AutoLock l(&aMutex);

	// A typical name is av_10_impr_2_FrontDetLV_pv_5
	char *token = strtok(volname, "_"); // throw out the first token (av)
	token = strtok(NULL,"_");
	detectorInfo[0] = atoi(token);
	token = strtok(NULL,"_"); // throw out the third token (impr)		
	token = strtok(NULL,"_");
	detectorInfo[1] = atoi(token);
	token = strtok(NULL,"_");
	if(LV_name != NULL)
		strcpy(*LV_name, token);
	token = strtok(NULL,"_"); // thow out sixth token (pv)
	token = strtok(NULL,"_");
	l.unlock();
	detectorInfo[2] = atoi(token);

	return detectorInfo;
}

// Get the amount of detectors created in assembly volumes before assembly volume #avNumber was created
// Valid values for avNumber are 1-12
int NpolEventAction::GetOffsetFromAssemblyVolumeNumber(int avNumber) {

	int offset = 0;
	const int arraySizes[13] = {0,26,28,13,14,26,28,13,14,12,16,12,16};

	for(; avNumber > 0; avNumber--)
		offset += arraySizes[avNumber-1];

	return offset;
}

// Get the amount of detectors created imprints before imprint #imprNumber in a given assembly volume.
// This amount is zero for the first imprint in an assembly volume.
// Valid values for avNumber are 1-12, valid values for imprNumber are 1 or 2
int NpolEventAction::GetOffsetFromImprintNumber(int avNumber, int imprNumber) {

	if(imprNumber == 1)
		return 0;

	switch(avNumber) {

		case 1:
		case 5:
			return 13;
		case 2:
		case 6:
			return 14;
		case 9:
		case 11:
			return 6;
		case 10:
		case 12:
			return 8;
	}

	return 0; // to suppress compiler warning
}

