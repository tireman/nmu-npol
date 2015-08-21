//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************

// Daniel Wilbern, dwilbern@nmu.edu February 2015

#ifndef Npol_Data_Structure_h
#define Npol_Data_Structure_h

#include <map>

#include "G4SystemOfUnits.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Event.hh"

//#include "TROOT.h"
//#include "TFile.h"


#define PROTON_ID 1
#define NEUTRON_ID 2
#define ELECTRON_ID 3
#define POSITRON_ID 4
#define GAMMA_ID 5
#define PIPOS 6
#define PINEG 7
#define PINEUTRAL 8

struct NtupleColumns {
  G4int volumeIDColID;
  G4int particleIDColID;
  G4int parentIDColID;
  G4int trackIDColID;
  G4int stepIDColID;
  G4int eventIDColID;
  G4int vertexEnergyColID;
  G4int kineticEnergyColID;
  G4int WxPosColID;
  G4int WyPosColID;
  G4int WzPosColID; 
  G4int VxPosColID;
  G4int VyPosColID;
  G4int VzPosColID;
  G4int xMomColID;
  G4int yMomColID;
  G4int zMomColID;
};

struct HistoData {
  G4int histoID;
  char *name;
  char *title;
  G4int nbins;
  G4double xmin;
  G4double xmax;
  G4double EDep;
};

class NpolAnalysisManager {
  
public:
  static NpolAnalysisManager *GetInstance();
  void RegisterActiveDetectorEDepHistogram(G4VPhysicalVolume *PV, char *nname, char *ttitle, G4int nnbins, G4double xxmin, G4double xxmax);
  void CreateHistograms();
  void CreateNtuple();
  void PrepareNewEvent(int eventID);
  void AddEDep(G4VPhysicalVolume *PV, G4double dep);
  void FillHistograms();
  void WriteDetectorIDsToFile();
  void FillNtuple(G4VPhysicalVolume *PV, G4int particleID, G4int parentID, G4int trackID, G4int stepID, G4double depositEnergy, G4double vertexEnergy, G4double kineticEnergy);//, G4double VxPos, G4double VyPos, G4double VzPos, G4double xMom, G4double yMom, G4double zMom);
  
private:
  void FillVolNameColumns(char *volName);
  void FillAHistogram(struct HistoData *histoData);
  bool isVolumeActive(G4VPhysicalVolume *PV);
  int getVolIDFor(G4VPhysicalVolume *PV);

private:
  NpolAnalysisManager();
  ~NpolAnalysisManager();
  
  std::map<G4VPhysicalVolume *, struct HistoData> detData;
  std::map<G4VPhysicalVolume *, int> detectorIDs;
  struct NtupleColumns cols;
  int nextVolumeID;
  int currentEventID;
  
};

#endif

