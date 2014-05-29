//Daniel Wilbern, dwilbern@nmu.edu

#include "G4VHit.hh"
#include "G4Step.hh"

class NMUHit : public G4VHit {

	private:	
		G4double totalEnergyDeposit;
		//From pre-step point:
		G4String volumeName;
		G4String processName;
		//From track:
		G4int TrackID;
		G4int ParentID;
		G4int stepNumber;
		G4String particleName;

	public:
		NMUHit(G4Step *aStep);
		~NMUHit();
		virtual void Draw();
		virtual void Print();
		void FilePrint();

	//Get functions
	public:
		G4int GetTrackID() {return TrackID;}
		G4int GetParentID() {return ParentID;}
		G4int GetStepNumber() {return stepNumber;}
		G4double GetTotalEnergyDeposit() {return totalEnergyDeposit;}
		G4String GetVolumeName() {return volumeName;}
		G4String GetProcessName() {return processName;}
		G4String GetParticleName() {return particleName;}
};

