


/* *************IMPORTANT DISCLAIMER*****************
  THIS FILE HAS BEEN MODIFIED TO ACCOMIDATE THE FOUR LAYER POLARIMETER DESIGN
IF YOU WOULD LIKE TO USE THE DATA FOR THE SIX LAYER DESIGN, YOU MUST COPY THE SCRIPT FROM THE FILE "dEoverE_SixLayer" 
     INTO THIS FILE
*/


#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <map>

#include "TVector3.h"
#include <TVectorD.h>
#include "TMath.h"
#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TSystem.h"

#include "NpolVertex.hh"
#include "NpolStep.hh"
#include "NpolStatistics.hh"
#include "NpolDetectorEvent.hh"

#define EDEP_THRESHOLD 1.0 /*MeV*/

enum PolarimeterDetector {
	analyzer = 0,
	tagger,
	topEArray,
	topdEArray,
	botEArray,
	botdEArray,
	backPlane,
	unknown
};

int GetAVNumber(const std::string &volName) {
	if(volName.substr(0,3) == "av_") {
		int underscorePos = volName.find_first_of("_",3);
		return atoi(volName.substr(3,underscorePos-3).c_str());
	} else{
		return -1;
	}
}

int GetImprNumber(const std::string &volName) {
	if(volName.substr(0,3) == "av_") {
		int underscorePos = volName.find_first_of("_",1+
				volName.find_first_of("_",3));
		return atoi(volName.substr(underscorePos+1,1).c_str());
	} else
		return -1;
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
		return -1;
}

// The NPOL polarimeter is divided into four sections.  This function takes a volume name and returns the section number that the detector belongs to, or 0 if the volume does not belong in the polarimeter.
int sectionNumber(const std::string &volName) {
	int avNum, imprNum, pvNum;
	if((avNum = GetAVNumber(volName)) == -1) return 0;

	switch(avNum) {
		case 1: // Top E array 1
		case 3: // Top dE array 1
		case 5: // Bottom E array 1
		case 7: // Bottom dE array 1
			pvNum = GetPlacementNumber(volName);
			if(pvNum <= 12 && pvNum >= 6) return 0;
			else if(pvNum <= 5 && pvNum >= 0) return 1;
			else return -1;
		case 2: // Top E array 2
		case 4: // Top dE array 2
		case 6: // Bottom E array 2
		case 8: // Bottom dE array 2
			pvNum = GetPlacementNumber(volName);
			if(pvNum <= 13 && pvNum >= 7) return 2;
			else if(pvNum <= 6 && pvNum >= 0) return 3;
			else return -1;
		case 9: // Front array 1
		case 11: // Front tag array 1
			imprNum = GetImprNumber(volName);
			if(imprNum == 1) return 0;
			else if(imprNum == 2) return 1;
			else return -1;
		case 10: // Front array 2
		case 12: // Front tag array 2
			imprNum = GetImprNumber(volName);
			if(imprNum == 1) return 2;
			else if(imprNum == 2) return 3;
			else return -1;

		default:
			return -1;
	}
}

