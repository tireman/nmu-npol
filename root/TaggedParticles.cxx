
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

void TaggedParticles() {
  gSystem->Load("NpolClass.so");

  Double_t xpos, ypos, zpos;
  std::vector<NpolTagger *> *tagEntry = NULL;

  // The TChain is very nice.
  TChain *npolTree = new TChain("T");
  //npolTree->Add("/data2/tireman/LongRun/npolRun_*.root");
  //npolTree->Add("/data2/tireman/LongRun/npolRun_1_0001.root");
  //npolTree->Add("/data2/tireman/LongRun/npolRun_1_0002.root");
  npolTree->Add("/data/tireman/simulation/output/FirstPass/Test/EventCut_0001.root");
  
  npolTree->SetBranchAddress("tagger",&tagEntry);
  
  TFile *outFile = new TFile("TaggedParticlesTest.root","RECREATE");
  TH2F *h_gammaXY = new TH2F("h_gammaXY","Gamma Rays in Tagger",200,-3.6,-2.6,160,-0.80,0.80);
  TH2F *h_electronXY = new TH2F("h_electronXY","Electrons in Tagger",200,-3.6,-2.6,160,-0.80,0.80);
  TH2F *h_protonXY = new TH2F("h_protonXY","Protons in Tagger",200,-3.6,-2.6,160,-0.80,0.80);
  TH2F *h_neutronXY = new TH2F("h_neutronXY","Neutrons in Tagger",200,-3.6,-2.6,160,-0.80,0.80);
  TH2F *h_otherXY = new TH2F("h_otherXY","Other Particles in Tagger",200,-3.6,-2.6,160,-0.80,0.80);
  
  // loop over all entries (one per event)
  Int_t nentries = npolTree->GetEntries();
  for(int i = 0; i < nentries; i++) {
    npolTree->GetEntry(i);
      
    if(i % 100 == 0)
      std::cout << "Processing event #" << i << std::endl;
    
    // loop over vector elements (one per vertex)
    if(tagEntry->empty()) continue;
    Int_t nvertices = tagEntry->size();
    std::cout << "vertices = " << nvertices << endl;
    
   for (Int_t j = 0; j < 1; j++){
     
     NpolTagger *aVertex = (*tagEntry)[j];
     
     if(aVertex == NULL) continue;
     
     xpos = aVertex->posX;
     ypos = aVertex->posY;
     if(aVertex->particle == "gamma") {
       h_gammaXY->Fill(xpos,ypos);
     }else if(aVertex->particle == "e-"){
       h_electronXY->Fill(xpos,ypos);
     }else if(aVertex->particle == "proton"){
       h_protonXY->Fill(xpos,ypos);
     }else if(aVertex->particle == "neutron"){
       h_neutronXY->Fill(xpos,ypos);
     }else{
       h_otherXY->Fill(xpos,ypos);
     }
     
   }
  }
  
  h_gammaXY->Write();
  h_electronXY->Write();
  h_protonXY->Write();
  h_neutronXY->Write();
  h_otherXY->Write();
  outFile->Close();

}

