
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
	std::map<std::string,TH1 *> histograms;

	//	TTree *npolTree = OpenFileAndGetTTree("/data3/cgen/FirstRun/ProtonOnly/npolProton_1_0001.root");
	// TChain loads a list of root files with the small tree structure
	// To add more use the "Add" function.  Wildcards are accepted.
	TChain *npolTree = new TChain("T");
	npolTree->Add("/data3/cgen/FirstRun/ProtonOnly/*");
//	npolTree->Add("../build/output/*");

	npolTree->SetBranchAddress("tracks",&anEntry);
	npolTree->Print();

	// loop over all entries (one per event)
	Int_t nentries = npolTree->GetEntries();
	std::cout << "Total Events: " << nentries << std::endl;
	for(int i = 0; i < npolTree->GetEntries(); i++) {
		npolTree->GetEntry(i);

		if(i % 1000 == 0)
			std::cout << "Processing event #" << i << std::endl;

		std::map<std::string, double> eDep;

		// loop over vector elements (one per vertex)
		Int_t nvertices = anEntry->size();
		for(int j = 0; j < nvertices; j++) {
			NpolVertex *aVertex = (*anEntry)[j];
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

		std::map<std::string,double>::iterator it;
		for(it = eDep.begin(); it != eDep.end(); it++) {
			if(histograms.find(it->first) == histograms.end()) {
				histograms[it->first] = new TH1F((it->first).c_str(),(it->first).c_str(),500,0*MeV,10*MeV);
			}
			(histograms[it->first])->Fill(it->second);
		}

		eDep.clear();
	}

	TFile *outFile = new TFile("EDep.root","RECREATE");
	std::map<std::string, TH1 *>::iterator it2;
	for(it2 = histograms.begin(); it2 != histograms.end(); it2++) {
		it2->second->Write();
	}
	delete outFile;
}

