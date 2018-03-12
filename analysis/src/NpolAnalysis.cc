/* Npol Analysis Script is designed to analyze the neutron flux on the NPOL polarimeter being
   designed by the CGEN collaboration at Jefferson National Laboratory. (2016)
   Revisions: Spring 2017 by Will Tireman and Ashley Adzima (added some histograms)
   Revisions: January-March 2018 by Will Tireman (Fixed eff. calculation, cleaned up code)
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
#include "TString.h"
#include "TRandom.h"

#include "NpolVertex.hh"
#include "NpolTagger.hh"
#include "NpolStep.hh"
#include "NpolStatistics.hh"
#include "NpolDetectorEvent.hh"


//********************** Definition of methods which come after the Main() code *********************//

#define EDEP_THRESHOLD 1.0  /*MeV*/
#define LOW_THRESHOLD 0.040 /*MeV This threshold is a per detector low value in SOI selection */
#define LAYER_NUM 4         /* number of analyzer layers; not general; only good for 4 and 6 layers */
#define angleLow 45.3       /*degrees: low angle recoil proton cut*/
#define angleHigh 81.6      /*degrees; high angle recoil proton cut*/
#define NpolAng 0.48869     /*radians; angle of NPOL relative to beam axis*/

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

TString BaseName = "";
TString JobNum = "";
TString Lead = ""; 
TString Energy = "";
TString Bfield = "";
TString OutputDir = "";
TString InputDir = "";
TString CType = "";

void HitPosCompute(double hitPos[]);
void AnalyzerTaggerHitPosition(double hitPos[],double lPos[],int detNums[]);
void DeltaEarrayHitPosition(double hitPos[],double lPos[],int detNums[]);
void EarrayHitPosition(double hitPos[],double lPos[],int detNums[]);
void RotateNpolToG4(double hitPos[],double RotationAngle);
void RotateDetToNpol(double hitPos[],int detNums[]);
void RetrieveENVvariables();
TString FormInputFile(TString InputDir);
TString FormOutputFile(TString OutputDir);
int GetAVNumber(const std::string &volName);
int GetImprNumber(const std::string &volName);
int GetPlacementNumber(const std::string &volName);
int sectionNumber(const std::string &volName);
PolarimeterDetector detectorType(const std::string &volName);
bool checkEarrayHits(const std::map<std::string,NpolDetectorEvent *> *detEvents);
bool checkdEarrayHits(const std::map<std::string,NpolDetectorEvent *> *detEvents);
int getSectionOfInterest(const std::map<std::string,NpolDetectorEvent *> *detEvents);
PolarimeterDetector getEArrayOfInterest(std::map<PolarimeterDetector, double> *eDepArrayTotal, int sectionOfInterest);
double getTotalEnergyDep(const std::map<std::string,NpolDetectorEvent *> *detEvents);
void sectionEffLocalCoordinates(TH1F *h_sectionEfficiencyLocalPositions, const std::map<std::string,NpolDetectorEvent *>
								*detEvents, int sectionOfInterest, PolarimeterDetector detector);
double getAzimuthAngle(const double p1x, const double p1y, const double p1z,const double p2x, const double p2y, 
					   const double p2z, const double p3x, const double p3y, const double p3z);
void GetPoI(double *ret, double *time, const int section, const PolarimeterDetector type, 
			const std::map<std::string,NpolDetectorEvent *> *detEvents); 
void GetPoI2(double *ret, double *time, const int section, const PolarimeterDetector type, 
			 const std::map<std::string,NpolDetectorEvent *> *detEvents);
double ReturnAngle(NpolVertex *incNeutronVert, std::map<std::string, NpolDetectorEvent *> *detEvents,
				   const int section, const PolarimeterDetector EArrayOfInterest, double *dTOF ); 

//***************** End Definitions of Variables and Methods ***********************//

//************************ Main Program Begins Here ********************************//

