//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//******************************************************************

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
}

