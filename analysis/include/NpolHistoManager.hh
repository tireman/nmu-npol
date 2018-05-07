//***** Manager for Histograms and other Plots *****//

#ifndef Npol_Histogram_Manager_h
#define Npol_Hisogram_Manager_h

#include "TVector3.h"
#include "TVectorD.h"
#include "TMath.h"
#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH1.h"
#include "TH2.h"
#include <map>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>

class NpolHistoManager {

public:
  static NpolHistoManager *GetInstance();
  void CreateHistograms();
  void ClearHistograms();
  void DeleteHistograms();
  void FillHistograms(std::string hName, double value);
  void WriteHistograms();
  
private:
  NpolHistoManager();
  ~NpolHistoManager();
  
private:
  static NpolHistoManager *HistoMan;

public:
  TFile *outFile = NULL;
  TH1F *h_recoilAngle_Real;
  std::map<std::string,TH1F* > histograms;
  
};


#endif