int main(int argc, char *argv[]) {
  
  RetrieveENVvariables();

  TString InputFile = FormInputFile(InputDir);
  TString OutputFile = FormOutputFile(OutputDir);

  TFile *outFile = new TFile(OutputFile,"RECREATE"); 
  
  std::ofstream txtOut;
  txtOut.open("HitPositions_" + JobNum + ".txt");
  
  TChain *npolTree = new TChain("T");
  TChain *statsTree = new TChain("T2");

  npolTree->Add(InputFile);
  statsTree->Add(InputFile);

  npolTree->SetCacheSize(250000000);
  statsTree->SetCacheSize(250000000);
  
  std::vector<NpolStep *> *steps = NULL;
  std::vector<NpolVertex *> *verts = NULL;
  std::vector<NpolTagger *> *tagEvent = NULL;
  std::vector<NpolStatistics *> *stats = NULL;
  npolTree->SetBranchAddress("steps",&steps);
  npolTree->SetBranchAddress("tracks",&verts);
  npolTree->SetBranchAddress("NPOLTagger",&tagEvent);
  statsTree->SetBranchAddress("stats",&stats);
  
  //********************************* Define your Histograms Here *******************************
  TH1F *h_recoilAngle = new TH1F("recoil_angle","Proton Recoil Angle", 100, 0.0, 90.0); 
  TH1F *h_recoilAngle_Raw = new TH1F("recoil_angle_raw","Proton Recoil Angle Before Angle Cut", 100, 0.0, 90.0);
  TH2F *h_dEoverEtop = new TH2F("dEoverEtop", "dE over E for top array", 400,0,150,400,0,20);
  TH2F *h_dEoverEbot = new TH2F("dEoverEbot", "dE over E for bottom array", 400,0,150,400,0,20);
  TH1F *h_sectionEfficiency1 = new TH1F("sectionEfficiency1","Polarimeter section efficiency before cuts",13,0.25,6.75);
  TH1F *h_sectionEfficiency2 = new TH1F("sectionEfficiency2","Polarimeter section efficiency after asymmetry cut",13,0.25,6.75);
  TH1F *h_sectionEfficiency3 = new TH1F("sectionEfficiency3","Polarimeter section efficiency after array energy total cuts",13,0.25,6.75);
  TH1F *h_sectionEfficiency4 = new TH1F("sectionEfficiency4","Polarimeter section efficiency after angle cut",13,0.25,6.75);
  TH1F *h_dTOF = new TH1F("dTOF","Delta time-of-flight",600,-30,120);
  //********************************* End Histogram Definitions ********************************

  // BEGIN STATS LOOP
  int totalEvents = 0;
  int eventsPassed = 0;
  int eventsFailed = 0;
  int taggedEvents = 0;
  for(int i = 0; i < statsTree->GetEntries(); i++) {
    statsTree->GetEntry(i);
    totalEvents += ((*stats)[0])->totalEvents;
  }// END STATS LOOP
  std::cout << totalEvents << " neutrons thrown at setup." << std::endl;
	
  // BEGIN EVENT LOOP
  int nentries = npolTree->GetEntries();
  for(int i = 0; i < nentries; i++) {
    //for(int i = 0; i < 100; i++) {
    //if(++eventCounter % 1000 == 0)
	if(i % 1000 == 0)
      std::cout << "Processing event #" << i << std::endl;
    npolTree->GetEntry(i);
    std::map<std::string,NpolDetectorEvent *> detEvents;

    // BEGIN STEPS LOOP: Fills the detEvent map with volumes and total energy, etc.
    std::vector<NpolStep *>::iterator s_it;	
    //bool tripFlag = false;
    bool eventFlag = false;
    for(s_it = steps->begin(); s_it != steps->end(); s_it++) {
      NpolStep *aStep = *s_it;
	// These lines of code count up the number of particles entering into the 
	// first analyzer array that are a neutron (2112), parent ID = 0, track ID = 1.
	  int AVNum = GetAVNumber(aStep->volume);
	  if(AVNum == -1) continue;
	  int imprintNum = GetImprNumber(aStep->volume);
	  int PVNum = GetPlacementNumber(aStep->volume);
	  if((!eventFlag) && (aStep->parentId == 0) && (aStep->trackId == 1) && (aStep->particleId == 2112) 
		&& (AVNum ==11) && (imprintNum == 1)) {
		taggedEvents++;
		eventFlag = true;
	  }
		
      if(detEvents.find(aStep->volume) == detEvents.end())
		detEvents[aStep->volume] = new NpolDetectorEvent();
	  
      (detEvents[aStep->volume])->totEnergyDep += aStep->eDep;
	  
      if(!(detEvents[aStep->volume])->thresholdExceeded && (detEvents[aStep->volume])->totEnergyDep >= EDEP_THRESHOLD) {
		gRandom = new TRandom();
		
		(detEvents[aStep->volume])->thresholdExceeded = true;
		(detEvents[aStep->volume])->lPosX = aStep->lPosX;
		(detEvents[aStep->volume])->lPosY = aStep->lPosY;
		(detEvents[aStep->volume])->lPosZ = aStep->lPosZ;
		(detEvents[aStep->volume])->gPosX = aStep->gPosX;
		(detEvents[aStep->volume])->gPosY = aStep->gPosY;
		(detEvents[aStep->volume])->gPosZ = aStep->gPosZ;
		(detEvents[aStep->volume])->time = (aStep->time) + gRandom->Gaus(0.0, 0.200);

		//****** Compute the hit position in the volume and save. This is done to "simulate" ****** // 
		// what we could see in a real scintillation detector based on detector resolutions.
		double hitPos[3] = { 0.0, 0.0, 0.0 };
		double lPos[3] = { aStep->lPosX, aStep->lPosY, aStep->lPosZ };
		int detNums[3] = { AVNum, imprintNum, PVNum };
			
		if((AVNum == 9) || (AVNum == 10) || (AVNum == 11) || (AVNum == 12)){
		  AnalyzerTaggerHitPosition(hitPos, lPos, detNums);
		} else if((AVNum == 3) || (AVNum == 4) || (AVNum == 7) || (AVNum == 8)){
		  DeltaEarrayHitPosition(hitPos, lPos, detNums);
		} else if((AVNum == 1) || (AVNum == 2) || (AVNum == 5) || (AVNum == 6)){
		  EarrayHitPosition(hitPos, lPos, detNums);
		}
		
		(detEvents[aStep->volume])->hPosX = hitPos[0]; 
		(detEvents[aStep->volume])->hPosY = hitPos[1]; 
		(detEvents[aStep->volume])->hPosZ = hitPos[2]; 
		txtOut << hitPos[0] << "\t\t" << hitPos[1] << "\t\t" << hitPos[2] << "\t\t" <<
		  (detEvents[aStep->volume])->time << std::endl; // write to text file to read with MatLab

		// ****** End of the hit position computations section ******* //
	  }
    } // END STEPS LOOP
	

	// Here, get Section of Interest (SOI) and then begin checking further requirements using detEvents map
	// of event data for the event.
    std::map<std::string,NpolDetectorEvent *>::iterator e_it;
    //double totalEnergyDep = getTotalEnergyDep(&detEvents);
    int sectionOfInterest = getSectionOfInterest(&detEvents); // determine SOI

	// Total energy map for each array; currently only does analyzer, analyzer taggers and top/bottom E and dE arrays
    std::map<PolarimeterDetector, double> eDepArrayTotal;     
    eDepArrayTotal[analyzer] = 0.0;
    eDepArrayTotal[tagger] = 0.0;
    eDepArrayTotal[topEArray] = 0.0;
    eDepArrayTotal[topdEArray] = 0.0;
    eDepArrayTotal[botEArray] = 0.0;
    eDepArrayTotal[botdEArray] = 0.0;

    if(sectionOfInterest != -1) {
	  h_sectionEfficiency1->Fill(sectionOfInterest+1); // Fill

	  
	  // Changed this on 12-March-2017 to test theory on the Recoil Proton Angle "question" by not requiring an
	  // E or dE detector be in a particular "geometric section (vertical slice).
	  // This allows E/dE detectors outside the "geometric" SOI to be counted as well.  It worked! (well, helped)
	  // This part of the "tracking" needs some work ... lots of work.
      for(e_it = detEvents.begin(); e_it != detEvents.end(); e_it++) {	
		
		PolarimeterDetector detector2 = detectorType(e_it->first);
		if(detector2 == topEArray || detector2 == topdEArray || 
		   detector2 == botEArray || detector2 == botdEArray){
		  eDepArrayTotal[detector2] += e_it->second->totEnergyDep;
		}
				
		if(sectionNumber(e_it->first) == sectionOfInterest) {
		  PolarimeterDetector detector = detectorType(e_it->first);
		  if(detector == analyzer || detector == tagger) {	
			//|| detector == topEArray || detector == topdEArray || detector == botEArray || detector == botdEArray){ // see note above
			eDepArrayTotal[detector] += e_it->second->totEnergyDep;
		  }
		}
	  }
	  
	  // Get the EArray of Interest (EOI) based on Requirement 5 of a ratio of 20:1 between the top and bottom arrays
	  // Not convinced this is the most prudent way to do it but it is quick to implement
      PolarimeterDetector EArrayOfInterest = getEArrayOfInterest(&eDepArrayTotal,sectionOfInterest);
      PolarimeterDetector dEArrayOfInterest;	  
	  
      if(EArrayOfInterest == topEArray) {
		dEArrayOfInterest = topdEArray;
      }
      else if(EArrayOfInterest == botEArray){
		dEArrayOfInterest = botdEArray;
      }

      if(EArrayOfInterest != unknown) {
		h_sectionEfficiency2->Fill(sectionOfInterest+1); //Fill
		double eDepAnalyzer = eDepArrayTotal[analyzer];
		double eDepE = eDepArrayTotal[EArrayOfInterest];// energy for array filled here 
		double eDepdE = eDepArrayTotal[dEArrayOfInterest];
		double eDepTotal = eDepAnalyzer + eDepE + eDepdE;
		double dTOF = -100.0;
		double azAngle = ReturnAngle(verts->at(1),&detEvents,sectionOfInterest,EArrayOfInterest,&dTOF);

		if(eDepAnalyzer >= 4.0 /*MeV*/ && eDepE >= 5.0 /*MeV */ && eDepTotal >= 50.0 /*MeV*/) { // Requirements 3 and 4
		  h_sectionEfficiency3->Fill(sectionOfInterest+1); //FILL
		  h_recoilAngle_Raw->Fill(azAngle);
		  if (azAngle >= angleLow && azAngle <= angleHigh){  // checks Requirement 6 
			h_sectionEfficiency4->Fill(sectionOfInterest+1); //FILL
			h_dTOF->Fill(dTOF); //FILL
			 h_recoilAngle->Fill(azAngle); // Fill the proton recoil angle histo
			if(EArrayOfInterest == topEArray) {// this is where histo for top dE/E gets filled
			  h_dEoverEtop->Fill(eDepE,eDepdE); //FILL
			} else if(EArrayOfInterest == botEArray) { 
			  h_dEoverEbot->Fill(eDepE,eDepdE);//FILL
			}
			
			eventsPassed++;
		  } else eventsFailed++;
		} else eventsFailed++;
      } else eventsFailed++;
    } else eventsFailed++;
	
    // Clear out the map for the next event
    eDepArrayTotal.clear();
    for(e_it = detEvents.begin(); e_it != detEvents.end(); e_it++)
      delete e_it->second;
    detEvents.clear();
  } // END EVENT LOOP
  
  std::cout << taggedEvents << " of the initial " << totalEvents << " neutrons have crossed the Npol Tagger." << std::endl;
  std::cout << eventsPassed << " events passed requirements.  "
			<< (taggedEvents - eventsPassed) << " failed." << std::endl;
  std::cout << (((double)eventsPassed)*100)/(double)taggedEvents << " % of the " << taggedEvents
			<< " neutrons passed cuts." << std::endl; 
  /*h_sectionEfficiency1->Scale(1.0); 
  h_sectionEfficiency2->Scale(1.0); 
  h_sectionEfficiency3->Scale(1.0); 
  h_sectionEfficiency4->Scale(1.0); */

  // **** Fill the statistics vector ****** //
  TVectorD runStatistics(6);
  runStatistics[0] = totalEvents;
  runStatistics[1] = taggedEvents;
  runStatistics[2] = eventsPassed;
  runStatistics[3] = eventsFailed;
  runStatistics[4] = -1.0;         
  runStatistics[5] = -1.0;

  // ****** Write out all the results to the root file and close the file(s) ******** //
  runStatistics.Write();
  h_dEoverEtop->Write();
  h_dEoverEbot->Write();
  h_sectionEfficiency1->Write();
  h_sectionEfficiency2->Write();
  h_sectionEfficiency3->Write();
  h_sectionEfficiency4->Write();
  h_dTOF->Write();
  h_recoilAngle->Write();
  h_recoilAngle_Raw->Write();
  outFile->Close();
  txtOut.close();
  return 0;
}

