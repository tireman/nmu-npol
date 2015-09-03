//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************

// Daniel Wilbern, dwilbern@nmu.edu August 2015

#ifndef Npol_Analysis_Manager_h
#define Npol_Analysis_Manager_h

#include <vector>

class G4Track;
class TFile;
class TTree;
class TBranch;
class NpolVertex;
class NpolAnalysisMessenger;

class NpolAnalysisManager {

public:
  static NpolAnalysisManager *GetInstance();
  void BeginOfRun();
  void EndOfRun();
  void Initialize();
  void PrepareNewEvent();
  void AddTrack(const G4Track *);
  void FillTree();
  void WriteTree();
  void CloseFile();
  void OpenFile();
  void setFileName(const G4String& nam);
private:
  NpolAnalysisManager();
  ~NpolAnalysisManager();
  
private:
  NpolAnalysisMessenger* analysisMessenger;
  G4String rootName;
  bool initialized;
  TTree *npolTree;
  TBranch *tracksBranch;
  TFile *npolOutFile;
  std::vector<NpolVertex *> *tracks;
};

#endif

