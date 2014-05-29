//Daniel Wilbern, dwilbern@nmu.edu

#ifndef TrackingAction_h
#define TrackingAction_h

#include "G4UserTrackingAction.hh"

class TrackingAction : public G4UserTrackingAction {

public:
	TrackingAction();
	~TrackingAction();

public:
	void PreUserTrackingAction(const G4Track *aTrack);
	void PostUserTrackingAction(const G4Track *aTrack);

};

#endif