// ************** End Main Program here *************** //

// ************* Methods for Main Program are below  this line ************** //

void AnalyzerTaggerHitPosition(double hPos[],double lPos[], int detNums[]){

  gRandom = new TRandom();
  do{
	hPos[0] = lPos[0] + gRandom->Gaus(0.0, 4.0);
  } while (TMath::Abs(hPos[0]) > 50.0);
  hPos[1] = 0.0;
  hPos[2] = 0.0;
  RotateNpolToG4(hPos, NpolAng);
  
  if((detNums[0] == 9) || (detNums[0] == 11)) hPos[1] = -25 + 10.*float(detNums[2]) +  hPos[1];
  if((detNums[0] == 10) || (detNums[0] == 12)) hPos[1] = -35 + 10.*float(detNums[2]) +  hPos[1];
  
  if((detNums[0] == 9) && (detNums[1] == 1)) hPos[2] = hPos[2] + 700.;
  if((detNums[0] == 9) && (detNums[1] == 2)) hPos[2] = hPos[2] + 760.;
  if((detNums[0] == 10) && (detNums[1] == 1)) hPos[2] = hPos[2] + 820.;
  if((detNums[0] == 10) && (detNums[1] == 2)) hPos[2] = hPos[2] + 880.;
  if((detNums[0] == 11) && (detNums[1] == 1)) hPos[2] = hPos[2] + 693.;
  if((detNums[0] == 11) && (detNums[1] == 2)) hPos[2] = hPos[2] + 753.;
  if((detNums[0] == 12) && (detNums[1] == 1)) hPos[2] = hPos[2] + 813.;
  if((detNums[0] == 12) && (detNums[1] == 2)) hPos[2] = hPos[2] + 873.;
  RotateNpolToG4(hPos, NpolAng);
  return;
}

