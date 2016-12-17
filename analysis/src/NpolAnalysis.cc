/* Npol Analysis Script is designed to analyze the neutron flux on the NPOL polarimeter being designed by the CGEN 
   collaboration at Jefferson National Laboratory.
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
#define LAYER_NUM 4        /* number of analyzer layers; not general; only good for 4 and 6 layers */

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

// The NPOL polarimeter is divided into 4 or 6 sections.  This function takes a volume name and returns the section number that the detector belongs to, or -1 if the volume does not belong in the polarimeter.
int sectionNumber(const std::string &volName) {
  int avNum, imprNum, pvNum;
  if((avNum = GetAVNumber(volName)) == -1) return 0;

  if(LAYER_NUM == 4){
    switch(avNum) {
    case 1: // Top E array 1 
    case 5: // Bottom E array 1
    case 3: // Top dE array 1 
    case 7: // Bottom dE array 1 
      pvNum = GetPlacementNumber(volName);
      if(pvNum <= 12 && pvNum >= 6) {return 0;}  // section 1
      else if(pvNum <= 5 && pvNum >= 0) {return 1;}  // section 2
      else {return -1;}
    case 2: // Top E array 2
    case 4: // Top dE array 2
    case 6: // Bottom E array 2
    case 8: // Bottom dE array 2
      pvNum = GetPlacementNumber(volName);
      if(pvNum <= 13 && pvNum >= 7) {return 2;}  // section 3
      else if(pvNum <= 6 && pvNum >= 0) {return 3;}  // section 4
      else {return -1;}
    case 9: // Front array 1
    case 11: // Front tag array 1
      imprNum = GetImprNumber(volName);
      if(imprNum == 1) return 0;  // section 1
      else if(imprNum == 2) return 1;  // section 2
      else return -1;
    case 10: // Front array 2
    case 12: // Front tag array 2
      imprNum = GetImprNumber(volName);
      if(imprNum == 1) return 2;  // section 3
      else if(imprNum == 2) return 3;  // section 4
      else return -1;
    default:
      return -1;
    }
  }else if(LAYER_NUM == 6){
    switch(avNum) {
    case 1: // Top E array 1 & 2
    case 5: // Bottom E array 1 & 2
      pvNum = GetPlacementNumber(volName);
      imprNum = GetImprNumber(volName);
      if((imprNum == 1) || (imprNum == 2)){
	if(pvNum <= 12 && pvNum >= 6) return 0;  // section 1
	else if(pvNum <= 5 && pvNum >= 0) return 1;  // section 2
      }else if((imprNum == 3) || (imprNum == 4)){
	if(pvNum <= 12 && pvNum >= 6) return 2;  // section 3
	else if(pvNum <= 5 && pvNum >= 0) return 3;  // section 4
      }else{
	return -1;
      }
    case 3: // Top dE array 1 & 2
    case 7: // Bottom dE array 1 & 2
      pvNum = GetPlacementNumber(volName);
      imprNum = GetImprNumber(volName);
      if(imprNum == 1){
	if(pvNum <= 12 && pvNum >= 6) return 0;  // section 1
	else if(pvNum <= 5 && pvNum >= 0) return 1;  // section 2
      }else if(imprNum == 2){
	if(pvNum <= 12 && pvNum >= 6) return 2;  // section 3
	else if(pvNum <= 5 && pvNum >= 0) return 3;  // section 4
      }else{
	return -1;
      }
    case 2: // Top E array 3
    case 4: // Top dE array 3
    case 6: // Bottom E array 3
    case 8: // Bottom dE array 3
      pvNum = GetPlacementNumber(volName);
      if(pvNum <= 13 && pvNum >= 6) return 4;  // section 5
      else if(pvNum <= 5 && pvNum >= 0) return 5;  // section 6
      else return -1;
    case 9: // Front array 1
    case 12: // Front tag array 1
      imprNum = GetImprNumber(volName);
      if(imprNum == 1) return 0;  // section 1
      else if(imprNum == 2) return 1;  // section 2
      else return -1;
    case 10: // Front array 2
    case 13: // Front tag array 2
      imprNum = GetImprNumber(volName);
      if(imprNum == 1) return 2;  // section 3
      else if(imprNum == 2) return 3;  // section 4
      else return -1;
    case 11:  // Front array 3
    case 14:  // Front tag array 4
      imprNum = GetImprNumber(volName);
      if(imprNum == 1) return 4;  // section 5
      else if(imprNum == 2) return 5;  // section 6
      else return -1;
    default:
      return -1;
    }
  }
}

