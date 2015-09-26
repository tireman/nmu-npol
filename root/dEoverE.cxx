
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TVector.h>
#include <TH1.h>
#include <TSystem.h>
#include <TClassTable.h>
#include <TInterpreter.h>

#include "../include/NpolVertex.hh"
#include "../include/NpolTagger.hh"


int GetAVNumber(const std::string &volName) {
  if(volName.substr(0,3) == "av_") {
    int underscoreLocation = volName.find_first_of("_",3);
    return atoi(volName.substr(3,underscoreLocation-3).c_str());
  } else{
    return 0;
  }
}

void dEoverE() {
  gSystem->Load("NpolClass.so");
  std::vector<NpolVertex *> *anEntry = NULL;
  std::map<std::string, double> eDep;
   
  // The TChain is very nice.
  TChain *npolTree = new TChain("T");
  //npolTree->Add("/data2/tireman/LongRun/npolRun_*.root");
  //npolTree->Add("/data2/tireman/LongRun/npolRun_1_0001.root");
  // npolTree->Add("/data2/tireman/LongRun/npolRun_1_0002.root");
  
  npolTree->Add("/data/tireman/simulation/output/FirstPass/Test/EventCut_0001.root");
  npolTree->SetBranchAddress("tracks",&anEntry);
  
  TFile *outFile = new TFile("dEoverE_p.root","RECREATE");
  TH2F *h_dEoverEtop = new TH2F("dEoverEtop","dE over E",20,0,150,40,0,20);
  TH2F *h_dEoverEbot = new TH2F("dEoverEbot","dE over E",20,0,150,40,0,20);
  TH2F *h_dEoverEplus = new TH2F("dEoverEplus","dE over E",20,0,150,40,0,20);
  TH2F *h_dEoverEdivide = new TH2F("dEoverEdivide","dE over E",20,0,150,40,0,20);
  
  // loop over all entries (one per event)
  Int_t nentries = npolTree->GetEntries();
  for(int i = 0; i < nentries; i++) {
    npolTree->GetEntry(i);
      
    if(i % 10000 == 0)
      std::cout << "Processing event #" << i << std::endl;
    
    // loop over vector elements (one per vertex)
    if(anEntry->empty()) continue;
    Int_t nvertices = anEntry->size();
 
    for (Int_t j = 0; j < nvertices; j++){
      NpolVertex *aVertex = (*anEntry)[j];
      
      if(aVertex == NULL) continue;
      if(!(aVertex->daughterIds).empty()) continue;
      if(aVertex->eMiss) continue;     
      if(eDep.find(aVertex->volume) == eDep.end())
	eDep[aVertex->volume] = 0;
      eDep[aVertex->volume] += aVertex->energy;
      
    }
  
    double Etop = 0.0;
    double Ebot = 0.0;
    double dEtop = 0.0;
    double dEbot = 0.0;
    
    std::map<std::string,double>::iterator it2;
    for(it2 = eDep.begin(); it2 != eDep.end(); it2++) {
      int avNum = GetAVNumber(it2->first);
      if(avNum == 0)
	continue;
      
      switch(avNum) {
      case 1: case 2:
	Etop += it2->second;
	break;			
      case 3: case 4:
	dEtop += it2->second;
	break;
      case 5: case 6:
	Ebot += it2->second;
	break;
      case 7: case 8:
	dEbot += it2->second;
	break;
      default:
	break;
      }		
    }
    
    h_dEoverEtop->Fill(Etop,dEtop);
    h_dEoverEbot->Fill(Ebot,dEbot);
    h_dEoverEplus->Fill(Etop+Ebot,dEtop+dEbot);
    h_dEoverEdivide->Fill(Etop/Ebot,dEtop+dEbot);
    
    eDep.clear();
    
  }
  
  h_dEoverEtop->Write();
  h_dEoverEbot->Write();
  h_dEoverEplus->Write();
  h_dEoverEdivide->Write();
  outFile->Close();

}