void DeltaEarrayHitPosition(double hPos[],double lPos[], int detNums[]){
  
  double VertOffSet = 42.0;
  
  gRandom = new TRandom();
  do {
	hPos[0] = lPos[0] + gRandom->Gaus(0.0, 4.0);
  } while (TMath::Abs(hPos[0]) > 80.0);
  hPos[1] = 0.0;
  hPos[2] = 0.0;
  RotateNpolToG4(hPos, NpolAng);
  
  if(detNums[0] == 3) hPos[1] = hPos[1] + VertOffSet;
  if(detNums[0] == 4) hPos[1] = hPos[1] + VertOffSet + 10.0;
  if(detNums[0] == 7) hPos[1] = hPos[1] + (-VertOffSet);
  if(detNums[0] == 8) hPos[1] = hPos[1] + (-(VertOffSet + 10.0));
  
  if((detNums[0] == 3) || (detNums[0] == 7)) hPos[2] = hPos[2] + 700. + (13. - (detNums[2] + 1)) * 10.;
  if((detNums[0] == 4) || (detNums[0] == 8)) hPos[2] = hPos[2] + 830. + (14. - (detNums[2] + 1)) * 10.;
  RotateNpolToG4(hPos, NpolAng);
  return;
}

void EarrayHitPosition(double hPos[],double lPos[], int detNums[]){

  double VertOffSet = 90.0;
  double HorOffSet = 60.2;
  
  gRandom = new TRandom();
  do {
	hPos[0] = lPos[0] + gRandom->Gaus(0.0, 4.0);
  } while (TMath::Abs(hPos[0]) > 80.0);
  hPos[1] = 0.0;
  hPos[2] = 0.0;
  RotateDetToNpol(hPos,detNums);
   
  if(((detNums[0] == 1) || (detNums[0] == 2)) && (detNums[1] == 1)) hPos[0] = hPos[0] + HorOffSet;
  if(((detNums[0] == 5) || (detNums[0] == 6)) && (detNums[1] == 1)) hPos[0] = hPos[0] + HorOffSet;
  if(((detNums[0] == 1) || (detNums[0] == 2)) && (detNums[1] == 2)) hPos[0] = hPos[0] - HorOffSet;
  if(((detNums[0] == 5) || (detNums[0] == 6)) && (detNums[1] == 2)) hPos[0] = hPos[0] - HorOffSet;
  // Need to position the hit in the detector then rotate to 28 degrees.
  RotateNpolToG4(hPos, NpolAng);
  
  if(detNums[0] == 1) hPos[1] = hPos[1] + VertOffSet;
  if(detNums[0] == 2) hPos[1] = hPos[1] + (VertOffSet + 10.0);
  if(detNums[0] == 5) hPos[1] = hPos[1] - VertOffSet;
  if(detNums[0] == 6) hPos[1] = hPos[1] - (VertOffSet + 10.0);
 
  if((detNums[0] == 1) || (detNums[0] == 5)) hPos[2] = hPos[2] + 700. + (13. - (detNums[2] + 1)) * 10.;
  if((detNums[0] == 2) || (detNums[0] == 6)) hPos[2] = hPos[2] + 830. + (14. - (detNums[2] + 1)) * 10.;
  RotateNpolToG4(hPos, NpolAng);
  // now we vertically and z-axis offset and rotate 28 degrees to the G4 global coordinates 
  
  return;
}

