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
#include <map>
#include "G4SystemOfUnits.hh"

class G4Track;
class G4Step;
class TFile;
class TTree;
class TBranch;
class NpolVertex;
class NpolTagger;
class NpolStep;
class NpolStatistics;
class NpolAnalysisMessenger;

class NpolAnalysisManager {

public:
   
  static NpolAnalysisManager *GetInstance();
  void BeginOfRun();
  void EndOfRun();
  void Initialize();
  void InitializeObjects();
  void PrepareNewEvent(const G4int evtID);
  void AddTrack(const G4Track *);
  void SetTrackAsKilled(int trackID);
  void AddNPOLTaggedParticle(const G4Track *);
  void AddSHMSTaggedParticle(const G4Track *);
  void AddStep(const G4Step *, std::string);
  void FillTree();
  void WriteObjectsToFile();
  void OpenRootFile();
  void setFileName(const G4String& nam);
  void ClearObjects();
  void InitializeFilenameVariables();
  int partition(std::vector<NpolStep*> *aVector, int p, int q);
  void QSort(std::vector<NpolStep*> *aVector, int p, int q);

private:
  NpolAnalysisManager();
  ~NpolAnalysisManager();
  
private:
  bool singletonInitialized;
  G4int eventsPerFile;
  G4String rootName; 
  G4String dirName;
  G4String jobNumber;
  TTree *npolTree;
  TTree *statsTree;
  G4int RootFileNumber;
  TFile *npolOutFile;
  std::vector<NpolVertex *> *tracks;
  std::vector<NpolTagger *> *NPOLTaggedParticle;
  std::vector<NpolTagger *> *SHMSTaggedParticle;
  std::vector<NpolStep *> *EventSteps;
  //std::map<std::string,double> *eDeposited;

  NpolStatistics *statistics;
};

#endif

