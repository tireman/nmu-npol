// %% NpolSteppingAction.cc %% //

// Created: Daniel Wilbern - November 2014


#include "G4Step.hh"
#include "G4ios.hh"

#include "NpolDataStructure.hh"
#include "NpolSteppingAction.hh"

NpolSteppingAction::NpolSteppingAction() {}

NpolSteppingAction::~NpolSteppingAction() {}

void NpolSteppingAction::UserSteppingAction(const G4Step *aStep) {

	NpolDataStructure *dataStructure = NpolDataStructure::GetInstance();

	dataStructure->AddEDep(aStep->GetPreStepPoint()->GetPhysicalVolume(), aStep->GetTotalEnergyDeposit());

  //G4cout << "stepped" << G4endl;
}