void RotateNpolToG4(double hPos[], double RotAng){

  double tempPos[3] = { hPos[0], hPos[1], hPos[2] };
  double RightAng = TMath::Pi()/2;
  
  hPos[0] = TMath::Cos(RotAng) * tempPos[0] + TMath::Cos(RightAng) * tempPos[1] + TMath::Cos(RightAng+RotAng) * tempPos[2];
  hPos[1] = TMath::Cos(RightAng) * tempPos[0] + TMath::Cos(0) * tempPos[1] + TMath::Cos(RightAng) * tempPos[2];
  hPos[2] = TMath::Cos(RightAng-RotAng) * tempPos[0] + TMath::Cos(RightAng) * tempPos[1] + TMath::Cos(RotAng) * tempPos[2];

  return;
}

void RotateDetToNpol(double hPos[], int detNums[]){

  double tempPos[3] = { hPos[0], hPos[1], hPos[2] };
  double DetAng = TMath::Pi()/4;
  double RightAng = TMath::Pi()/2;
 
  if(((detNums[0] == 1) || (detNums[0] == 2)) && (detNums[1] == 2)) DetAng = -1*DetAng;
  if(((detNums[0] == 5) || (detNums[0] == 6)) && (detNums[1] == 1)) DetAng = -1*DetAng;
   
  hPos[0] = TMath::Cos(DetAng) * tempPos[0] + TMath::Cos(RightAng - DetAng) * tempPos[1] + TMath::Cos(RightAng) * tempPos[2];
  hPos[1] = TMath::Cos(RightAng + DetAng) * tempPos[0] + TMath::Cos(DetAng) * tempPos[1] + TMath::Cos(RightAng) * tempPos[2];
  hPos[2] = TMath::Cos(RightAng) * tempPos[0] + TMath::Cos(RightAng) * tempPos[1] + TMath::Cos(0) * tempPos[2];
  
  return;
}

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


