
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

#define MeV 1.0

struct polarimeter_edep_info {
	bool analyzerSectionsHit[4];
	bool topESectionsHit[4];
	bool topdESectionsHit[4];
	bool bottomESectionsHit[4];
	bool bottomdESectionsHit[4];

	double analyzerSectionsEdep[4];
	double topESectionsEdep[4];
	double topdESectionsEdep[4];
	double bottomESectionsEdep[4];
	double bottomdESectionsEdep[4];
};

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

struct polarimeter_edep_info *GetEdepInfo(const std::map<std::string,double> &eDep) {
	struct polarimeter_edep_info *info = 
		(struct polarimeter_edep_info *)calloc(1,sizeof(struct polarimeter_edep_info));

	std::map<std::string,double>::iterator it;
	for(it = eDep.begin(); it != eDep.end(); it++) {
		int AVNumber = GetAVNumber(it->first);
		if(AVNumber == 0 || AVNumber > 10) continue;
		int PVNumber = GetPlacementNumber(it->first);

		switch(AVNumber) {
			case 1: // Top E array 1
				int section = (PVNumber >= 5 ? 0 : 1);
				(info->topESectionsEdep)[section] += it->second;
				if(it->second >= 1*MeV)
					(info->topESectionsHit)[section] = true;
				break;
			case 2: // Top E array 2
				int section = (PVNumber >= 5 ? 2 : 3);
				(info->topESectionsEdep)[section] += it->second;
				if(it->second >= 1*MeV)
					(info->topESectionsHit)[section] = true;
				break;
			case 3: // Top dE array 1
				int section = (PVNumber >= 5 ? 0 : 1);
				(info->topdESectionsEdep)[section] += it->second;
				if(it->second >= 1*MeV)
					(info->topdESectionsHit)[section] = true;
				break;
			case 4: // Top dE array 2
				int section = (PVNumber >= 5 ? 2 : 3);
				(info->topdESectionsEdep)[section] += it->second;
				if(it->second >= 1*MeV)
					(info->topdESectionsHit)[section] = true;
				break;
			case 5: // BottomE array 1
				int section = (PVNumber >= 5 ? 0 : 1);
				(info->bottomESectionsEdep)[section] += it->second;
				if(it->second >= 1*MeV)
					(info->bottomESectionsHit)[section] = true;
				break;
			case 6: // Bottom E array 2
				int section = (PVNumber >= 5 ? 2 : 3);
				(info->bottomESectionsEdep)[section] += it->second;
				if(it->second >= 1*MeV)
					(info->bottomESectionsHit)[section] = true;
				break;
			case 7: // Bottom dE array 1
				int section = (PVNumber >= 5 ? 0 : 1);
				(info->bottomdESectionsEdep)[section] += it->second;
				if(it->second >= 1*MeV)
					(info->bottomdESectionsHit)[section] = true;
				break;
			case 8: // Bottom dE array 2
				int section = (PVNumber >= 5 ? 2 : 3);
				(info->bottomdESectionsEdep)[section] += it->second;
				if(it->second >= 1*MeV)
					(info->bottomdESectionsHit)[section] = true;
				break;
			case 9: // Front arrays 1 and 2
				int ImprNumber = GetImprNumber(it->first);
				(info->analyzerSectionsEdep)[ImprNumber-1] += it->second;
				if(it->second >= 1*MeV)
					(info->analyzerSectionsHit)[ImprNumber-1] = true;
				break;
			case 10: // Front arrays 3 and 4
				int ImprNumber = GetImprNumber(it->first);
				(info->analyzerSectionsEdep)[ImprNumber+1] += it->second;
				if(it->second >= 1*MeV)
					(info->analyzerSectionsHit)[ImprNumber+1] = true;
				break;

			default:
				break;
		}
	}

