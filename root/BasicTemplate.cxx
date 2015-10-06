
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

void BasicTemplate() {
  gSystem->Load("NpolClass.so"); // Load up the user class lib
  
  Double_t xpos, ypos, zpos;
  std::vector<NpolVertex *> *anEntry = NULL;
  std::vector<NpolTagger *> *tagEntry = NULL;
  
  // The TChain is very nice.
  TChain *npolTree = new TChain("T");
  //npolTree->Add("/data2/cgen/FirstRun/NoMagField/npolNoMagField_*.root");
  npolTree->Add("/data2/cgen/FirstRun/npolNpol_*.root");  
  
  npolTree->SetBranchAddress("tagger",&tagEntry);
  npolTree->SetBranchAddress("tracks",&anEntry);
  
  TFile *outFile = new TFile("CreativeName.root","RECREATE");
  
  // this is a good place for your histograms to be created
  
  // loop over all entries (one per event)
  Int_t nentries = npolTree->GetEntries();
  for(int i = 0; i < nentries; i++) {
    npolTree->GetEntry(i);
    
    if(i % 100 == 0)
      std::cout << "Processing event #" << i << std::endl;
    
    // loop over vector elements (one per vertex in the tracks branch)
    if(anEntry->empty()) continue;
    Int_t nvertices = anEntry->size();
    
    for (Int_t j = 0; j < nvertices; j++){
      
      NpolVertex *aVertex = (*anEntry)[j];
      
      if(aVertex == NULL) continue;
      
      // enter code to analyze the tracks vertex
    }
    
    // loop over vector elements (one per vertex in the tagger branch)
    if(tagEntry->empty()) continue;
    Int_t nvertices = tagEntry->size();
    
    for (Int_t j = 0; j < nvertices; j++){
      
      NpolTagger *aVertex2 = (*tagEntry)[j];
      
      if(aVertex2 == NULL) continue;
      
      // enter code to analyze the tagger vertex
    }
    
  }
  
  
  // write histograms and other stuff to the ROOT outfile here
  
  outFile->Close();  // cose the root file
  
}