// The NPOL polarimeter is divided into 4 or 6 sections.  This function takes a volume name and returns 
// the section number that the detector belongs to, or -1 if the volume does not belong in the polarimeter.
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
	case 6: // Bottom E array 2
    case 4: // Top dE array 2
	case 8: // Bottom dE array 2
      pvNum = GetPlacementNumber(volName);
      if(pvNum <= 13 && pvNum >= 7) {return 2;}  // section 3
      else if(pvNum <= 6 && pvNum >= 0) {return 3;}  // section 4
      else {return -1;}
    case 9: // Front array 1
    case 11: // Front Veto array 1
      imprNum = GetImprNumber(volName);
      if(imprNum == 1) return 0;  // section 1
      else if(imprNum == 2) return 1;  // section 2
      else return -1;
    case 10: // Front array 2
    case 12: // Front Veto array 2
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

// This method returns the NPOL detector "type" which is defined as the following:
// analyzer = detector in 1 of the 4 vertically stacked arrays making up the analyzer sections
// tagger = detector in the arrays in front of each analyzer section
// topEArray = any detector in the top E-array detectors.  There are 2 imprints per section with 2 sections (4 total)
// botEArray = any detector in the bottom E-array detectors.  There are 2 imprints per section with 2 sections (4 total)
// topdEArray = any detector in the top dE-array detectors.  There are 2 imprints per section with 2 sections (4 total)
// botdEArray = any detector in the bottom dE-array detectors.  There are 2 imprints per section with 2 sections (4 total)
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

bool checkEarrayHits(const std::map<std::string,NpolDetectorEvent *> *detEvents){
  std::map<std::string,NpolDetectorEvent *>::const_iterator it;
  int countTop = 0;
  int countBottom = 0;
  for(it = detEvents->begin(); it != detEvents->end(); it++) {
	if((detectorType(it->first) == topEArray) && (it->second->thresholdExceeded == true)) countTop++;
	if((detectorType(it->first) == botEArray) && (it->second->thresholdExceeded == true)) countBottom++;
  }
  //std::cout << "Number Top E-Array Detectors 'Hit' =  " << countTop << std::endl;
  //std::cout << "Number Bottom E-Array Detectors 'Hit' =  " << countBottom << std::endl;
  return true;
}

bool checkdEarrayHits(const std::map<std::string,NpolDetectorEvent *> *detEvents){
  std::map<std::string,NpolDetectorEvent *>::const_iterator it;
  int countTop = 0;
  int countBottom = 0;
  for(it = detEvents->begin(); it != detEvents->end(); it++) {
	if((detectorType(it->first) == topdEArray) && (it->second->thresholdExceeded == true)) countTop++;
	if((detectorType(it->first) == botdEArray) && (it->second->thresholdExceeded == true)) countBottom++;
  }
  //std::cout << "Number Top dE-Array Detectors 'Hit' =  " << countTop << std::endl;
  //std::cout << "Number Bottom dE-Array Detectors 'Hit' =  " << countBottom << std::endl;
  return true;
}

