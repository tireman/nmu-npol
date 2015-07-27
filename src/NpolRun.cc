// NpolRun.cc  Class for the run information for the ROOT tree
// Just frack'n loads of fun.
// Auther: William Tireman 7/22/2015
#include <cstdio>

#include "NpolRun.hh"
#include "G4SystemOfUnits.hh"
#include "G4Event.hh"
#include "NpolAnalysisManager.hh"

NpolRun::NpolRun()
{
  //G4SDManager* SDMan = G4SDManager::GetSDMpointer();

  G4cout << "NpolRun" << G4endl;

}

NpolRun::~NpolRun()
{
    G4cout << "~NpolRun" << G4endl;
}


void NpolRun::RecordEvent(const G4Event* evt)
{
  NpolAnalysisManager *analysisMan = NpolAnalysisManager::GetInstance();
 
  Int_t eventnum = evt->GetEventID();
  ntuple.vertex = evt->GetNumberOfPrimaryVertex();
  ntuple.event = eventnum;
  tree->Fill();
  
}
