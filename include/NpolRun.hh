// NpolRun.hh
// Created for adding TTree output
// Authoer: William Tireman 7/22/2015

#ifndef NpolRun_h
#define NpolRun_h 1

#include "G4Run.hh"
#include "G4THitsMap.hh"

#ifdef G4ANALYSIS_USE_ROOT
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#endif

class G4Event;

class NpolRun : public G4Run 
{
public:
  NpolRun();
  virtual ~NpolRun();
  
public:
  virtual void RecordEvent(const G4Event*);
  
protected:
  //G4int numberOfNeutrons;
  //G4int numberOfPhotons;
  
private:
  G4int nEvent;
  // G4int analyzerCollID;
  
#ifdef G4ANALYSIS_USE_ROOT
  TTree *tree;
  struct {
    Int_t vertex;
    Int_t event;
  } ntuple;
#endif

  
public:
#ifdef G4ANALYSIS_USE_ROOT
  inline void SetTree(TTree* t) {tree = t;};
  inline void* GetNtuplePointer() {return &ntuple;};
#endif
  
};

#endif