// Return the frontmost polarimeter section that passes requirements 1 and 2.
// If -1 is returned, then no section passed requirements 1 and 2.
int getSectionOfInterest(const std::map<std::string,NpolDetectorEvent *> *detEvents) {
  int sectionOfInterest = -1; int countTdE=0; int countTE=0; 
  int countBdE=0; int countBE =0; int countA = 0; int countV = 0;
  //bool multiscatter = false;
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
		case analyzer: analyzerFlag |= it->second->thresholdExceeded == true;
		  if(it->second->totEnergyDep >= LOW_THRESHOLD) countA++;
		  break;
		case tagger: taggerFlag |= it->second->thresholdExceeded == true; 
		  if(it->second->totEnergyDep >= LOW_THRESHOLD) countV++;
		  break;
		case topEArray: topEArrayFlag |= it->second->thresholdExceeded == true; 
		  if(it->second->totEnergyDep >= LOW_THRESHOLD) countTE++; 
		  break;
		case topdEArray: topdEArrayFlag |= it->second->thresholdExceeded == true; 
		  if(it->second->totEnergyDep >= LOW_THRESHOLD) countTdE++; 
		  break;
		case botEArray: botEArrayFlag |= it->second->thresholdExceeded == true; 
		  if(it->second->totEnergyDep >= LOW_THRESHOLD) countBE++; 
		  break;
		case botdEArray: botdEArrayFlag |= it->second->thresholdExceeded == true; 
		  if(it->second->totEnergyDep >= LOW_THRESHOLD) countBdE++; 
		  break;
		default: break;
		}
	  }
	}
    
    
    // Mod: Tireman (2017-January-18) to test if multiscattering counts are playing havoc on efficiences
    // If one of this section's analyzers took a hit, then any section after this fails requirement 2.
    // Note: Here we check if SOI has been set and then set Multiscatter = true; remove this check to allow multiscatter
    if(analyzerFlag) {
      //if(sectionOfInterest != -1) multiscatter = true;  // Turn this off by commenting out this line
      sectionOfInterest = -1;
	  //}
	  //if((analyzerFlag && topEArrayFlag && topdEArrayFlag && !taggerFlag) || (analyzerFlag && botEArrayFlag && botdEArrayFlag && !taggerFlag)) {  
	  //if(!taggerFlag) sectionOfInterest = section; // If this section passes requirement 1, then it may be the section of interest
	  sectionOfInterest = section;
    }
    
    //  Code inserted for future "checks" of the E-array and dE-array if necessary
    //if(checkEarrayHits(detEvents) && checkdEarrayHits(detEvents)){ } 	
  }
 
  // Reject events with hits in multiple sections of interest
  // Reject events with more than 40 detectors with hits above 1 MeV
  // Otherwise return the ID'd section of interest
  int totalDetHit = countA + countTE + countBE + countV + countTdE + countBdE;
  //if(multiscatter) {  
  //	sectionOfInterest = -1;
  //	std::cout << "Multi-hit: Event rejected!" << std::endl;
  //	return sectionOfInterest;
  //} 
  if(totalDetHit >= 40) { 
	std::cout << "Event Rejected! Total number of detectors with 40 keV or greater: " << totalDetHit << std::endl;
	sectionOfInterest = -1;
	return sectionOfInterest;
  } else {
	return sectionOfInterest;
  }
}


// If requirement 5 is passed, return the E array of interest (top or bottom).  If requirement 5 does not pass, unknown is returned.
PolarimeterDetector getEArrayOfInterest(std::map<PolarimeterDetector, double> *eDepArrayTotal, int sectionOfInterest) {
  double topETotal = (*eDepArrayTotal)[topEArray];
  double botETotal = (*eDepArrayTotal)[botEArray];
  double topdETotal = (*eDepArrayTotal)[topdEArray];
  double botdETotal = (*eDepArrayTotal)[botdEArray];

  if((topETotal > 20*botETotal) && (topdETotal > 20*botdETotal))
    return topEArray;
  else if((botETotal > 20*topETotal) && (botdETotal > 20*topdETotal))
    return botEArray;
  else
    return unknown;
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


// Return the azimuthal part of the scattering angle (in degrees) in the polarimeter given three points in global coordinates:
// (p1x,p1y,p1z) - a point along the line of the incoming neutron's trajectory (the vertex in the target will do)
// (p2x,p2y,p2z) - the point where the n,p elastic collision occurs in the analyzer detector
// (p3x,p3y,p3z) - a point along the trajectory of the scattered proton (in the E array)
// The calculation is done by forming a triangle with these points and using the law of cosines
double getAzimuthAngle(const double p1x, const double p1y, const double p1z,
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
		if((it->second->totEnergyDep) > totEdepSoFar){
		  totEdepSoFar = it->second->totEnergyDep;
		  ret[0] = (it->second->hPosX);
		  ret[1] = (it->second->hPosY);
		  ret[2] = (it->second->hPosZ);
		  *time  = (it->second->time);
		}
      }
    }
  }
}

