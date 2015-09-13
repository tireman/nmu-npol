
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TSystem.h>
#include <TClassTable.h>
#include <TInterpreter.h>

#include "../include/NpolVertex.hh"
#include "../include/NpolTagger.hh"

void LoadClassesIntoDictionary() {
	if(!TClassTable::GetDict("NpolVertex.hh"))
		gSystem->Load("../NpolVertex_hh.so");
	if(!TClassTable::GetDict("NpolTagger.hh"))
		gSystem->Load("../NpolTagger_hh.so");

	gInterpreter->GenerateDictionary("vector<NpolVertex *>","../include/NpolVertex.hh;vector");
	gInterpreter->GenerateDictionary("vector<NpolTagger *>","../include/NpolTagger.hh;vector");
}

TTree *OpenFileAndGetTTree(const char *filename) {
	TFile *inFile = new TFile("../npol.root","READ");
	TTree *t = (TTree *)inFile->GetObjectChecked("t_npolTree","TTree");
	if(t == NULL) {
		std::cout << "t_npolTree not found in npol.root." << std::endl;
		delete inFile;
		return NULL;
	}
	t->Print();

	return t;
}

void EDepSum() {

	std::vector<NpolVertex *> *anEntry = NULL;

	LoadClassesIntoDictionary();

	TTree *npolTree = OpenFileAndGetTTree("../npol.root");
	npolTree->SetBranchAddress("tracks_branch",&anEntry);

	// loop over all entries (one per event)
	for(int i = 0; i < npolTree->GetEntries(); i++) {
		npolTree->GetEntry(i);

		if(i % 100 == 0)
			std::cout << "Processing event #" << i << std::endl;

		std::map<std::string, double> eDep;

		std::vector<NpolVertex *>::iterator it;
		// loop over vector elements (one per vertex)
		for(it = anEntry->begin(); it != anEntry->end(); it++) {
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

