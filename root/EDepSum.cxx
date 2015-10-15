
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TBranch.h>
#include <TObject.h>
#include <TSystem.h>
#include <TClassTable.h>
#include <TInterpreter.h>

#include "../include/NpolVertex.hh"


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

	std::vector<NpolVertex *> *anEntry = NULL;

//	TTree *npolTree = OpenFileAndGetTTree("/data3/cgen/FirstRun/ProtonOnly/npolProton_1_0001.root");
	// TChain loads a list of root files with the small tree structure
	// To add more use the "Add" function.  Wildcards are accepted.
	TChain *npolTree = new TChain("T");
	npolTree->Add("/data3/cgen/FirstRun/ProtonOnly/*");

	npolTree->SetBranchAddress("tracks",&anEntry);
	npolTree->Print();

	// loop over all entries (one per event)
	Int_t nentries = npolTree->GetEntries();
	//for(int i = 0; i < npolTree->GetEntries(); i++) {
	for(int i = 0; i < 5000 ; i++){
		npolTree->GetEntry(i);

		std::cout << anEntry->size() << std::endl;

		if(i % 100 == 0)
			std::cout << "Processing event #" << i << std::endl;

		std::map<std::string, double> eDep;

		// loop over vector elements (one per vertex)
		Int_t nvertices = anEntry->size();
		for(int i = 0; i < nvertices; i++) {
			NpolVertex *aVertex = (*anEntry)[i];
			if(aVertex == NULL)
				continue;
			if(!(aVertex->daughterIds).empty())
				continue;
			if(aVertex->eMiss)
				continue;
			if(eDep.find(aVertex->volume) == eDep.end())
				eDep[aVertex->volume] = 0;
			eDep[aVertex->volume] += aVertex->energy;
		}

		std::cout << "Event No. " << i << ":" << std::endl;
		std::map<std::string,double>::iterator it2;
		for(it2 = eDep.begin(); it2 != eDep.end(); it2++) {
			std::cout << it2->first << ": " << it2->second << ", ";
		}
		std::cout << std::endl; 
		eDep.clear();
	}
}

