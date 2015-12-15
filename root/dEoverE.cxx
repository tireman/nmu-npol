
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <TFile.h>
#include <TChain.h>
#include <TBranch.h>
#include <TVector.h>
#include <TH2.h>
#include <TSystem.h>
#include <TClassTable.h>
#include <TInterpreter.h>

#include "NpolStatistics.hh"
#include "NpolVertex.hh"
#include "NpolStep.hh"

#define MeV 1.0

// Example assembly volume name: av_1_impr_1_TopDetLV_0

int GetAVNumber(const std::string &volName) {
	if(volName.substr(0,3) == "av_") {
		int underscorePos = volName.find_first_of("_",3);
		return atoi(volName.substr(3,underscorePos-3).c_str());
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

// Requirement 1: At least one hit (energy deposition >= 1 MeV) in some detector in the
// analyzer and an energy deposition of >= 1 MeV in some detectors in the dE and E arrays
// (both top or both bottom) in the same section.

// Requirement 2: No hits in the analyzers in the previous (upstream) sections to avoid
// counting of secondary scatterings in the polarimeter.

// Requirement 3: Total energy in the analyzer detectors in the section of interest should
// be more than 4 MeV; total energy deposition in the E-array of interest should be more
// than 5 MeV.

// Requirement 4: Total energy deposition in the analyzer+dE+E detectors in the section of
// interest should be >= 50 MeV (viz. a little bit more than the minimum ionizing particle
// (MIP) crossing two 10 cm thick detectors.

// Requirement 5: Top/Bottom asymmetry of energy depositions in the section of interest
// (for example, if we have "good" energy depositions in the bottom dE and E arrays, the
// total energy deposition in the top arrays should be at least 20 times less than in the
// bottom ones.  And vice versa...)  This criterion helps suppress inelastic interactions
// and select recoil protons from elastic and quasielastic scatterings of neutrons in the
// polarimeter analyzer.

// Requirement 6 (Not yet implemented): Because the analyzing power is concentrated in the
// range of neutron scattering angles of 4-25 degrees, we applied correspondent cuts on the
// "recoil proton" scattering angle (viz. 45.3 degrees at Q^2 = 3.95 (GeV/c)^2, 40.8-80.2
// degrees at 5.22 (GeV/c)^2, and 36.0-78.3 degrees at 6.88 (GeV/c)^2).

bool EventRequirementsPassed(const std::map<std::string,double> &eDep) {
	return true;
}

void dEoverE() {
	gSystem->Load("NpolClass.so");
	std::vector<NpolVertex *> *aVertexEntry = NULL;
	std::vector<NpolStep *> *aStepEntry = NULL;
	std::vector<NpolStatistics *> *aStatsEntry = NULL;
	std::map<std::string, double> eDep;

	// The TChain is very nice.
	TChain *npolTree = new TChain("T");
	npolTree->Add("/data3/cgen/FirstRun/NeutronOnly/*.root");
	TChain *statsTree = new TChain("T2");
	statsTree->Add("/data3/cgen/FirstRun/NeutronOnly/*.root");

	npolTree->SetBranchAddress("tracks",&aVertexEntry);
	npolTree->SetBranchAddress("steps",&aStepEntry);
	statsTree->SetBranchAddress("stats",&aStatsEntry);

	// loop over all stats branches (one per file)
	Long_t totalNeutrons = 0;
	Long_t totalNeturonsRecorded = 0;
	for(int i = 0; i < statsTree->GetEntries(); i++) {
		statsTree->GetEntry(i);
		totalNeutrons += ((*aStatsEntry)[0])->totalEvents;
		totalNeturonsRecorded += ((*aStatsEntry)[0])->eventsSaved;
	}
	std::cout << "Total neutrons on polarimeter: " << totalNeutrons << std::endl
		<< "Total neutrons striking polarimeter: " << totalNeturonsRecorded << std::endl;

	TFile *outFile = new TFile("dEoverE_elasticequirement.root","RECREATE");
	TH2F *h_dEoverEtop = new TH2F("dEoverEtop","dE over E",200,0,150,400,0,20);
	TH2F *h_dEoverEbot = new TH2F("dEoverEbot","dE over E",200,0,150,400,0,20);

	// loop over all entries (one per event)
	Int_t nentries = npolTree->GetEntries();
       	for(int i = 0; i < nentries; i++) {
	  //for(int i = 0; i < 100000; i++) {
		npolTree->GetEntry(i);

		if(i % 1000 == 0)
			std::cout << "Processing event #" << i << std::endl;

		// loop over all vertices, determine if there was a proton created by hadElastic
		bool elasticFlag = false;
		std::vector<NpolVertex *>::iterator v_it;
		for(v_it = aVertexEntry->begin(); v_it != aVertexEntry->end(); v_it++) {
			NpolVertex *aVertex = *v_it;
			if(aVertex == NULL)
				continue;

			int avNum = GetAVNumber(aVertex->volume);
			if(avNum == 9 || avNum == 10)
				if(aVertex->parentId == 1 && aVertex->process == "hadElastic" && aVertex->particle == "proton") {
					elasticFlag = true;
					break;
				}
		}

		if(!elasticFlag)
			continue;

		std::map<std::string,double> eDep;

		// loop over all steps, fill eDep map
		std::vector<NpolStep *>::iterator s_it;
		for(s_it = aStepEntry->begin(); s_it != aStepEntry->end(); s_it++) {
			NpolStep *aStep = *s_it;
			if(aStep == NULL)
				continue;

			if(eDep.find(aStep->volume) == eDep.end())
				eDep[aStep->volume] = 0;
			eDep[aStep->volume] += aStep->eDep;
		}

		double Etop = 0.0;
		double Ebot = 0.0;
		double dEtop = 0.0;
		double dEbot = 0.0;

		// loop over all volumes in eDep map, fill dE/E histograms
		std::map<std::string,double>::iterator e_it;
		for(e_it = eDep.begin(); e_it != eDep.end(); e_it++) {
			int avNum = GetAVNumber(e_it->first);
			if(avNum == 0)
				continue;

			switch(avNum) {
				case 1: case 2:
					Etop += e_it->second;
					break;
				case 3: case 4:
					dEtop += e_it->second;
					break;
				case 5: case 6:
					Ebot += e_it->second;
					break;
				case 7: case 8:
					dEbot += e_it->second;
					break;
				default:
					break;
			}
		}

		if(Etop > 5*MeV && dEtop > 1*MeV)
			h_dEoverEtop->Fill(Etop,dEtop);
		if(Ebot > 5*MeV && dEbot > 1*MeV)
			h_dEoverEbot->Fill(Ebot,dEbot);

		eDep.clear();
	} // end event loop

	h_dEoverEtop->Write();
	h_dEoverEbot->Write();
	delete outFile;
}

