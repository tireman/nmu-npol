//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************

#ifndef Npol_TrackingAction_h
#define Npol_TrackingAction_h

#include "G4UserTrackingAction.hh"

class G4Track;

class NpolTrackingAction : public G4UserTrackingAction {

	public:
		NpolTrackingAction();
		~NpolTrackingAction();

	private:
		virtual void PreUserTrackingAction(const G4Track *);
		virtual void PostUserTrackingAction(const G4Track *);
};

#endif

