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
#include <string>
#include "G4Event.hh"
#include "G4RunManager.hh"

#include "NpolEventAction.hh"
#include "NpolAnalysisManager.hh"

NpolEventAction::NpolEventAction()
{
  // set printing per each 10 events default (can be changed in macro)
  G4RunManager::GetRunManager()->SetPrintProgress(10);   
}

NpolEventAction::~NpolEventAction()
{}

void NpolEventAction::BeginOfEventAction(const G4Event* evt) {
  NpolAnalysisManager *analysisMan = NpolAnalysisManager::GetInstance();
  analysisMan->PrepareNewEvent();
  
  G4int evtID = evt->GetEventID();
  
  if(evtID == 0) ROOTFileNumber = "1";
  if((evtID%250000 == 0) && (evtID != 0)){
    analysisMan->CloseROOTChainFile();
    analysisMan->ClearROOT();
    ROOTFileNumber = sum(ROOTFileNumber, "1");
    analysisMan->SetROOTFileNumber(ROOTFileNumber);
    analysisMan->OpenFile();
    analysisMan->Initialize();
    analysisMan->PrepareNewEvent();
    }
}

void NpolEventAction::EndOfEventAction(const G4Event* evt) {
  NpolAnalysisManager *analysisMan = NpolAnalysisManager::GetInstance();
  analysisMan->FillTree();
}

G4String NpolEventAction::sum (G4String a, G4String b) {

  while (a.length() < b.length()) a = "0" + a;
  while (b.length() < a.length()) b = "0" + b;
  a = "0"+a;
  b = "0"+b;
  G4String sum = "";
  G4int carry = 0;
  for (G4int i=a.length()-1; i >= 0; i--) {
    G4int ac = a[i] - '0'; //convert from char to int
    G4int bc = b[i] - '0';
    G4int ss = ac + bc + carry;
    carry = ss / 10;
    ss = ss % 10; //remainder when I divide by 10
    sum = ((char)(ss + '0')) + sum;
  }
  return remleadzero (sum);
}

G4String NpolEventAction::remleadzero(G4String a) {
  
  while (a.length() >= 2 && a[0]=='0') a = a.substr(1);
  return a;
}
