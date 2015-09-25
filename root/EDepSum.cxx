
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TObject.h>
#include <TSystem.h>
#include <TClassTable.h>
#include <TInterpreter.h>

//#include "../include/NpolVertex.hh"
//#include "../include/NpolTagger.hh"


TTree *OpenFileAndGetTTree(const char *filename) {
  TFile *inFile = new TFile(filename,"READ");
  TTree *npolTree = (TTree *)inFile->GetObjectChecked("T","TTree");
  if(npolTree == NULL) {
    std::cout << "Tree not found in npol.root." << std::endl;
    delete inFile;
    return NULL;
  }
  npolTree->Print();
  
  return npolTree;
}

void EDepSum() {
  gSystem->Load("NpolClass.so");
  gClassTable.Print();

  std::vector<NpolVertex *> anEntry = NULL;

  //TTree *npolTree = OpenFileAndGetTTree("/data2/tireman/LongRun/npolRun_1_0001.root");
  // TChain loads a list of root files with the small tree structure
  // To add more use the "Add" function.  Wildcards are accepted.
  TChain *npolTree = new TChain("T");
  npolTree->Add("/data2/tireman/LongRun/npolRun_1_0001.root");
  
  npolTree->SetBranchAddress("tracks",&anEntry);
  
  // loop over all entries (one per event)
  for(int i = 0; i < npolTree->GetEntries(); i++) {
  //for(int i = 0; i < 50000 ; i++){
    npolTree->GetEntry(i);
    
    if(i % 1000 == 0)
      std::cout << "Processing event #" << i << std::endl;
    
    std::map<std::string, double> eDep;
    std::vector<NpolVertex *>::iterator it;
    it = anEntry.begin();
    std::cout << "Got to Here!" << endl;
    NpolVertex *aVertex = *it;

    // loop over vector elements (one per vertex)
    //for(it = anEntry->begin(); it != anEntry->end(); it++) {
       std::cout << "Got to Here!" << endl;
       /*NpolVertex *aVertex = *it;
      if(aVertex == NULL)
      	continue;
      if(!(aVertex->daughterIds).empty())
      	continue;
      if(aVertex->eMiss)
      	continue;
       std::cout << "Got to Here!" << endl;
      if(eDep.find(aVertex->volume) == eDep.end())
	eDep[aVertex->volume] = 0;
	eDep[aVertex->volume] += aVertex->energy;*/
       //}
    
    std::cout << "Event No. " << i << ":" << std::endl;
    std::map<std::string,double>::iterator it2;
    for(it2 = eDep.begin(); it2 != eDep.end(); it2++) {
      std::cout << it2->first << ": " << it2->second << ", ";
    }
    std::cout << std::endl; 
    eDep.clear();
  }
}