void GetPoI2(double *ret, double *time, const int section, const PolarimeterDetector type, const std::map<std::string,NpolDetectorEvent *> *detEvents) {
  std::map<std::string,NpolDetectorEvent *>::const_iterator it;

  ret[0] = 0.0;
  ret[1] = 0.0;
  ret[2] = 0.0;
  double totEdepSoFar = 0.0;
  for(it = detEvents->begin(); it != detEvents->end(); it++) {
    if((sectionNumber(it->first) == section || sectionNumber(it->first) == (section + 1) || sectionNumber(it->first) == (section + 2)) && detectorType(it->first) == type) {
      if(it->second->thresholdExceeded) {
		ret[0] += (it->second->totEnergyDep)*(it->second->hPosX); //gPosX
		ret[1] += (it->second->totEnergyDep)*(it->second->hPosY);
		ret[2] += (it->second->totEnergyDep)*(it->second->hPosZ);
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

// Return the Azimuth angle only.  Check is in main code.
// Optionally return delta time-of-flight between the analyzer and E-array
double ReturnAngle(NpolVertex *incNeutronVert, std::map<std::string,NpolDetectorEvent *> *detEvents, const int section, const PolarimeterDetector EArrayOfInterest, double *dTOF = NULL) {
  double targetPt[3];
  double analyzerPt[3];
  double EarrayPt[3];
  double analyzerTime;
  double EarrayTime;

  targetPt[0] = incNeutronVert->posX;
  targetPt[1] = incNeutronVert->posY;
  targetPt[2] = incNeutronVert->posZ;

  GetPoI(analyzerPt,&analyzerTime,section,analyzer,detEvents);
  GetPoI2(EarrayPt,&EarrayTime,section,EArrayOfInterest,detEvents);

  double azAngle = getAzimuthAngle(targetPt[0],targetPt[1], targetPt[2],
								   analyzerPt[0],analyzerPt[1],analyzerPt[2],
								   EarrayPt[0],EarrayPt[1],EarrayPt[2]);

  //txtOut << azAngle << " deg" << std::endl;
  //	std::cout << "(" << targetPt[0] << "," << targetPt[1] << "," << targetPt[2] << ")   "
  //		<< "(" << analyzerPt[0] << "," << analyzerPt[1] << "," << analyzerPt[2] << ")   "
  //		<< "(" << EarrayPt[0] << "," << EarrayPt[1] << "," << EarrayPt[2] << ")   "
  //		<< azAngle << " " << std::endl;
  //	if(azAngle > 90.0)
  //		std::cout << azAngle << " " << std::endl;

  if(dTOF != NULL)
    *dTOF = EarrayTime - analyzerTime;
  return azAngle;
}

TString FormInputFile(TString InputDir){
  
  TString fileName = InputDir + "/" + BaseName + "_" + JobNum + ".root";
  
  return fileName;
}

TString FormOutputFile(TString OutputDir){
  
  TString fileName =  OutputDir + "/" + BaseName + "_NpolEff" + "_" + JobNum + ".root";
  
  return fileName;
}

void RetrieveENVvariables() {

  if(getenv("CHARGE_TYPE")){
	CType = getenv("CHARGE_TYPE");
  }else{
	CType = "All";
  }

  if(getenv("JOBNUMBER")){
	JobNum = getenv("JOBNUMBER");
  }else{
    JobNum = "99999"; // default job number is 99999
  }

  std::cout << "Processing job number: " << JobNum << std::endl;

  if(getenv("NPOLBASENAME")){
	BaseName = getenv("NPOLBASENAME");
  }else{
	std::cout << "Npol Base Name environmental variable not set" << std::endl; 
	return; // Return error if not found
  }

  if(getenv("Lead")){
    Lead = getenv("Lead");
  }else{
     std::cout << "Lead environmental variable not set" << std::endl;
     return; // Return error if not found
  }

  if(getenv("Energy")){
    Energy = getenv("Energy");
  }else{
    std::cout << "Energy environmental variable not set" << std::endl;
     return; // Return error if not found
  }
  
  if(getenv("Bfield")){
    Bfield = getenv("Bfield");
  }else{
    std::cout << "Bfield environmental variable not set" << std::endl;
     return; // Return error if not found
  }
  
  if(getenv("HistoOutputDir")){
	OutputDir = getenv("HistoOutputDir");
  }else{
	std::cout << "Output Directory environmental varilable not set" << std::endl;
	return;
  }

  if(getenv("RawDataDir")){
	InputDir = getenv("RawDataDir");
  }else{
	std::cout << "Input Directory environmental varilable not set" << std::endl;
	return;
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
