//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************

#define NUM_DETECTORS 26+28+13+14+26+28+13+14+12+16+12+16

#ifndef Npol_EventAction_h
#define Npol_EventAction_h

#include "G4HCofThisEvent.hh"
#include "G4UserEventAction.hh"

class G4Event;
class NpolHit;

class NpolEventAction : public G4UserEventAction
{
	public:
		NpolEventAction();
		~NpolEventAction();

	public:
		void BeginOfEventAction(const G4Event*);
  void EndOfEventAction(const G4Event*);

	private:
		void ProcessHitsInASensitiveDetector(G4HCofThisEvent *HCE, int CHCID);
		int *ParseAssemblyVolumeName(const char *VolumeName, char **LV_name);
		int GetOffsetFromAssemblyVolumeNumber(int avNumber);
		int GetOffsetFromImprintNumber(int avNumber, int imprNumber);
		void fillEdepArray(NpolHit *aHit, int *detectorInfo);
		void filldEoverEArray(NpolHit *aHit, char *volname);

	private:
		// Edep array has one spot for each histogram created in RunAction's constructor
		// and one extra because the histograms start at ID 1 and not 0.
		// The number of histograms should be the same as the number of detectors in the setup.
		double Edep[NUM_DETECTORS+1];
		double EdepdEoverE[6];
};

#endif

