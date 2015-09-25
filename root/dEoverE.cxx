
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

std::vector<NpolVertex *> *anEntry = NULL;
std::vector<NpolVertex *>::iterator it;
std::map<std::string, double> eDep;

void dEoverE() {
  gSystem->Load("NpolClass.so");

  // Use this section if you wish to load one file at a time ... but why?
  //char *filename = "/data2/tireman/LongRun/npolRun_1_0001.root";
  //TFile *inFile = new TFile(filename);//,"READ");
  //TTree *npolTree = (TTree *)inFile->GetObjectChecked("T","TTree");
  //if(npolTree == NULL) {
  //  std::cout << "Tree not found in " << filename << "." << std::endl;
  //  delete inFile;
  // return;
  //}
  
  // The TChain is very nice.
  TChain *npolTree = new TChain("T");
  npolTree->Add("/data2/tireman/LongRun/npolRun_*.root");
  //npolTree->Add("/data2/tireman/LongRun/npolRun_1_0001.root");
  // npolTree->Add("/data2/tireman/LongRun/npolRun_1_0002.root");
  
  
  npolTree->SetBranchAddress("tracks",&anEntry);
  
  TFile *outFile = new TFile("dEoverE.root","RECREATE");
  TH2F *h_dEoverEtop = new TH2F("dEoverEtop","dE over E",20,0,150,40,0,20);
  TH2F *h_dEoverEbot = new TH2F("dEoverEbot","dE over E",20,0,150,40,0,20);
  TH2F *h_dEoverEplus = new TH2F("dEoverEplus","dE over E",20,0,150,40,0,20);
  TH2F *h_dEoverEdivide = new TH2F("dEoverEdivide","dE over E",20,0,150,40,0,20);
  
  // loop over all entries (one per event)
  //for(int i = 0; i < 100000; i++) {
  Int_t nentries = npolTree->GetEntries();
  for(int i = 0; i < nentries; i++) {
    npolTree->GetEntry(i);
      
    if(i % 10000 == 0)
      std::cout << "Processing event #" << i << std::endl;
    
    Int_t nvertices = anEntry->size();
    it = anEntry->begin();
   
    // loop over vector elements (one per vertex)
    // for(it = anEntry->begin(); it != anEntry->end(); it++) {
         // HACK ALERT!     
         // There is something up with this for loop.  Running as I have it is
         // fine but using the for loop above fills up the memory even when you
         // comment out ALL code in the loop.  What the ...?
   
    for (Int_t j = 0; j < nvertices; j++){
      NpolVertex *aVertex = *it;
      
      if(aVertex == NULL)
	continue;
      if(!(aVertex->daughterIds).empty())
	continue;
      if(aVertex->eMiss)
	continue;
      
      if(eDep.find(aVertex->volume) == eDep.end())
	eDep[aVertex->volume] = 0;
      eDep[aVertex->volume] += aVertex->energy;
      
      it++;
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
    
    if((Etop > 20.0 || Ebot > 20.0) && (dEtop > 1.0 || dEbot > 1.0)){
      h_dEoverEtop->Fill(Etop,dEtop);
      h_dEoverEbot->Fill(Ebot,dEbot);
      h_dEoverEplus->Fill(Etop+Ebot,dEtop+dEbot);
      h_dEoverEdivide->Fill(Etop/Ebot,dEtop+dEbot);
    }
    eDep.clear();
    
  }
  
  h_dEoverEtop->Write();
  h_dEoverEbot->Write();
  h_dEoverEplus->Write();
  h_dEoverEdivide->Write();
  outFile->Close();

}