PolarimeterDetector detectorType(const std::string &volName) {
	int avNum = GetAVNumber(volName);
	switch(avNum) {
		case 9: case 10: return analyzer;
		case 11: case 12: return tagger;
		case 1: case 2: return topEArray;
		case 5: case 6: return botEArray;
		case 3: case 4: return topdEArray;
		case 7: case 8: return botdEArray;
		case 13: return backPlane;
		default: return unknown;
	}
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
// bottom ones. And vice versa...) This criterion helps suppress inelastic interactions
// and select recoil protons from elastic and quasielastic scatterings of neutrons in the
// polarimeter analyzer.

// Requirement 6: Because the analyzing power is concentrated in the
// range of neutron scattering angles of 4-25 degrees, we applied correspondent cuts on the
// "recoil proton" scattering angle (viz. 45.3-81.6 degrees at Q^2 = 3.95 (GeV/c)^2,
// 40.8-80.2 degrees at 5.22 (GeV/c)^2, and 36.0-78.3 degrees at 6.88 (GeV/c)^2).

// Return the frontmost polarimeter section that passes requirements 1 and 2.
// If -1 is returned, then no section passed requirements 1 and 2.
int getSectionOfInterest(const std::map<std::string,NpolDetectorEvent *> *detEvents) {
	int sectionOfInterest = -1;
	for(int section = 3; section >= 0; section--) {
		std::map<std::string,NpolDetectorEvent *>::const_iterator it;
		bool analyzerFlag = false;
		bool topEArrayFlag = false;
		bool topdEArrayFlag = false;
		bool botEArrayFlag = false;
		bool botdEArrayFlag = false;
		bool taggerFlag = false;
		for(it = detEvents->begin(); it != detEvents->end(); it++) {
			if(sectionNumber(it->first) == section) {
				switch(detectorType(it->first)) {
					case analyzer: analyzerFlag |= it->second->totEnergyDep >= EDEP_THRESHOLD; break;
				    case tagger: taggerFlag |= it->second->totEnergyDep >= EDEP_THRESHOLD; break;
					case topEArray: topEArrayFlag |= it->second->totEnergyDep >= EDEP_THRESHOLD; break;
					case topdEArray: topdEArrayFlag |= it->second->totEnergyDep >= EDEP_THRESHOLD; break;
					case botEArray: botEArrayFlag |= it->second->totEnergyDep >= EDEP_THRESHOLD; break;
					case botdEArray: botdEArrayFlag |= it->second->totEnergyDep >= EDEP_THRESHOLD; break;
					default: break;
				}
			}
		}

		if(analyzerFlag) sectionOfInterest = -1; // If one of this section's analyzers took a hit, then any section after this fails requirement 2.
		if((analyzerFlag && topEArrayFlag && topdEArrayFlag && !taggerFlag) || (analyzerFlag && botEArrayFlag && botdEArrayFlag && !taggerFlag)) sectionOfInterest = section; // If this section passes requirement 1, then it may be the section of interest
	}

	return sectionOfInterest;
}

// If requirement 5 is passed, return the E array of interest (top or bottom).  If requirement 5 does not pass, unknown is returned.
PolarimeterDetector getEArrayOfInterest(std::map<PolarimeterDetector, double> *eDepArrayTotal, int sectionOfInterest) {
	double topETotal = (*eDepArrayTotal)[topEArray];
	double botETotal = (*eDepArrayTotal)[botEArray];
	double topdETotal = (*eDepArrayTotal)[topdEArray];
	double botdETotal = (*eDepArrayTotal)[botdEArray];

	if(topETotal > 20*botETotal && topdETotal > 20*botdETotal)
		return topEArray;
	else if(botETotal > 20*topETotal && botdETotal > 20*topdETotal)
		return botEArray;
	else
		return unknown;
}


double highestEDepPV(const std::map<std::string,NpolDetectorEvent *> *detEvents, int sectionOfInterest, PolarimeterDetector detectorOfInterest) {

  double highestE_Dep = -1.0;

  std::map<std::string,NpolDetectorEvent *>::const_iterator it;

  for(it = detEvents->begin(); it != detEvents->end(); it++) {
    if((sectionOfInterest == sectionNumber(it->first)) && (detectorOfInterest == detectorType(it->first))) { 
      if((it->second->totEnergyDep) > highestE_Dep) 
	highestE_Dep = it->second->totEnergyDep;

    }
  }
  return highestE_Dep;
}

double getTotalEnergyDep(const std::map<std::string,NpolDetectorEvent *> *detEvents) {

  double totEnergyDeposit = -1.0;
  std::map<std::string,NpolDetectorEvent *>::const_iterator it;
  for(it = detEvents->begin(); it != detEvents->end(); it++) {
    if((it->second->totEnergyDep) > 0.0)  {
	totEnergyDeposit += it->second->totEnergyDep;
      }
  }

  return totEnergyDeposit;
}


void sectionEffLocalCoordinates(TH1F *h_sectionEfficiencyLocalPositions, const std::map<std::string,NpolDetectorEvent *> *detEvents, int sectionOfInterest, PolarimeterDetector detector) {


  std::map<std::string,NpolDetectorEvent *>::const_iterator it;
  for(it = detEvents->begin(); it != detEvents->end(); it++) {
    if((sectionNumber(it->first) == sectionOfInterest) && (detectorType(it->first) == detector) && (it->second->thresholdExceeded)){
      h_sectionEfficiencyLocalPositions->Fill(it->second->lPosZ);
    }
  }

}

// Return the azimuthal part of the scattering angle (in degrees) in the polarimeter given three points in global coordinates:
// (p1x,p1y,p1z) - a point along the line of the incoming neutron's trajectory (the vertex in the target will do)
// (p2x,p2y,p2z) - the point where the n,p elastic collision occurs in the analyzer detector
// (p3x,p3y,p3z) - a point along the trajectory of the scattered proton (in the E array)
// The calculation is done by forming a triangle with these points and using the law of cosines
double getAzimuthAngle(std::ofstream &txtOut, const double p1x, const double p1y, const double p1z,
		const double p2x, const double p2y, const double p2z, const double p3x, const double p3y, const double p3z) {
	double s1, s2, s3; // the lengths of the triangle's three sides
	s1 = TMath::Sqrt(TMath::Power(p2x-p1x,2) + TMath::Power(p2y-p1y,2) + TMath::Power(p2z-p1z,2));
	s2 = TMath::Sqrt(TMath::Power(p3x-p2x,2) + TMath::Power(p3y-p2y,2) + TMath::Power(p3z-p2z,2));
	s3 = TMath::Sqrt(TMath::Power(p3x-p1x,2) + TMath::Power(p3y-p1y,2) + TMath::Power(p3z-p1z,2));

	double theta = TMath::ACos((s1*s1 + s2*s2 - s3*s3)/(2.0*s1*s2));
	return 180.0-TMath::RadToDeg()*theta;
}

// Determine the point of interaction (in the global coordinate system) using a clustering approach
// Take the weighted average of the position of the particles that caused detectors in the array to cross their energy threshold, weighted by total energy deposition in each detector
// Pass an array of 3 doubles to hold the x,y,z positions, the section number and detector type identifying the array, and the detEvents map with global position set when the detector first crosses the desired energy threshold
void GetPoI(double *ret, double *time, const int section, const PolarimeterDetector type, const std::map<std::string,NpolDetectorEvent *> *detEvents) {
	std::map<std::string,NpolDetectorEvent *>::const_iterator it;

	ret[0] = 0.0;
	ret[1] = 0.0;
	ret[2] = 0.0;
	double totEdepSoFar = 0.0;
	for(it = detEvents->begin(); it != detEvents->end(); it++) {
		if(sectionNumber(it->first) == section && detectorType(it->first) == type) {
			if(it->second->thresholdExceeded) {
				ret[0] += (it->second->totEnergyDep)*(it->second->gPosX);
				ret[1] += (it->second->totEnergyDep)*(it->second->gPosY);
				ret[2] += (it->second->totEnergyDep)*(it->second->gPosZ);
				*time += (it->second->totEnergyDep)*(it->second->time);
				totEdepSoFar += it->second->totEnergyDep;
			}
		}
	}

	// Compute the weighted average
	ret[0] /= totEdepSoFar;
	ret[1] /= totEdepSoFar;
	ret[2] /= totEdepSoFar;
	*time /= totEdepSoFar;
}

// Check requirement 6
// Optionally return delta time-of-flight between the analyzer and E-array
bool CheckAngleRequirement(std::ofstream &txtOut, NpolVertex *incNeutronVert, std::map<std::string,NpolDetectorEvent *> *detEvents, const int section, const PolarimeterDetector EArrayOfInterest, double *dTOF = NULL) {
	double targetPt[3];
	double analyzerPt[3];
	double EarrayPt[3];
	double analyzerTime;
	double EarrayTime;

	targetPt[0] = incNeutronVert->posX;
	targetPt[1] = incNeutronVert->posY;
	targetPt[2] = incNeutronVert->posZ;

	GetPoI(analyzerPt,&analyzerTime,section,analyzer,detEvents);
	GetPoI(EarrayPt,&EarrayTime,section,EArrayOfInterest,detEvents);

	double azAngle = getAzimuthAngle(txtOut,targetPt[0],targetPt[1], targetPt[2],
			analyzerPt[0],analyzerPt[1],analyzerPt[2],
			EarrayPt[0],EarrayPt[1],EarrayPt[2]);

	txtOut << azAngle << " deg" << std::endl;
	//	std::cout << "(" << targetPt[0] << "," << targetPt[1] << "," << targetPt[2] << ")   "
	//		<< "(" << analyzerPt[0] << "," << analyzerPt[1] << "," << analyzerPt[2] << ")   "
	//		<< "(" << EarrayPt[0] << "," << EarrayPt[1] << "," << EarrayPt[2] << ")   "
	//		<< azAngle << " " << std::endl;
	//	if(azAngle > 90.0)
	//		std::cout << azAngle << " " << std::endl;

	if(dTOF != NULL)
		*dTOF = EarrayTime - analyzerTime;
	return (azAngle > 45.3 && azAngle < 81.6);
}

void OutputTracks(const std::vector<NpolVertex *> *verticies, std::ofstream &txtOut, int eventNo, const std::map<PolarimeterDetector,double> *eDepArrayTotal, int sectionOfInterest) {

	std::map<PolarimeterDetector, double>::const_iterator e_it;
	txtOut << std::endl << eventNo << ": " << sectionOfInterest << std::endl;

	for(e_it = eDepArrayTotal->begin(); e_it != eDepArrayTotal->end(); e_it++)
		txtOut << e_it->first << ";" << e_it->second << std::endl;

	// BEGIN VERTICES LOOP
	std::vector<NpolVertex *>::const_iterator v_it;
	for(v_it = verticies->begin(); v_it != verticies->end(); v_it++) {
		NpolVertex *aVertex = *v_it;
		if(aVertex == NULL) continue;

		if((aVertex->volume).substr(0,3) == "av_")
			//	if(detectorType(aVertex->volume) == analyzer)
			txtOut << "#" << aVertex->parentId << " " << aVertex->particle << ": " << aVertex->process << " in volume " << aVertex->volume << std::endl;

	} // END VERTICES LOOP
}

/**************IMPORTANT DISCLAIMER*****************

  THIS FILE HAS BEEN MODIFIED TO ACCOMIDATE THE FOUR LAYER POLARIMETER DESIGN
IF YOU WOULD LIKE TO USE THE DATA FOR THE SIX LAYER DESIGN, YOU MUST COPY THE SCRIPT FROM THE FILE "dEoverE_SixLayer" 
     INTO THIS FILE

*/


//int dEoverE() {
  //gSystem->Load("../../build/npollib/libNpolClasses.so");
int main(int argc, char *argv[]) {
  
  std::string JobNum;
  if(getenv("JOBNUMBER")){
    JobNum = getenv("JOBNUMBER");
  }else{
    JobNum = "99999"; // default job number is 99999
  }
  
  std::string InputDir;
  if(getenv("RawDataDir")){
    InputDir = getenv("RawDataDir");
  }else{
    InputDir = "output"; 
  }
  
  std::string BaseName;
  if(getenv("NPOLBASENAME")){
    BaseName = getenv("NPOLBASENAME");
  }else{
    BaseName = "output"; 
  }
  
  std::string HistoDIR;
  if(getenv("OutputDir")){
	HistoDIR = getenv("OutputDir");
  }else{
	HistoDIR = "output";
  }
  
  TString inFilename = InputDir + "/" + BaseName + "_" + JobNum + ".root";
  TString outFilename = HistoDIR + "/" + BaseName + "_NpolEff_" + JobNum + ".root";

  //TString inFilename ="/data1/cgen/NeutronOnly/RealisticSource/10cmScint/root/sourceNeutron_Lead15cm_4.4GeV_4Bdl_51.root";
  //TString outFilename ="/data1/cgen/NeutronOnly/RealisticSource/10cmScint/histos/output.root";


	std::ofstream txtOut;
	//txtOut.open("/data1/dEoverE_txt_files/" + outFilenamePrefix + ".txt");
	txtOut.open("/dev/null");

	TChain *npolTree = new TChain("T");
	TChain *statsTree = new TChain("T2");

	npolTree->SetCacheSize(500000000);
	statsTree->SetCacheSize(500000000);

	npolTree->Add(inFilename);
	statsTree->Add(inFilename);

	std::vector<NpolStep *> *steps = NULL;
	std::vector<NpolVertex *> *verts = NULL;
	std::vector<NpolStatistics *> *stats = NULL;
	npolTree->SetBranchAddress("steps",&steps);
	npolTree->SetBranchAddress("tracks",&verts);
	statsTree->SetBranchAddress("stats",&stats);
	TFile *outFile = new TFile(outFilename,"RECREATE");
	TH2F *h_dEoverE_TopHigh = new TH2F("dEoverE_Top", "dE over E for top array - HIGHEST PV ONLY", 200,0,150,400,0,20);
	TH2F *h_dEoverE_BotHigh = new TH2F("dEoverE_Bot", "dE over E for bottom array - HIGHEST PV ONLY", 200,0,150,400,0,20);
	TH2F *h_dEoverEtop = new TH2F("dEoverEtop", "dE over E for top array", 200,0,150,400,0,20);
	TH2F *h_dEoverEbot = new TH2F("dEoverEbot", "dE over E for bottom array", 200,0,150,400,0,20);
	TH2F *h_dEoverEtop3 = new TH2F("dEoverEtop3", "dE over E for top array in section 3", 200,0,150,400,0,20);
	TH2F *h_dEoverEbot3 = new TH2F("dEoverEbot3", "dE over E for bottom array in section 3", 200,0,150,400,0,20);
	TH1F *h_sectionEfficiency1 = new TH1F("sectionEfficiency1","Polarimeter section efficiency before cuts",6,0,5);
	TH1F *h_sectionEfficiency2 = new TH1F("sectionEfficiency2","Polarimeter section efficiency after asymmetry cut",6,0,5);
	TH1F *h_sectionEfficiency3 = new TH1F("sectionEfficiency3","Polarimeter section efficiency after array energy total cuts",6,0,5);
	TH1F *h_sectionEfficiency4 = new TH1F("sectionEfficiency4","Polarimeter section efficiency after angle cut",6,0,5);
	TH1F *h_dTOF = new TH1F("dTOF","Delta time-of-flight",600,-30,30);
	TH1F *h_sectionEfficiencyLocalPositions[4]; 
	for(int i = 0; i <= 3; i++) {
	  std::string title = "Polarimeter Efficiency - Section ";
	  title = title + std::to_string(i + 1);
	  std::string name = "sectionEfficiencyLocalPosition";
	  name = name + std::to_string(i + 1);
	  h_sectionEfficiencyLocalPositions[i] = new TH1F(name.c_str(), title.c_str(),200, -5.0, 5.0);
	}
	
	// BEGIN STATS LOOP
	int totalEvents = 0;
	int eventInteraction = 0;
	for(int i = 0; i < statsTree->GetEntries(); i++) {
	  statsTree->GetEntry(i);
	  totalEvents += ((*stats)[0])->totalEvents;
	}// END STATS LOOP
	std::cout << totalEvents << " neutrons on polarimeter." << std::endl;
	
	int eventCounter = 0;
	int eventsPassed = 0;
	int eventsFailed = 0;
	// BEGIN EVENT LOOP
	int nentries = npolTree->GetEntries();
	for(int i = 0; i < nentries; i++) {
	  //for(int i = 0; i < 100; i++) {
	  if(++eventCounter % 1000 == 0)
	    std::cout << "Processing event #" << eventCounter << std::endl;
	  npolTree->GetEntry(i);
	  std::map<std::string,NpolDetectorEvent *> detEvents;
	  // BEGIN VERTICES LOOP  run through two times with both only elastic and inelastic collisions - RENAME THE OUTPUT FILES EACH TIME! 
	  bool elasticFlag = false;
	  std::vector<NpolVertex *>::const_iterator v_it;
	  for(v_it = verts->begin(); v_it != verts->end(); v_it++) {
	    NpolVertex *aVertex = *v_it;
		if(aVertex == NULL) continue;
		
		if(aVertex->parentId == 1 && aVertex->process == "hadElastic" && aVertex->particle == "proton") {
	      elasticFlag = true;
	      break;
	    }
		
	  } 
	  // END VERTICES LOOP - 
	  // !elasticFlag is false (eventsFailed -> inelastic) file -> Elastic
	  // elasticFlag is true (eventsFailed -> elastic) file -> Inelastic
	  if(!elasticFlag) {
		eventsFailed++;
	    continue;
	  } 
	  
	  
	  /*for(v_it = verts->begin(); v_it != verts->end(); v_it++) { // BEGIN VERTICES LOOP
	    NpolVertex *aVertex = *v_it;
	    if(aVertex == NULL) continue;
	    if((detectorType(aVertex->volume) == analyzer) && (aVertex->parentId == 0)) {
	      eventInteraction++;
	      break;
	    }
		} // END VERTICES LOOP */

		// BEGIN STEPS LOOP
	  std::vector<NpolStep *>::iterator s_it;
	  for(s_it = steps->begin(); s_it != steps->end(); s_it++) {
		NpolStep *aStep = *s_it;
		
		if(detEvents.find(aStep->volume) == detEvents.end())
		  detEvents[aStep->volume] = new NpolDetectorEvent();
		
		(detEvents[aStep->volume])->totEnergyDep += aStep->eDep;
		
		if(!(detEvents[aStep->volume])->thresholdExceeded && (detEvents[aStep->volume])->totEnergyDep >= EDEP_THRESHOLD) {
		  (detEvents[aStep->volume])->thresholdExceeded = true;
		  (detEvents[aStep->volume])->lPosX = aStep->lPosX;
		  (detEvents[aStep->volume])->gPosX = aStep->gPosX;
		  (detEvents[aStep->volume])->gPosY = aStep->gPosY;
		  (detEvents[aStep->volume])->gPosZ = aStep->gPosZ;
		  (detEvents[aStep->volume])->lPosZ = aStep->lPosZ;
		  (detEvents[aStep->volume])->time = aStep->time;
		}
	  } // END STEPS LOOP
	  
	  std::map<std::string,NpolDetectorEvent *>::iterator e_it;
	  //double totalEnergyDep = getTotalEnergyDep(&detEvents);
	  int sectionOfInterest = getSectionOfInterest(&detEvents);
	  std::map<PolarimeterDetector, double> eDepArrayTotal;
	  eDepArrayTotal[analyzer] = 0.0;
	  eDepArrayTotal[tagger] = 0.0;
	  eDepArrayTotal[topEArray] = 0.0;
	  eDepArrayTotal[topdEArray] = 0.0;
	  eDepArrayTotal[botEArray] = 0.0;
	  eDepArrayTotal[botdEArray] = 0.0;
	  if(sectionOfInterest != -1) {
		eventInteraction++;
		h_sectionEfficiency1->Fill(sectionOfInterest+1);
		for(e_it = detEvents.begin(); e_it != detEvents.end(); e_it++) {
		  if(sectionNumber(e_it->first) == sectionOfInterest) {
			PolarimeterDetector detector = detectorType(e_it->first);
			if(detector == analyzer || detector == tagger || detector == topEArray || detector == topdEArray || detector == botEArray || detector == botdEArray) {
			  eDepArrayTotal[detector] += e_it->second->totEnergyDep;
			}
		  }
		}
		
		PolarimeterDetector EArrayOfInterest = getEArrayOfInterest(&eDepArrayTotal,sectionOfInterest);
		PolarimeterDetector dEArrayOfInterest;
		
		
		if(EArrayOfInterest == topEArray) {
		  dEArrayOfInterest = topdEArray;
		}
		else {
		  dEArrayOfInterest = botdEArray;
		}
		if(EArrayOfInterest != unknown) {
		  h_sectionEfficiency2->Fill(sectionOfInterest+1);
		  double eDepAnalyzer = eDepArrayTotal[analyzer];
		  double eDepE = eDepArrayTotal[EArrayOfInterest];// energy for array filled here 
		  double eDepdE = eDepArrayTotal[dEArrayOfInterest];
		  double eDepTotal = eDepAnalyzer + eDepE + eDepdE;
		  double dTOF = -100.0;
		  double dEDepHighest = highestEDepPV(&detEvents, sectionOfInterest, dEArrayOfInterest);
		  double eDepHighest = highestEDepPV(&detEvents, sectionOfInterest, EArrayOfInterest);
		  if(eDepAnalyzer > 4.0 /*MeV*/ && eDepE > 5.0 /*MeV */ && eDepTotal >= 50.0 /*MeV*/) { // Requirements 3 and 4
			h_sectionEfficiency3->Fill(sectionOfInterest+1);
			if(CheckAngleRequirement(txtOut,verts->at(1),&detEvents,sectionOfInterest,EArrayOfInterest,&dTOF)) {
			  OutputTracks(verts,txtOut,eventCounter,&eDepArrayTotal,sectionOfInterest);
			  h_sectionEfficiency4->Fill(sectionOfInterest+1);
			  h_dTOF->Fill(dTOF);
			  sectionEffLocalCoordinates(h_sectionEfficiencyLocalPositions[sectionOfInterest],&detEvents,sectionOfInterest,analyzer);
			  if(EArrayOfInterest == topEArray) {// this is where histo for top dE/E gets filled
				h_dEoverEtop->Fill(eDepE,eDepdE);
				h_dEoverE_TopHigh->Fill(eDepHighest, dEDepHighest);
				if(sectionOfInterest == 3)
				  h_dEoverEtop3->Fill(eDepE,eDepdE);
			  } else if(EArrayOfInterest == botEArray) { // bottom dE/E gets filled
				h_dEoverEbot->Fill(eDepE,eDepdE);
				h_dEoverE_BotHigh->Fill(eDepHighest, dEDepHighest);
				if(sectionOfInterest == 3)
				  h_dEoverEbot3->Fill(eDepE,eDepdE);
			  }
			  
			  eventsPassed++;
			} else eventsFailed++;// TVector3 no longer works
		  } else eventsFailed++;
		} else eventsFailed++;
	  } else eventsFailed++;
	  
	  if(sectionOfInterest != -1) eventsPassed ++;
	  
	  // Clear out the map for the next event
	  eDepArrayTotal.clear();
	  for(e_it = detEvents.begin(); e_it != detEvents.end(); e_it++)
		delete e_it->second;
	  detEvents.clear();
	} // END EVENT LOOP
	
	std::cout << eventsPassed << " events passed requirements.  "
			  << eventsFailed << " failed." << std::endl;
	h_sectionEfficiency1->Scale(1.0); ///eventInteraction);
	h_sectionEfficiency2->Scale(1.0); ///eventInteraction);
	h_sectionEfficiency3->Scale(1.0); ///eventInteraction);
	h_sectionEfficiency4->Scale(1.0); ///eventInteraction);
	
	TVectorD runStatistics(4);
	runStatistics[0] = totalEvents;
	runStatistics[1] = eventInteraction;
	runStatistics[2] = eventsPassed;
	runStatistics[3] = eventsFailed;
	runStatistics.Write();
	h_dEoverE_TopHigh->Write();
	h_dEoverE_BotHigh->Write();
	h_dEoverEtop->Write();
	h_dEoverEbot->Write();
	h_dEoverEtop3->Write();
	h_dEoverEbot3->Write();
	h_sectionEfficiency1->Write();
	h_sectionEfficiency2->Write();
	h_sectionEfficiency3->Write();
	h_sectionEfficiency4->Write();
	for(int i = 0; i <= 3; i++) {
	  (h_sectionEfficiencyLocalPositions[i])->Write();
	}
	h_dTOF->Write();
	outFile->Close();
	txtOut.close();
	return 0;
}
