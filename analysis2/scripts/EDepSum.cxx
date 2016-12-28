
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
#include "NpolStatistics.hh"
#include "NpolStep.hh"

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
void EDepSum() {
	gSystem->Load("NpolClasses.so");

	std::vector<NpolStep *> *anEntry = NULL;
	std::map<std::string,TH1 *> histograms;

	//	TTree *npolTree = OpenFileAndGetTTree("/data3/cgen/FirstRun/ProtonOnly/npolProton_1_0001.root");
	// TChain loads a list of root files with the small tree structure
	// To add more use the "Add" function.  Wildcards are accepted.
	TChain *npolTree = new TChain("T");
	npolTree->Add("/home/dwilbern/output/*.root");
//	npolTree->Add("../build/output/*");

	npolTree->SetBranchAddress("steps",&anEntry);
	npolTree->Print();

	// loop over all entries (one per event)
	Int_t nentries = npolTree->GetEntries();
	std::cout << "Total Events: " << nentries << std::endl;
	for(int i = 0; i < npolTree->GetEntries(); i++) {
		npolTree->GetEntry(i);

		if(i % 1000 == 0)
			std::cout << "Processing event #" << i << std::endl;

		std::map<std::string, double> eDep;

		// loop over vector elements (one per step)
		Int_t nvertices = anEntry->size();
		for(int j = 0; j < nvertices; j++) {
			NpolStep *aStep = (*anEntry)[j];
			if(aStep == NULL)
				continue;
			
			if(eDep.find(aStep->volume) == eDep.end())
				eDep[aStep->volume] = 0;
			eDep[aStep->volume] += aStep->eDep;
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
  
  std::vector<NpolVertex *> *anEntry = NULL;
  std::vector<NpolStep *> *EventSteps = NULL;
  std::map<std::string,TH1 *> histograms;
  std::map<std::string, double>eDep;
  NpolStatistics *aStat = NULL;
  
  // TChain loads a list of root files with the small tree structure
  // To add more use the "Add" function.  Wildcards are accepted.
  TChain *npolTree = new TChain("T");
  TChain *statsTree = new TChain("T2");

  npolTree->Add("/data2/muon/npolTest_1_*.root");
  statsTree->Add("/data2/muon/npolTest_1_*.root");

  npolTree->SetBranchAddress("tracks",&anEntry);
  npolTree->SetBranchAddress("Event_Steps",&EventSteps);
  statsTree->SetBranchAddress("stats",&aStat);
  
  // Count the total number of electrons on target and total events 
  // saved to the files 
  Long_t TotalElectrons = 0;
  Long_t TotalEventsRecorded = 0;
  for(int i = 0; i < statsTree->GetEntries(); i++){
    statsTree->GetEntry(i);
    TotalElectrons += aStat->totalEvents;
    TotalEventsRecorded += aStat->eventsSaved;
  }
  
  // loop over all entries (one per event)
  Int_t nentries = npolTree->GetEntries();
  std::cout << "Total Events: " << nentries << std::endl;

  for(int i = 0; i < npolTree->GetEntries(); i++) {
    npolTree->GetEntry(i);
    
    if(i % 1000 == 0)
      std::cout << "Processing event #" << i << std::endl;
   
    // loop over vector elements (one per vertex) 
    std::map<std::string,double>::iterator it;
    Int_t nvertices = EventSteps->size();
    for(int j = 0; j < nvertices; j++) { 
      NpolStep *aStep = (*EventSteps)[j];
      if(aStep == NULL) continue;
      if(eDep.find(aStep->volume) == eDep.end()){
	eDep.insert(std::pair<std::string,double>(aStep->volume,0.0));
      }
      it = eDep.find(aStep->volume);
      it->second += aStep->eDep;
    }
    
    for(it = eDep.begin(); it != eDep.end(); it++) {
      if(histograms.find(it->first) == histograms.end()) {
	histograms[it->first] = new TH1F((it->first).c_str(),(it->first).c_str(),500,0*MeV,200*MeV);
      }
      if(it->second > 0.0) (histograms[it->first])->Fill(it->second);
    }
    
    eDep.clear();
  }
  
  TFile *outFile = new TFile("EDep_Proton.root","RECREATE");
  std::map<std::string, TH1 *>::iterator it2;
  for(it2 = histograms.begin(); it2 != histograms.end(); it2++) {
    it2->second->Write();
  }

  std::cout << "Total electrons: " << TotalElectrons << std::endl;
  delete outFile;
}

