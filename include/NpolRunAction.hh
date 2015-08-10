//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************

#ifndef RunAction_h
#define RunAction_h

#include "G4UserRunAction.hh"
#include "G4Timer.hh"

//#include "TROOT.h"
//#include "TFile.h"
//#include "TTree.h"
//#include "TSystem.h"
//#include "TString.h"
//#include "TObject.h"

class G4Run;
//class TTree;
//class TFile;
class NpolRunAction : public G4UserRunAction
{
public:
NpolRunAction();
~NpolRunAction();

public:
void BeginOfRunAction(const G4Run*);
void EndOfRunAction(const G4Run*);

private:
  G4Timer* runTimer;
  //TFile *theTFile;
  //TTree *tree;
};

#endif

