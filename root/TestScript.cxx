
#include <iostream>
#include <string>
#include <vector>

#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>

#include "../include/NpolVertex.hh"

typedef struct detector_hit {
	std::string particle; // the particle that caused the hit
	double time;
	double eDep;
} DetectorHit;

void LoadClassesIntoDictionary() {
	if(!TClassTable::GetDict("NpolVertex.hh"))
		gSystem->Load("../NpolVertex_hh.so");

	gInterpreter->GenerateDictionary("vector<NpolVertex *>","../include/NpolVertex.hh;vector");
}

TTree *OpenFileAndGetTTree(char *filename) {
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

void TestScript() {

	std::vector<NpolVertex *> *anEntry = NULL;

	LoadClassesIntoDictionary();

	TTree *npolTree = OpenFileAndGetTTree("../npol.root");
	npolTree->SetBranchAddress("tracks_branch",&anEntry);

	// loop over all entries (one per event)
	for(int i = 0; i < npolTree->GetEntries(); i++) {
		npolTree->GetEntry(i);

		// loop over vector elements (one per vertex)
		std::vector<NpolVertex *>::iterator it;
		for(it = anEntry->begin(); it != anEntry->end(); it++) {
			NpolVertex *aVertex = *it;
			// ...
		}
	}
}

