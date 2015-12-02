
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TBranch.h>
#include <TH1.h>
#include <TObject.h>
#include <TSystem.h>
#include <TClassTable.h>
#include <TInterpreter.h>

#include "NpolVertex.hh"

#define MeV 1.0

int GetAVNumber(const std::string &volName) {
  if(volName.substr(0,3) == "av_") {
    int underscoreLocation = volName.find_first_of("_",3);
    return atoi(volName.substr(3,underscoreLocation-3).c_str());
  } else{
    return 0;
  }
}

int GetImprNumber(const std::string &volName) {
  if(volName.substr(0,3) == "av_") {
    int underscorePos = volName.find_first_of("_",1+
      volName.find_first_of("_",3));
    return atoi(volName.substr(underscorePos+1,1).c_str());
  } else
    return 0;
}

int GetPlacementNumber(const std::string &volName) {
  if(volName.substr(0,3) == "av_") {
    int underscorePos = volName.find_first_of("_",1+
      volName.find_first_of("_",1+
      volName.find_first_of("_",1+
      volName.find_first_of("_",1+
      volName.find_first_of("_",3)))));
    return atoi(volName.substr(underscorePos+1,std::string::npos).c_str());
  } else
    return 0;
}


void EDepSum() {
  gSystem->Load("NpolClass.so");
  
  //std::vector<NpolVertex *> *anEntry = NULL;
  std::map<std::string,TH1 *> histograms;
  std::map<std::string, double>* eDep = NULL;

  // TChain loads a list of root files with the small tree structure
  // To add more use the "Add" function.  Wildcards are accepted.
  TChain *npolTree = new TChain("T");
  npolTree->Add("/data2/muon/npolNegMuon_2GeV_Vertex_1_*.root");
  
  //npolTree->SetBranchAddress("tracks",&anEntry);
  npolTree->SetBranchAddress("EDeposit_Volume",&eDep);

  // loop over all entries (one per event)
  Int_t nentries = npolTree->GetEntries();
  std::cout << "Total Events: " << nentries << std::endl;
  //for(int i = 0; i < 500000; i++) {
  for(int i = 0; i < npolTree->GetEntries(); i++) {
    npolTree->GetEntry(i);
    
    if(eDep == NULL) continue;
    
    if(i % 1000 == 0)
      std::cout << "Processing event #" << i << std::endl;
    
    /* // loop over vector elements (one per vertex)
    Int_t nvertices = anEntry->size();
    for(int j = 0; j < nvertices; j++) {
      NpolVertex *aVertex = (*anEntry)[j];
      if(aVertex == NULL) continue;
      if(!(aVertex->daughterIds).empty()) continue;
      if(aVertex->eMiss) continue;
      //if(aVertex->parentId != 1) continue;
      //cout << "Process: " << aVertex->process << endl;
      if(eDep.find(aVertex->volume) == eDep.end())
	eDep[aVertex->volume] = 0;
      eDep[aVertex->volume] += aVertex->energy;
      }*/
    
    std::map<std::string,double>::iterator it;
    for(it = eDep->begin(); it != eDep->end(); it++) {
      if(histograms.find(it->first) == histograms.end()) {
	histograms[it->first] = new TH1F((it->first).c_str(),(it->first).c_str(),500,0*MeV,200*MeV);
      }
      if(it->second > 0.0) (histograms[it->first])->Fill(it->second);
    }
    
    eDep->clear();
  }
  
  TFile *outFile = new TFile("EDep_Muon.root","RECREATE");
  std::map<std::string, TH1 *>::iterator it2;
  for(it2 = histograms.begin(); it2 != histograms.end(); it2++) {
    it2->second->Write();
  }
  delete outFile;
}