	return info;
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

bool CheckRequirements(const struct polarimeter_edep_info *info) {

	// Find out which polarimeter section was hit, start from the back
	int sectionOfInterest = 0;
	for(int i = 3; i >= 0 ; i--) {
		if((info->analyzerSectionsHit)[i]) {
			// check requirement 2
			if(sectionOfInterest == 0)
				sectionOfInterest = i;
			else {
			  		return false;
			}
		}
	}
	// Check requirement 1a
	//if(sectionOfInterest == 0) return false;

	// Check requirement 5
	bool topWasHit;
	if((info->topESectionsEdep)[sectionOfInterest] > (info->bottomESectionsEdep)[sectionOfInterest]*20 &&
	(info->topdESectionsEdep)[sectionOfInterest] > (info->bottomdESectionsEdep)[sectionOfInterest]*20)
	  //	topWasHit = true;
	else if((info->bottomESectionsEdep)[sectionOfInterest] > (info->topESectionsEdep)[sectionOfInterest]*20 &&
	(info->bottomdESectionsEdep)[sectionOfInterest] > (info->topdESectionsEdep)[sectionOfInterest]*20)
	  //	topWasHit = false;
	else
	  //	return false;

	// Check requirement 4a
	if((info->analyzerSectionsEdep)[sectionOfInterest] <= 4*MeV)
	  //	return false;
	if(topWasHit) {
		// Check requirement 1b
		if(!((info->topESectionsHit)[sectionOfInterest] && (info->topdESectionsHit)[sectionOfInterest]))
		  //		return false;
		// Check requirement 4b
		if((info->topESectionsEdep)[sectionOfInterest] <= 5*MeV)
		  //		return false;
		// Check requirement 3
		double totalEdep = (info->analyzerSectionsEdep)[sectionOfInterest] + (info->topESectionsEdep)[sectionOfInterest] +
			(info->topdESectionsEdep)[sectionOfInterest];
		if(totalEdep <= 50 MeV)
		  //		return false;
	} else {
		if(!((info->bottomESectionsHit)[sectionOfInterest] && (info->bottomdESectionsHit)[sectionOfInterest]))
		  //return false;
		if((info->bottomESectionsEdep)[sectionOfInterest] <= 5*MeV)
		  //	return false;
		double totalEdep = (info->analyzerSectionsEdep)[sectionOfInterest] + (info->bottomESectionsEdep)[sectionOfInterest] +
			(info->bottomdESectionsEdep)[sectionOfInterest];
		if(totalEdep <= 50 MeV)
		  //return false;
	}

	return true;
}

bool EventRequirementsPassed(const std::map<std::string,double> &eDep) {
	struct polarimeter_edep_info *info = GetEdepInfo(eDep);

	bool ret = CheckRequirements(info);

	free(info);
	return ret;
}

void dEoverE() {
	gSystem->Load("NpolClass.so");
	std::vector<NpolVertex *> *anEntry = NULL;
	std::map<std::string, double> eDep;

	// The TChain is very nice.
	TChain *npolTree = new TChain("T");
	npolTree->Add("/data2/cgen/FirstRun/npolNeutron_1_00*.root");
	//npolTree->Add("/data2/tireman/LongRun/npolRun_1_0001.root");
	// npolTree->Add("/data2/tireman/LongRun/npolRun_1_0002.root");

	npolTree->SetBranchAddress("tracks",&anEntry);

	TFile *outFile = new TFile("dEoverE_p.root","RECREATE");
	TH2F *h_dEoverEtop = new TH2F("dEoverEtop","dE over E",200,0,150,400,0,20);
	TH2F *h_dEoverEbot = new TH2F("dEoverEbot","dE over E",200,0,150,400,0,20);
	TH2F *h_dEoverEplus = new TH2F("dEoverEplus","dE over E",200,0,150,400,0,20);

	int eventsPassed = 0;
	int eventsFailed = 0;

	// loop over all entries (one per event)
	Int_t nentries = npolTree->GetEntries();
	for(int i = 0; i < nentries; i++) {
		npolTree->GetEntry(i);

		if(i % 100 == 0)
			std::cout << "Processing event #" << i << std::endl;

		// loop over vector elements (one per vertex)
		if(anEntry->empty()) continue;
		Int_t nvertices = anEntry->size();

		for (Int_t j = 0; j < nvertices; j++){
			NpolVertex *aVertex = (*anEntry)[j];
			if(aVertex == NULL) continue;
			if(!(aVertex->daughterIds).empty() || aVertex->eMiss) continue;

			if(eDep.find(aVertex->volume) == eDep.end())
				eDep[aVertex->volume] = 0;
			eDep[aVertex->volume] += aVertex->energy;
		}

		if(!EventRequirementsPassed(eDep)) {
			eventsFailed++;
			continue;
		} else
			eventsPassed++;

		double Etop = 0.0;
		double Ebot = 0.0;
		double dEtop = 0.0;
		double dEbot = 0.0;

		std::map<std::string,double>::iterator it2;
		for(it2 = eDep.begin(); it2 != eDep.end(); it2++) {
			int avNum = GetAVNumber(it2->first);
			if(avNum == 0)
				continue;

			switch(avNum) {
				case 1: case 2:
					Etop += it2->second;
					break;			
				case 3: case 4:
					dEtop += it2->second;
					break;
				case 5: case 6:
					Ebot += it2->second;
					break;
				case 7: case 8:
					dEbot += it2->second;
					break;
				default:
					break;
			}		
		}

		h_dEoverEtop->Fill(Etop,dEtop);
		h_dEoverEbot->Fill(Ebot,dEbot);
		h_dEoverEplus->Fill(Etop+Ebot,dEtop+dEbot);

		eDep.clear();
	}

	std::cout << eventsPassed << " events passed requirements.  "
		<< eventsFailed << " failed." << std::endl;

	h_dEoverEtop->Write();
	h_dEoverEbot->Write();
	h_dEoverEplus->Write();
	outFile->Close();

}

