
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <TFile.h>
#include <TChain.h>
#include <TTree.h>
#include <TBranch.h>
#include <TVector.h>
#include <TH1.h>
#include <TH1.h>
#include <TSystem.h>
#include <TInterpreter.h>

#include "NpolVertex.hh"
#include "NpolTagger.hh"

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

void BasicTemplate() {
  gSystem->Load("NpolClass.so"); // Load up the user class lib
  
  Double_t xpos, ypos, zpos, xmom, ymom, zmom, totmom;
  Double_t E_D913;

  std::vector<NpolVertex *> *anEntry = NULL;
  std::vector<NpolTagger *> *npolEntry = NULL;
  std::vector<NpolTagger *> *shmsEntry = NULL;

  // The TChain is very nice.
  TChain *npolTree = new TChain("T");
  npolTree->Add("/data3/cgen/FirstRun/test_*.root");  
  
  npolTree->SetBranchAddress("NPOL_Tagger",&npolEntry);
  npolTree->SetBranchAddress("SHMS_Tagger",&shmsEntry);
  npolTree->SetBranchAddress("tracks",&anEntry);
  
  // this is a good place for your histograms to be created
  TH1F *H_D913 = new TH1F("H_D913","Energy in Detector 913",200, 0., 100.);
  
  // loop over all entries (one per event)
  Int_t nentries = npolTree->GetEntries();
  for(int i = 0; i < nentries; i++) {
    npolTree->GetEntry(i);
    
    if(i % 1000 == 0)
      std::cout << "Processing event #" << i << std::endl;
    
    // loop over vector elements (one per vertex in the tracks branch)
    if(anEntry->empty()) continue;
    Int_t nvertices = anEntry->size();
    
    for (Int_t j = 0; j < nvertices; j++){
      
      NpolVertex *aVertex = (*anEntry)[j];
      
      // enter code to analyze the tracks vertices
      if(aVertex == NULL) continue;
      //if(!(aVertex->particle == "proton")) continue;
      if(!(aVertex->daughterIds).empty()) continue;
      if(aVertex->eMiss) continue;
      if(GetAVNumber(aVertex->volume) == 9){
	if(GetImprNumber(aVertex->volume) == 1){
	  if(GetPlacementNumber(aVertex->volume) == 3){
	    E_D913 = aVertex->energy;
	  }
	}
      }
    }
    
    // loop over vector elements (one per vertex in the tagger branch)
    if(npolEntry->empty()) continue;
    nvertices = npolEntry->size();
    
    for (Int_t j = 0; j < nvertices; j++){
      
      NpolTagger *aVertex2 = (*npolEntry)[j];
      
      if(aVertex2 == NULL) continue;
      
      // enter code to analyze the tagger vertex
    }
    H_D913->Fill(E_D913);
  }
  
  
  // open, write histograms and other stuff to the ROOT outfile here
  TFile *outFile = new TFile("CreativeName.root","RECREATE");
  H_D913->Write();   // Write object to ROOT file
  outFile->Close();  // close the root file
  
}

