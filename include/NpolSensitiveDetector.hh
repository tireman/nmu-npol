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

