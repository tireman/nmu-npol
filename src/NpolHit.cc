   //********************************************************************
   //* License and Disclaimer: From GEANT Collaboration                 *
   //*                                                                  *
   //* The  Geant4 software  is  copyright of the Copyright Holders  of *
   //* the Geant4 Collaboration.  It is provided  under  the terms  and *
   //* conditions of the Geant4 Software License,  included in the file *
   //* LICENSE and available at  http://cern.ch/geant4/license .  These *
   //* include a list of copyright holders.     		      	*
   //********************************************************************
   //* The Geant4 software is used by the Northern Michigan University  *
   //* in accordance to the Geant4 software license specified above.    *
   //* The NMU Collaboration does not hold any rights to this software  *
   //* and gives full permission for its use to others under the limits *
   //* imposed by the GEANT4 Collaboration.  The NMU Collaboration      *
   //* gives no express or implied warranty and use of our code is at   *
   //* the users discretion only.  		    			*
   //********************************************************************


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

G4ThreadLocal G4Allocator<NpolHit>* NpolHitAllocator;

NpolHit::NpolHit(G4Step *aStep) {

	G4Track *aTrack = aStep->GetTrack();

	prepos = aStep->GetPreStepPoint()->GetPosition();
	postpos = aStep->GetPostStepPoint()->GetPosition();

	totalEnergyDeposit = aStep->GetTotalEnergyDeposit();

	volumeName = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName();
	processName = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();

	TrackID = aTrack->GetTrackID();
	ParentID = aTrack->GetParentID();
	stepNumber = aTrack->GetCurrentStepNumber();
	particleName = aTrack->GetParticleDefinition()->GetParticleName();
	ParticleMomentum = aTrack->GetMomentum();
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

