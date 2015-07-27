//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************

//%% NpolEventAction.cc %% 

// This is were the event level actions are placed
// Created: Daniel Wilbern November 2014
// Modified: William Tireman December 2014

#include <cstring>

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4RunManager.hh"
#include "G4ios.hh"
#include "G4String.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "NpolAnalysisManager.hh"
#include "NpolEventAction.hh"

NpolEventAction::NpolEventAction()
{
  // set printing per each 10 events default (can be changed in macro)
  G4RunManager::GetRunManager()->SetPrintProgress(10);     
}

NpolEventAction::~NpolEventAction()
{}

void NpolEventAction::BeginOfEventAction(const G4Event* evt) {
  
  NpolAnalysisManager *analysisMan = NpolAnalysisManager::GetInstance();
  
  analysisMan->PrepareNewEvent(evt->GetEventID());

}

void NpolEventAction::EndOfEventAction(const G4Event* evt) {
  
  NpolAnalysisManager *analysisMan = NpolAnalysisManager::GetInstance();
  
  analysisMan->FillHistograms();
}

