//Daniel Wilbern, dwilbern@nmu.edu

#include "G4VSensitiveDetector.hh"
#include "G4THitsCollection.hh"
#include "G4Step.hh"
#include "G4TouchableHistory.hh"


#include "NMUHit.hh"

typedef G4THitsCollection<NMUHit> NMUHitsCollection;

class NMUSensitiveDetector : public G4VSensitiveDetector {

	private:
		G4int collectionID;
		NMUHitsCollection *hitsCollection;

	public:
		NMUSensitiveDetector(G4String name);
		virtual ~NMUSensitiveDetector();
		virtual void Initialize(G4HCofThisEvent *HCE);
		virtual void EndOfEvent(G4HCofThisEvent *HCE);

		virtual G4bool ProcessHits(G4Step *aStep, G4TouchableHistory *ROHist);
};

