
//Daniel Wilbern, dwilbern@nmu.edu

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4StepPoint.hh"
#include "G4VProcess.hh"
#include "G4ios.hh"

#ifdef G4VIS_USE
#include "G4VVisManager.hh"
#include "G4VisAttributes.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#endif

#include "NpolHit.hh"
#include "NpolAnalysisManager.hh"

G4Allocator<NpolHit> NpolHitAllocator;

//NpolHit::NpolHit()
//{pLogV=NULL;}

NpolHit::NpolHit(G4Step *aStep): G4VHit() {
  G4Track *aTrack = aStep->GetTrack();
  prepos = aStep->GetPreStepPoint()->GetPosition();
  postpos = aStep->GetPostStepPoint()->GetPosition();
  totalEnergyDeposit = aStep->GetTotalEnergyDeposit(); 
  vertexEnergy = aTrack->GetVertexKineticEnergy();
  kineticEnergy = aTrack->GetKineticEnergy();
  volumeName = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName();
  volumePointer = aStep->GetPreStepPoint()->GetPhysicalVolume();
  processName = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
  TrackID = aTrack->GetTrackID();
  ParentID = aTrack->GetParentID();
  stepNumber = aTrack->GetCurrentStepNumber();
  particleName = aTrack->GetParticleDefinition()->GetParticleName();
  ParticleMomentum = aTrack->GetMomentum();
  G4int particleID = -1;
  if(particleName == "e-") particleID = ELECTRON_ID;
  else if(particleName == "e+") particleID = POSITRON_ID;
  else if(particleName == "proton") particleID = PROTON_ID;
  else if(particleName == "neutron") particleID = NEUTRON_ID;
  else if(particleName == "gamma") particleID = GAMMA_ID;
  else if(particleName == "pi-") particleID = PINEG;
  else if(particleName == "pi+") particleID = PIPOS;
  else if(particleName == "pi0") particleID = PINEUTRAL;
  //  FilePrint();

}

NpolHit::~NpolHit() {}

void NpolHit::Draw() {
#ifdef G4VIS_USE
  G4VVisManager *pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager) {
    G4Circle preCircle(prepos);
    preCircle.SetScreenSize(3.0);
    preCircle.SetFillStyle(G4Circle::filled);
    G4VisAttributes preAttribs = G4VisAttributes(G4Color::White());
    preCircle.SetVisAttributes(&preAttribs);
    G4Circle postCircle(postpos);
    postCircle.SetScreenSize(3.0);
    postCircle.SetFillStyle(G4Circle::filled);
    G4VisAttributes postAttribs = G4VisAttributes(G4Color::Yellow());
    postCircle.SetVisAttributes(&postAttribs);
    pVVisManager->Draw(preCircle);
    pVVisManager->Draw(postCircle);
  }
#endif
}

void NpolHit::Print() {
  G4cout << "Track ID: " << TrackID << ", Parent ID: " << ParentID << " Step number: " << stepNumber << G4endl;
  G4cout << "Particle Name: " << particleName << G4endl;
  G4cout << "Total Energy Deposited: " << totalEnergyDeposit << G4endl;
  G4cout << "Volume Name: " << volumeName << G4endl;
  G4cout << "Process Name: " << processName << G4endl << G4endl;
}
void NpolHit::FilePrint() {
  FILE *out = fopen("hitoutput.txt","a");
  fprintf(out,"%d %d %d %f %s %s %s\n",TrackID,ParentID,stepNumber,totalEnergyDeposit,particleName.data(),volumeName.data(),processName.data());
  fclose(out);
}
