//Daniel Wilbern, dwilbern@nmu.edu

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4StepPoint.hh"
#include "G4VProcess.hh"
#include "G4ios.hh"

#include "NMUHit.hh"

NMUHit::NMUHit(G4Step *aStep) {

	G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
	G4Track *aTrack = aStep->GetTrack();

	totalEnergyDeposit = aStep->GetTotalEnergyDeposit();

	volumeName = preStepPoint->GetPhysicalVolume()->GetName();
	processName = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();

	TrackID = aTrack->GetTrackID();
	ParentID = aTrack->GetParentID();
	stepNumber = aTrack->GetCurrentStepNumber();
	particleName = aTrack->GetParticleDefinition()->GetParticleName();

}

NMUHit::~NMUHit() {}

void NMUHit::Draw() {}

void NMUHit::Print() {

	G4cout << "Track ID: " << TrackID << ", Parent ID: " << ParentID << " Step number: " << stepNumber << G4endl;
	G4cout << "Particle Name: " << particleName << G4endl;
	G4cout << "Total Energy Deposited: " << totalEnergyDeposit << G4endl;
	G4cout << "Volume Name: " << volumeName << G4endl;
	G4cout << "Process Name: " << processName << G4endl << G4endl;
}

void NMUHit::FilePrint() {

	FILE *out = fopen("hitoutput.txt","a");

	fprintf(out,"%d %d %d %f %s %s %s\n",TrackID,ParentID,stepNumber,totalEnergyDeposit,particleName.data(),volumeName.data(),processName.data());

	fclose(out);
}