PolarimeterDetector detectorType(const std::string &volName) {
  int avNum = GetAVNumber(volName);
  if(LAYER_NUM == 4){
    switch(avNum) {
    case 1: case 2: return topEArray;
    case 5: case 6: return botEArray;
    case 3: case 4: return topdEArray;
    case 7: case 8: return botdEArray;
    case 9: case 10: return analyzer;
    case 11: case 12: return tagger;
    case 13: return backPlane;
    default: return unknown;
    }
  }else if(LAYER_NUM == 6){
    switch(avNum) {
    case 1: case 2: return topEArray;
    case 5: case 6: return botEArray;
    case 3: case 4: return topdEArray;
    case 7: case 8: return botdEArray;
    case 9: case 10: case 11: return analyzer;
    case 12: case 13: case 14: return tagger;
    case 15: return backPlane;
    default: return unknown;
    }
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
  for(int section = (LAYER_NUM - 1); section >= 0; section--) {
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

//method that checks if the event is elastic or inelastic and then places it in the 
//  correct histogram
void fillEvent1DHisto(TH1F* elastic, TH1F* inelastic, bool flag, double someInfo) {

  if(flag) elastic->Fill(someInfo);
  else if(!flag) inelastic->Fill(someInfo);

}

void fillEvent2DHisto(TH2F* elastic, TH2F* inelastic, bool flag, double someInfo, double moreInfo) {

  if(flag) elastic->Fill(someInfo, moreInfo);
  else if(!flag) inelastic->Fill(someInfo, moreInfo);

}

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
  if(getenv("HistoOutputDir")){
    HistoDIR = getenv("HistoOutputDir");
  }else{
    HistoDIR = "output";
  }
  
  TString inFilename = InputDir + "/" + BaseName + "_" + JobNum + ".root";
  TString outFilename = HistoDIR + "/" + BaseName + "_NpolEff_" + JobNum + ".root";
  
  std::ofstream txtOut;
  //txtOut.open("/data1/dEoverE_txt_files/" + outFilenamePrefix + ".txt");
  txtOut.open("/dev/null");
  
  TChain *npolTree = new TChain("T");
  TChain *statsTree = new TChain("T2");

  npolTree->SetCacheSize(50000000);
  statsTree->SetCacheSize(50000000);

  npolTree->Add(inFilename);
  statsTree->Add(inFilename);

  std::vector<NpolStep *> *steps = NULL;
  std::vector<NpolVertex *> *verts = NULL;
  std::vector<NpolStatistics *> *stats = NULL;
  npolTree->SetBranchAddress("steps",&steps);
  npolTree->SetBranchAddress("tracks",&verts);
  statsTree->SetBranchAddress("stats",&stats);
  TFile *outFile = new TFile(outFilename,"RECREATE");
  TH2F *h_dEoverE_TopHigh = new TH2F("dEoverE_Top", "dE over E for top array - HIGHEST PV ONLY", 400,0,150,400,0,20);
  TH2F *h_dEoverE_BotHigh = new TH2F("dEoverE_Bot", "dE over E for bottom array - HIGHEST PV ONLY", 400,0,150,400,0,20);
  TH2F *h_dEoverEtop = new TH2F("dEoverEtop", "dE over E for top array", 400,0,150,400,0,20);
  TH2F *h_dEoverEbot = new TH2F("dEoverEbot", "dE over E for bottom array", 400,0,150,400,0,20);
  TH1F *h_sectionEfficiency1 = new TH1F("sectionEfficiency1","Polarimeter section efficiency before cuts",13,0.25,6.75);
  TH1F *h_sectionEfficiency2 = new TH1F("sectionEfficiency2","Polarimeter section efficiency after asymmetry cut",13,0.25,6.75);
  TH1F *h_sectionEfficiency3 = new TH1F("sectionEfficiency3","Polarimeter section efficiency after array energy total cuts",13,0.25,6.75);
  TH1F *h_sectionEfficiency4 = new TH1F("sectionEfficiency4","Polarimeter section efficiency after angle cut",13,0.25,6.75);
  TH1F *h_dTOF = new TH1F("dTOF","Delta time-of-flight",600,-30,120);
  TH1F *h_sectionEfficiencyLocalPositions[LAYER_NUM]; 
  for(int i = 0; i <= (LAYER_NUM-1); i++) {
    std::string title = "Polarimeter Efficiency - Section ";
    title = title + std::to_string(i + 1);
    std::string name = "sectionEfficiencyLocalPosition";
    name = name + std::to_string(i + 1);
    h_sectionEfficiencyLocalPositions[i] = new TH1F(name.c_str(), title.c_str(),200, -5.0, 5.0);
  }
  //elastic ONLY histos
  TH2F *h_dEoverE_TopHigh_Elastic = new TH2F("dEoverE_Top_Elastic", "dE over E for top array - HIGHEST PV ONLY(Elastic Events)", 400,0,150,400,0,20);
  TH2F *h_dEoverE_BotHigh_Elastic = new TH2F("dEoverE_Bot_Elastic", "dE over E for bottom array - HIGHEST PV ONLY(Elastic Events)", 400,0,150,400,0,20);
  TH2F *h_dEoverEtop_Elastic = new TH2F("dEoverEtop_Elastic", "dE over E for top array(Elastic Events)", 400,0,150,400,0,20);
  TH2F *h_dEoverEbot_Elastic = new TH2F("dEoverEbot_Elastic", "dE over E for bottom array(Elastic Events)", 400,0,150,400,0,20);
  TH1F *h_sectionEfficiency1_Elastic = new TH1F("sectionEfficiency1_Elastic","Polarimeter section efficiency before cuts (Elastic Events)",13,0.25,6.75);
  TH1F *h_sectionEfficiency2_Elastic = new TH1F("sectionEfficiency2_Elastic","Polarimeter section efficiency after asymmetry cut (Elastic Events)",13,0.25,6.75);
  TH1F *h_sectionEfficiency3_Elastic = new TH1F("sectionEfficiency3_Elastic","Polarimeter section efficiency after array energy total cuts (Elastic Events)",13,0.25,6.75);
  TH1F *h_sectionEfficiency4_Elastic = new TH1F("sectionEfficiency4_Elastic","Polarimeter section efficiency after angle cut (Elastic Events)",13,0.25,6.75);
  TH1F *h_dTOF_Elastic = new TH1F("dTOF_Elastic","Delta time-of-flight (Elastic Events)",600,-30,120);
  TH1F *h_sectionEfficiencyLocalPositions_Elastic[LAYER_NUM]; 
  for(int i = 0; i <= (LAYER_NUM-1); i++) {
    std::string title = "Polarimeter Efficiency (Elastic Events)- Section ";
    title = title + std::to_string(i + 1);
    std::string name = "sectionEfficiencyLocalPosition_Elastic";
    name = name + std::to_string(i + 1);
    h_sectionEfficiencyLocalPositions_Elastic[i] = new TH1F(name.c_str(), title.c_str(),200, -2.5, 2.5);
  }

  //inelastic ONLY histos
  TH2F *h_dEoverE_TopHigh_Inelastic = new TH2F("dEoverE_Top_Inelastic", "dE over E for top array - HIGHEST PV ONLY(Inelastic Events)", 400,0,150,400,0,20);
  TH2F *h_dEoverE_BotHigh_Inelastic = new TH2F("dEoverE_Bot_Inelastic", "dE over E for bottom array - HIGHEST PV ONLY(Inelastic Events)", 400,0,150,400,0,20);
  TH2F *h_dEoverEtop_Inelastic = new TH2F("dEoverEtop_Inelastic", "dE over E for top array(Inelastic Events)", 400,0,150,400,0,20);
  TH2F *h_dEoverEbot_Inelastic = new TH2F("dEoverEbot_Inelastic", "dE over E for bottom array(Inelastic Events)", 400,0,150,400,0,20);
  TH1F *h_sectionEfficiency1_Inelastic = new TH1F("sectionEfficiency1_Inelastic","Polarimeter section efficiency before cuts (Inelastic Events)",13,0.25,6.75);
  TH1F *h_sectionEfficiency2_Inelastic = new TH1F("sectionEfficiency2_Inelastic","Polarimeter section efficiency after asymmetry cut (Inelastic Events)",13,0.25,6.75);
  TH1F *h_sectionEfficiency3_Inelastic = new TH1F("sectionEfficiency3_Inelastic","Polarimeter section efficiency after array energy total cuts (Inelastic Events)",13,0.25,6.75);
  TH1F *h_sectionEfficiency4_Inelastic = new TH1F("sectionEfficiency4_Inelastic","Polarimeter section efficiency after angle cut (Inelastic Events)",13,0.25,6.75);
  TH1F *h_dTOF_Inelastic = new TH1F("dTOF_Inelastic","Delta time-of-flight (Inelastic Events)",600,-30,120);
  TH1F *h_sectionEfficiencyLocalPositions_Inelastic[LAYER_NUM];
  for(int i = 0; i <= (LAYER_NUM-1); i++) {
    std::string title = "Polarimeter Efficiency(Inelastic Events)  - Section ";
    title = title + std::to_string(i + 1);
    std::string name = "sectionEfficiencyLocalPosition_Inelastic";
    name = name + std::to_string(i + 1);
    h_sectionEfficiencyLocalPositions_Inelastic[i] = new TH1F(name.c_str(), title.c_str(),200, -2.5, 2.5);
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
		
    // END VERTICES LOOP - 
    // !elasticFlag is false (eventsFailed -> inelastic) file -> Elastic
    // elasticFlag is true (eventsFailed -> elastic) file -> Inelastic
    //if(!elasticFlag) {
    //	eventsFailed++;
    //	continue;
    //} 
	  
	  
    /*for(v_it = verts->begin(); v_it != verts->end(); v_it++) { // BEGIN VERTICES LOOP
      NpolVertex *aVertex = *v_it;
      if(aVertex == NULL) continue;
      if((detectorType(aVertex->volume) == analyzer) && (aVertex->parentId == 0)) {
      eventInteraction++;
      break;
      }// END VERTICES LOOP */
	} 

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
      h_sectionEfficiency1->Fill(sectionOfInterest+1); // Fill
	  fillEvent1DHisto(h_sectionEfficiency1_Elastic, h_sectionEfficiency1_Inelastic, elasticFlag, sectionOfInterest+1);
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
		h_sectionEfficiency2->Fill(sectionOfInterest+1); //Fill
		fillEvent1DHisto(h_sectionEfficiency2_Elastic, h_sectionEfficiency2_Inelastic, elasticFlag, sectionOfInterest+1);
	double eDepAnalyzer = eDepArrayTotal[analyzer];
	double eDepE = eDepArrayTotal[EArrayOfInterest];// energy for array filled here 
	double eDepdE = eDepArrayTotal[dEArrayOfInterest];
	double eDepTotal = eDepAnalyzer + eDepE + eDepdE;
	double dTOF = -100.0;
	double dEDepHighest = highestEDepPV(&detEvents, sectionOfInterest, dEArrayOfInterest);
	double eDepHighest = highestEDepPV(&detEvents, sectionOfInterest, EArrayOfInterest);
	if(eDepAnalyzer > 4.0 /*MeV*/ && eDepE > 5.0 /*MeV */ && eDepTotal >= 50.0 /*MeV*/) { // Requirements 3 and 4
	  h_sectionEfficiency3->Fill(sectionOfInterest+1); //FILL
	  fillEvent1DHisto(h_sectionEfficiency3_Elastic, h_sectionEfficiency3_Inelastic, elasticFlag, sectionOfInterest+1);
	  if(CheckAngleRequirement(txtOut,verts->at(1),&detEvents,sectionOfInterest,EArrayOfInterest,&dTOF)) {
	    OutputTracks(verts,txtOut,eventCounter,&eDepArrayTotal,sectionOfInterest);
	    h_sectionEfficiency4->Fill(sectionOfInterest+1); //FILL
		fillEvent1DHisto(h_sectionEfficiency4_Elastic, h_sectionEfficiency4_Inelastic, elasticFlag, sectionOfInterest+1);
	    h_dTOF->Fill(dTOF);//FILL
		fillEvent1DHisto(h_dTOF_Elastic, h_dTOF_Inelastic, elasticFlag, dTOF);
	    sectionEffLocalCoordinates(h_sectionEfficiencyLocalPositions[sectionOfInterest],&detEvents,sectionOfInterest,analyzer);
		sectionEffLocalCoordinates(h_sectionEfficiencyLocalPositions_Elastic[sectionOfInterest],&detEvents,sectionOfInterest,analyzer);
		sectionEffLocalCoordinates(h_sectionEfficiencyLocalPositions_Inelastic[sectionOfInterest],&detEvents,sectionOfInterest,analyzer);
	    if(EArrayOfInterest == topEArray) {// this is where histo for top dE/E gets filled
	      h_dEoverEtop->Fill(eDepE,eDepdE); //FILL
		  fillEvent2DHisto(h_dEoverEtop_Elastic, h_dEoverEtop_Inelastic, elasticFlag, eDepE, eDepdE);
	      h_dEoverE_TopHigh->Fill(eDepHighest, dEDepHighest); //FILL
		  fillEvent2DHisto(h_dEoverE_TopHigh_Elastic, h_dEoverE_TopHigh_Inelastic, elasticFlag, eDepE, eDepdE);
	    } else if(EArrayOfInterest == botEArray) { 
	      h_dEoverEbot->Fill(eDepE,eDepdE);//FILL
		  fillEvent2DHisto(h_dEoverEbot_Elastic, h_dEoverEbot_Inelastic, elasticFlag, eDepE, eDepdE);
	      h_dEoverE_BotHigh->Fill(eDepHighest, dEDepHighest);//FILL
		  fillEvent2DHisto(h_dEoverE_BotHigh_Elastic, h_dEoverE_BotHigh_Inelastic, elasticFlag, eDepE, eDepdE);
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
  h_sectionEfficiency1->Scale(1.0); 
  h_sectionEfficiency2->Scale(1.0); 
  h_sectionEfficiency3->Scale(1.0); 
  h_sectionEfficiency4->Scale(1.0); 
	
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
  h_sectionEfficiency1->Write();
  h_sectionEfficiency2->Write();
  h_sectionEfficiency3->Write();
  h_sectionEfficiency4->Write();
  for(int i = 0; i <= (LAYER_NUM - 1); i++) {
    (h_sectionEfficiencyLocalPositions[i])->Write();
  }
  h_dTOF->Write();
  //write elastic only
  h_dEoverE_TopHigh_Elastic->Write();
  h_dEoverE_BotHigh_Elastic->Write();
  h_dEoverEtop_Elastic->Write();
  h_dEoverEbot_Elastic->Write();
  h_sectionEfficiency1_Elastic->Write();
  h_sectionEfficiency2_Elastic->Write();
  h_sectionEfficiency3_Elastic->Write();
  h_sectionEfficiency4_Elastic->Write();
  for(int i = 0; i <= (LAYER_NUM - 1); i++) {
	(h_sectionEfficiencyLocalPositions_Elastic[i])->Write();
  }
  h_dTOF_Elastic->Write();
  //write inelastic only
  h_dEoverE_TopHigh_Inelastic->Write();
  h_dEoverE_BotHigh_Inelastic->Write();
  h_dEoverEtop_Inelastic->Write();
  h_dEoverEbot_Inelastic->Write();
  h_sectionEfficiency1_Inelastic->Write();
  h_sectionEfficiency2_Inelastic->Write();
  h_sectionEfficiency3_Inelastic->Write();
  h_sectionEfficiency4_Inelastic->Write();
  for(int i = 0; i <= (LAYER_NUM - 1); i++) {
	(h_sectionEfficiencyLocalPositions_Inelastic[i])->Write();
  }
  h_dTOF_Inelastic->Write();
  outFile->Close();
  txtOut.close();
  return 0;
}

