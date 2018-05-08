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

#include "NpolVertex.hh"
#include "NpolTagger.hh"
#include "NpolStep.hh"
#include "NpolStatistics.hh"
#include "NpolDetectorEvent.hh"

#include "NpolFileEnvManager.hh"
#include "NpolEventPreProcessing.hh"
#include "NpolEventProcessing.hh"
#include "NpolPhysicsVariables.hh"
#include "NpolHistoManager.hh"

using namespace std;

//********************** Definition of Variables and Classes *********************//

#define EDEP_THRESHOLD 1.0  /*MeV*/
#define angleLow 45.3       /*degrees: low angle recoil proton cut*/
#define angleHigh 81.6      /*degrees; high angle recoil proton cut*/
;
NpolEventProcessing *Process = NpolEventProcessing::GetInstance();
NpolEventPreProcessing *PProcess = NpolEventPreProcessing::GetInstance();
NpolPhysicsVariables *PhysicsVar = NpolPhysicsVariables::GetInstance();
NpolFileEnvManager *FEman = NpolFileEnvManager::GetInstance();
//NpolHistoManager *HistMan = NpolHistoManager::GetInstance();

//***************** End Definitions of Variables and Classes ***********************//

//************************ Main Program Begins Here ********************************//

int main(int argc, char *argv[]) {
  
  FEman->RetrieveENVvariables();

  TString InputFile = FEman->FormInputFile(FEman->InputDir);
  TString OutputFile = FEman->FormOutputFile(FEman->OutputDir);

  TFile *outFile = new TFile(OutputFile,"RECREATE"); 
    
  TChain *npolTree = new TChain("T");
  TChain *statsTree = new TChain("T2");

  npolTree->Add(InputFile);
  statsTree->Add(InputFile);

  //npolTree->SetCacheSize(50000000);  // This increases the amount of data loaded 
  //statsTree->SetCacheSize(50000000); // per call for more data to chew on. 
  
  std::vector<NpolStep *> *steps = NULL;
  std::vector<NpolVertex *> *verts = NULL;
  std::vector<NpolTagger *> *tagEvent = NULL;
  std::vector<NpolStatistics *> *stats = NULL;
  npolTree->SetBranchAddress("steps",&steps);
  npolTree->SetBranchAddress("tracks",&verts);
  npolTree->SetBranchAddress("NPOLTagger",&tagEvent);
  statsTree->SetBranchAddress("stats",&stats);
  
  //********************************* Define your Histograms Here *******************************
  TH1F *h_recoilAngle_Real = new TH1F("recoilAngle_Real","Real Proton Recoil Angle",200, 0.0, 180.0);
  TH1F *h_recoilEnergy_Real = new TH1F("recoilEnergy_Real","Real Proton Recoil Energy",200, 000.0, 2400.);
  TH1F *h_asymmetry_Real = new TH1F("asymmetry_Real","Real Asymmetry from Recoil Proton",5, -2,+2);
  
  TH1F *h_recoilAngle = new TH1F("recoil_angle","Proton Recoil Angle", 200, 0.0, 180.0); 
  TH1F *h_recoilAngle_Raw = new TH1F("recoil_angle_raw","Proton Recoil Angle Before Angle Cut", 200, 0.0, 180.0);
  TH1F *h_Neutron_Theta_Angle = new TH1F("Neutron_Theta_Angle","Neutron Angle at first tagger", 100, 15.0, 40.0);
  TH1F *h_Neutron_Momentum = new TH1F("Neutron_Momentum","Neutron Momentum at the first tagger",100, 1600.0, 3200.0);
  TH1F *h_Neutron_Momentum_Initial = new TH1F("Neutron_Momentum_Initial","Initial Neutron Momentum when Generated",100, 1600.0, 3200.0);
  TH1F *h_Neutron_Energy_Initial = new TH1F("Neutron_Energy_Initial","Initial Neutron Energy when Generated",100, 1000.0, 2400.0);
  TH1F *h_Neutron_Energy = new TH1F("Neutron_Energy","Neutron Energy at NPOL Tagger",100, 1000.0, 2400.0);
  TH1F *h_totEnergy = new TH1F("totEnergy","Total Energy Deposited", 100, 0.0, 350.0);
  TH2F *h_dEoverEtop = new TH2F("dEoverEtop", "dE over E for top array", 400,0,120,400,0,20);
  TH2F *h_dEoverEbot = new TH2F("dEoverEbot", "dE over E for bottom array", 400,0,120,400,0,20);
  TH1F *h_sectionEfficiency1 = new TH1F("sectionEfficiency1","NPOL Efficiency after SOI Selection",13,0.25,6.75);
  TH1F *h_sectionEfficiency2 = new TH1F("sectionEfficiency2","#splitline{NPOL Efficiency after EOI Selection}{and Asymmetry Cut}",13,0.25,6.75);
  TH1F *h_sectionEfficiency3 = new TH1F("sectionEfficiency3","#splitline{NPOL Efficiency after Array}{Total Energy Cuts}",13,0.25,6.75);
  TH1F *h_sectionEfficiency4 = new TH1F("sectionEfficiency4","NPOL Efficiency after Angle Cut",13,0.25,6.75);
  TH1F *h_dTOF = new TH1F("dTOF","Delta time-of-flight",600,-30,120);
  //********************************* End Histogram Definitions ********************************

  // BEGIN STATS LOOP
  int totalEvents = 0;
  int taggedEvents = 0;
  int eventsPassed = 0;
  int eventsFailed = 0;
  
  for(int i = 0; i < statsTree->GetEntries(); i++) {
    statsTree->GetEntry(i);
    totalEvents += ((*stats)[0])->totalEvents;
  }// END STATS LOOP
  std::cout << totalEvents << " neutrons thrown at setup." << std::endl;
	
  // BEGIN EVENT LOOP
  int asym = 1;
  int nentries = npolTree->GetEntries();
  for(int i = 0; i < nentries; i++) {
	//for(int i = 0; i < 1; i++) {
   	if(i % 100 == 0)
      std::cout << "Processing event #" << i << std::endl;
    npolTree->GetEntry(i);
    std::map<std::string,NpolDetectorEvent *> detEvents;   // Event map (NPOL Detector Class)
	std::map<PolarimeterDetector, double> eDepArrayTotal;  // Total energy map for each array
    eDepArrayTotal[analyzer] = 0.0;
    eDepArrayTotal[tagger] = 0.0;
    eDepArrayTotal[topEArray] = 0.0;
    eDepArrayTotal[topdEArray] = 0.0;
    eDepArrayTotal[botEArray] = 0.0;
    eDepArrayTotal[botdEArray] = 0.0;

	// BEGIN TRACK LOOP: Scans Tracks vector (NpolVertex) and fills histograms
	// This is to analyze sim output for "real" (n,p) scattering events
	std::vector<NpolVertex *>::iterator v_it;
	int selectSOI = -1;

	// This loop scans through the tracks vector and looks for an (n,p) scattering
	// event and which assembly volume it occurred in and then saves that AVNum to
	// 'selectSOI'.  We have to do this first to account for muiltple scattering
	// and due to the fact that the vertices are not stored in order.
	for(v_it = verts->begin(); v_it != verts->end(); v_it++){
	  NpolVertex *aVertex = *v_it;
	  if(aVertex == NULL) continue;
	  
	  int PID = aVertex->parentId;
	  int TID = aVertex->trackId;
	  int pType = aVertex->particleId;
	  std::string process = aVertex->process;
	  std::string volName = aVertex->volume;
	  int AVNum = PProcess->GetAVNumber(volName);

	  // We cut on parent ID = 1 (original neutron), track ID > 2 (could be changed later,
	  // particle type = proton (2212), physics process = hadron elastic, and AV number
	  // between 9 and 10 (analyzer volume)
	  if((PID == 1 && TID >= 2 && pType == 2212 && process == "hadElastic") && (AVNum == 9 || AVNum == 10)){
		if(selectSOI == -1 || AVNum < selectSOI) selectSOI = AVNum;
	  }
	}

	// After determining the 'selectSOI' which has the (n,p) scattering, we scan through
	// the tracks vector again to fill the histograms and use 'selectSOI' as a cut as
	// necessary
	for(v_it = verts->begin(); v_it != verts->end(); v_it++){
	  NpolVertex *aVertex = *v_it;
	  if(aVertex == NULL) continue;
	  
	  int PID = aVertex->parentId;
	  int TID = aVertex->trackId;
	  int pType = aVertex->particleId;
	  std::string process = aVertex->process;
	  std::string volName = aVertex->volume;
	  int AVNum = PProcess->GetAVNumber(volName);

	  // Extract out Initial Neutron Information from Tracks vector
	  if(PID == 0 && TID == 1){
		double xMomI = aVertex->momX; double yMomI = aVertex->momY; double zMomI = aVertex->momZ;
		double totMomI = TMath::Sqrt(TMath::Power(xMomI,2)+TMath::Power(yMomI,2)+TMath::Power(zMomI,2));
		h_Neutron_Energy_Initial->Fill(aVertex->energy);
		h_Neutron_Momentum_Initial->Fill(totMomI);
	  }

	  if((PID == 1 && TID >= 2 && pType == 2212 && process == "hadElastic") && AVNum == selectSOI){
		
		NpolVertex *intNeutron = verts->at(1);
		double momX = aVertex->momX; double momY = aVertex->momY; double momZ = aVertex->momZ;
		double momTot = TMath::Sqrt(momX*momX + momY*momY + momZ*momZ);
		double P1x = intNeutron->posX; double P1y = intNeutron->posY; double P1z = intNeutron->posZ;
		double P2x = aVertex->posX; double P2y = aVertex->posY; double P2z = aVertex->posZ;

		double P2Theta = TMath::ATan(momY/momX);
		double P2Phi = TMath::ACos(momZ/momTot);
				
		double P3x = P2x + 2*TMath::Sin(P2Phi)*TMath::Cos(P2Theta);
		double P3y = P2y + 2*TMath::Sin(P2Phi)*TMath::Sin(P2Theta);
		double P3z = P2z + 2*TMath::Cos(P2Phi);
		double computedAngle = PhysicsVar->getAzimuthAngle(P1x,P1y,P1z,P2x,P2y,P2z,P3x,P3y,P3z);
		if (computedAngle >= angleLow && computedAngle <= angleHigh) {
		  h_recoilAngle_Real->Fill(computedAngle);
		  if(P2y > 0) asym = 1;
		  if(P2y < 0) asym =-1;
		  if(P2y == 0) asym = 0;
		  h_asymmetry_Real->Fill(asym);
		  h_recoilEnergy_Real->Fill(aVertex->energy);
		}
	  }
	}
	// END TRACK LOOP

	
    // BEGIN STEPS LOOP: Fills the detEvent map with volumes and total energy, etc.
	std::vector<NpolStep *>::iterator s_it;
	std::vector<NpolTagger *>::iterator t_it;
	bool eventFlag = false;
    for(s_it = steps->begin(); s_it != steps->end(); s_it++) {
      NpolStep *aStep = *s_it;
	  if(aStep == NULL) continue;
	  
	// These lines of code count up the number of particles entering into the 
	// first analyzer array that are a neutron (2112), parent ID = 0, track ID = 1.
	  int AVNum = PProcess->GetAVNumber(aStep->volume);
	  if(AVNum == -1) continue;
	  int imprintNum = PProcess->GetImprNumber(aStep->volume);
	  int PVNum = PProcess->GetPlacementNumber(aStep->volume);

	  if((!eventFlag) && (aStep->parentId == 0) && (aStep->trackId == 1) && (aStep->particleId == 2112) 
		&& (AVNum == 11) && (imprintNum == 1)) {
		taggedEvents++;
		eventFlag = true;
		
		// Neutron Diagnostics!	
		
		double xMom = aStep->momX; double yMom = aStep->momY; double zMom = aStep->momZ;
		double totMom = TMath::Sqrt(TMath::Power(xMom,2)+TMath::Power(yMom,2)+TMath::Power(zMom,2));
		
		double Rxy = TMath::Sqrt(TMath::Power(xMom,2)+TMath::Power(yMom,2));
		double neutronAngle = TMath::ATan(Rxy/zMom)*TMath::RadToDeg();

		t_it = tagEvent->begin();
		NpolTagger *tTemp = *t_it;
		h_Neutron_Energy->Fill(tTemp->energy);

		h_Neutron_Momentum->Fill(totMom);
		h_Neutron_Theta_Angle->Fill(neutronAngle);
		
	  }
		
      if(detEvents.find(aStep->volume) == detEvents.end())
		detEvents[aStep->volume] = new NpolDetectorEvent();
	  
      (detEvents[aStep->volume])->totEnergyDep += aStep->eDep;
	  
      if(!(detEvents[aStep->volume])->thresholdExceeded &&
		 (detEvents[aStep->volume])->totEnergyDep >= EDEP_THRESHOLD) {
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
		  PProcess->AnalyzerTaggerHitPosition(hitPos, lPos, detNums);
		} else if((AVNum == 3) || (AVNum == 4) || (AVNum == 7) || (AVNum == 8)){
		  PProcess->DeltaEarrayHitPosition(hitPos, lPos, detNums);
		} else if((AVNum == 1) || (AVNum == 2) || (AVNum == 5) || (AVNum == 6)){
		  PProcess->EarrayHitPosition(hitPos, lPos, detNums);
		}
		
		(detEvents[aStep->volume])->hPosX = hitPos[0]; 
		(detEvents[aStep->volume])->hPosY = hitPos[1]; 
		(detEvents[aStep->volume])->hPosZ = hitPos[2];
		

		// ****** End of the hit position computations section ******* //
	  }
    } // END STEPS LOOP
	

	// ***** This Section will retrieve SOI and EOI and then make event selections based on cuts ******* //
    // First, we determine the SOI; then we check the other requirements and fill the histograms
	int sectionOfInterest = Process->getSectionOfInterest(&detEvents); // call method to determine SOI
	
	if(sectionOfInterest != -1) {
	  h_sectionEfficiency1->Fill(sectionOfInterest+1); // Fill first SOI histogram
	  // Fill the Energy Deposited per array map for the SOI
	  Process->getEDepArrayTotal(&detEvents, &eDepArrayTotal, sectionOfInterest); 
	  
	  // E-array of Interest (EOI) needs some serious work.  Andrei suggestion receieved 3/27/2018. 
      PolarimeterDetector EArrayOfInterest = Process->getEArrayOfInterest(&detEvents,&eDepArrayTotal,sectionOfInterest);
      PolarimeterDetector dEArrayOfInterest = unknown;	  
	  
      if(EArrayOfInterest == topEArray){ dEArrayOfInterest = topdEArray; }
	  else if(EArrayOfInterest == botEArray){ dEArrayOfInterest = botdEArray; }

      if((EArrayOfInterest != unknown) && (dEArrayOfInterest != unknown)) {
		h_sectionEfficiency2->Fill(sectionOfInterest+1); //Fill
		double eDepAnalyzer = eDepArrayTotal[analyzer]; 
		double eDepE = eDepArrayTotal[EArrayOfInterest]; 
		double eDepdE = eDepArrayTotal[dEArrayOfInterest];
		double eDepTotal = eDepAnalyzer + eDepE + eDepdE;
		double dTOF = -100.0;
		double azAngle = PhysicsVar->ReturnAngle(verts->at(1),&detEvents,sectionOfInterest,EArrayOfInterest,&dTOF);

		if(eDepAnalyzer >= 4.0 /*MeV*/ && eDepE >= 5.0 /*MeV */ && eDepTotal >= 50.0 /*MeV*/) { // Requirements 3 and 4
		  h_sectionEfficiency3->Fill(sectionOfInterest+1); //FILL
		  h_recoilAngle_Raw->Fill(azAngle);
		  if (azAngle >= angleLow && azAngle <= angleHigh){  // checks Requirement 6 
			h_sectionEfficiency4->Fill(sectionOfInterest+1); //FILL
			h_totEnergy->Fill(eDepTotal);
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
	
    // Clear out the maps for the next event
    eDepArrayTotal.clear();
	std::map<std::string,NpolDetectorEvent *>::iterator e_it;
    for(e_it = detEvents.begin(); e_it != detEvents.end(); e_it++) delete e_it->second;
    detEvents.clear();
  } // END EVENT LOOP
  
  std::cout << taggedEvents << " of the initial " << totalEvents << " neutrons have crossed the Npol Tagger." << std::endl;
  std::cout << eventsPassed << " events passed requirements.  "
			<< (taggedEvents - eventsPassed) << " failed." << std::endl;
  std::cout << (((double)eventsPassed)*100)/(double)taggedEvents << " % of the " << taggedEvents
			<< " neutrons passed cuts." << std::endl;
  

  // **** Fill the statistics vector ****** //
  TVectorD runStatistics(6);
  runStatistics[0] = static_cast<double>(totalEvents);
  runStatistics[1] = static_cast<double>(taggedEvents);
  runStatistics[2] = static_cast<double>(eventsPassed);
  runStatistics[3] = static_cast<double>(eventsFailed);
  runStatistics[4] = static_cast<double>(-1.0);         
  runStatistics[5] = static_cast<double>(-1.0);

  // ****** Write out all the results to the root file and close the file(s) ******** //
  runStatistics.Write();
  h_dEoverEtop->Write();
  h_dEoverEbot->Write();
  h_totEnergy->Write();
  h_sectionEfficiency1->Write();
  h_sectionEfficiency2->Write();
  h_sectionEfficiency3->Write();
  h_sectionEfficiency4->Write();
  h_dTOF->Write();
  h_recoilAngle->Write();
  h_recoilAngle_Raw->Write();
  h_Neutron_Theta_Angle->Write();
  h_Neutron_Momentum->Write();
  h_Neutron_Momentum_Initial->Write();
  h_Neutron_Energy_Initial->Write();
  h_Neutron_Energy->Write();
  h_recoilAngle_Real->Write();
  h_recoilEnergy_Real->Write();
  h_asymmetry_Real->Write();
  outFile->Close();
  return 0;
}

// ************** End Main Program here *************** //

// ************* Methods for Main Program are below  this line ************** //

// No methods here ... they have all been moved to separate classes!

// ************* Requirements as specified in the PAC 37 proposal *********** //

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

/*if((PID == 1 && TID >= 2 && pType == 2212 && process == "hadElastic") && (AVNum >= 9 && AVNum <= 12)){
		std::cout << "Event Number= : " << i << "   PID= " << aVertex->parentId << "   TID= " << aVertex->trackId << "   pType= " << aVertex->particleId << "   Process= " << aVertex->process << std::endl;
		}*/
