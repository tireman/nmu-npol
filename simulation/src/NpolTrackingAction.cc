
#include "G4Track.hh"

#include "NpolTrackingAction.hh"
#include "NpolAnalysisManager.hh"

NpolTrackingAction::NpolTrackingAction() {}
NpolTrackingAction::~NpolTrackingAction() {}

void NpolTrackingAction::PreUserTrackingAction(const G4Track *aTrack) {
	NpolAnalysisManager *analysisMan = NpolAnalysisManager::GetInstance();
	analysisMan->RecordTrack(aTrack);
}

void NpolTrackingAction::PostUserTrackingAction(const G4Track *aTrack) {}

