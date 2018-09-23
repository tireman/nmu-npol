/* Npol Analysis Script is designed to analyze the neutron flux on 
   the NPOL polarimeter being designed by the CGEN collaboration at 
   Jefferson National Laboratory. (2016) Revisions: Spring 2017 by 
   Will Tireman and Ashley Adzima (added some histograms) Revisions: 
   January-March 2018 by Will Tireman (Fixed eff. calculation, cleaned up code)
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
#include "TRandom3.h"

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
NpolPhysicsVariables *PhysVars = NpolPhysicsVariables::GetInstance();
NpolFileEnvManager *FEman = NpolFileEnvManager::GetInstance();
NpolHistoManager *HistoMan = NpolHistoManager::GetInstance();

//***************** End Definitions of Variables and Classes ***********************//

//************************ Main Program Begins Here ********************************//

int main(int argc, char *argv[]) {
  
  FEman->RetrieveENVvariables();

  TString InputFile = FEman->FormInputFile(FEman->InputDir);
  TString OutputFile = FEman->FormOutputFile(FEman->OutputDir);

  HistoMan->OpenFile(OutputFile);

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
  // 1D-Histograms
  HistoMan->CreateHistograms("recoilAngleReal","Real Proton Recoil Angle",200, 0.0, 180.0);
  HistoMan->CreateHistograms("recoilEnergyReal","Real Proton Recoil Energy",200, 000.0, 2400.);
  HistoMan->CreateHistograms("asymmetryReal","Real Asymmetry from Recoil Proton",5, -2,+2); 
  HistoMan->CreateHistograms("recoilAngle","Proton Recoil Angle", 200, 0.0, 180.0);
  HistoMan->CreateHistograms("recoilAngleRaw","Proton Recoil Angle Before Angle Cut", 200, 0.0, 180.0);
  HistoMan->CreateHistograms("selectedRecoilMomentum","Highest Energy Recoil Proton Momentum",200, -3000.0, 1000.);
  HistoMan->CreateHistograms("Qsquared","Q-Squared Value for recoil Proton", 100, 0.0, 2e6);					   
  HistoMan->CreateHistograms("NeutronThetaAngle","Neutron Angle at first tagger", 100, 15.0, 40.0);
  HistoMan->CreateHistograms("NeutronMomentum","Neutron Momentum at the first tagger",100, 1600.0, 3200.0);
  HistoMan->CreateHistograms("NeutronEnergy","Neutron Energy at NPOL Tagger",100, 1000.0, 2400.0);
  HistoMan->CreateHistograms("sectionEfficiency1","NPOL Efficiency after SOI Selection",13,0.25,6.75);
  HistoMan->CreateHistograms("sectionEfficiency2","#splitline{NPOL Efficiency after EOI Selection}{and Asymmetry Cut}",13,0.25,6.75);
  HistoMan->CreateHistograms("sectionEfficiency3","#splitline{NPOL Efficiency after Array}{Total Energy Cuts}",13,0.25,6.75);
  HistoMan->CreateHistograms("sectionEfficiency4","NPOL Efficiency after Angle Cut",13,0.25,6.75);							 
  HistoMan->CreateHistograms("NeutronMomentumInitial","Initial Neutron Momentum when Generated",100, 1600.0, 3200.0);
  HistoMan->CreateHistograms("NeutronEnergyInitial","Initial Neutron Energy when Generated",100, 1000.0, 2400.0);							 
  HistoMan->CreateHistograms("totalEnergy","Total Energy Deposited", 100, 0.0, 350.0);
  HistoMan->CreateHistograms("dTOF","Delta time-of-flight",600,-30,120);

  // 2D-Histograms
  HistoMan->CreateHistograms("dEoverEtop", "dE over E for top array", 400,0,120,400,0,20);
  HistoMan->CreateHistograms("dEoverEbot", "dE over E for bottom array", 400,0,120,400,0,20);
  HistoMan->CreateHistograms("dEvsEReal", "dE over E for Real Events", 400,0,120,400,0,20);
  HistoMan->CreateHistograms("dEvsEReal2", "dE over E for Real Events with Energy Resolution", 400,0,120,400,0,20);						
  
  //********************************* End Histogram Definitions ********************************

  // ****** BEGIN STATS LOOP ******
  int totalEvents = 0;   // Total number of neutrons (events) generated and recorded in the data file
  int taggedEvents = 0;  // Total number of neutrons (events) which make it through magnets and lead curtain
  int eventsPassed = 0;  // Total number of neutron scattering events which pass all Proposal 37 cuts
  int eventsFailed = 0;  // Total number of neutron scattering events which do not pass all Proposal 37 cuts
    for(int i = 0; i < statsTree->GetEntries(); i++) {
    statsTree->GetEntry(i);
    totalEvents += ((*stats)[0])->totalEvents;
  }
  std::cout << totalEvents << " neutrons thrown at setup." << std::endl;
  // ****** END STATS LOOP ****** 

  
  // ****** BEGIN EVENT LOOP ****** 
  // int asym = 1;
  int nentries = npolTree->GetEntries();
  TRandom *rand = new TRandom3();
  for(int i = 0; i < nentries; i++) {
	//for(int i = 0; i < 500; i++) {
   	if(i % 1000 == 0) std::cout << "Processing event #" << i << std::endl;

    npolTree->GetEntry(i);
	
	std::map<int,NpolVertex *> vertexMap;  // Vertex Map of tracks that originate from event of interest
    std::map<std::string,NpolDetectorEvent *> detEvents;   // Event map (NPOL Detector Class)
	std::map<PolarimeterDetector, double> eDepArrayTotal;  // Total energy map for each array
    eDepArrayTotal[analyzer] = 0.0;
    eDepArrayTotal[tagger] = 0.0;
    eDepArrayTotal[topEArray] = 0.0;
    eDepArrayTotal[topdEArray] = 0.0;
    eDepArrayTotal[botEArray] = 0.0;
    eDepArrayTotal[botdEArray] = 0.0;

	// ****** BEGIN STEP LOOP: Scans Steps vector (NpolStep) and IDs the event type (elastic, inelastic) ****** 
	// This loop scans through the Steps Vector and checks PID = 0, TID = 1 (original neutron) and checks
	// the physics process to identify elastic or inelastic event and which AV (SOI) it occurs within.  Since
	// quasi-elastic is wrapped up with inelastic in G4, a seperate method will be needed to separate out those
	// two types of events.  For now (28-Aug-2018), this will only separate elastic and inelastic events.
	// This is to analyze simulation output for "real" (n,p) scattering events
	
	int npSOI = -2; int npPID = -1;
	bool inelasticFlag = false; bool elasticFlag = false; //bool pionFlag = false;
	bool quasielasticFlag = false; 

	std::vector<NpolStep *>::iterator ps_it;
	for(ps_it = steps->begin(); ps_it != steps->end(); ps_it++) {
	  NpolStep *aStep = *ps_it;
	  if(aStep == NULL) continue;

	  std::string physProcess = aStep->process;
	  std::string volName = aStep->volume;
	  int AVNum = PProcess->GetAVNumber(volName);
	  int section = Process->sectionNumber(volName);
	  int PID = aStep->parentId;
	  int TID = aStep->trackId;

	  if((PID == 0 && TID == 1) && (AVNum == 9 || AVNum == 10)){
		if(physProcess == "hadElastic"){
		  elasticFlag = true;
		  PhysVars->fillVertexMap(vertexMap,verts,1,volName);
		  
		} else if(physProcess == "neutronInelastic"){
		  int neutronCount = 0; int protonCount = 0; int gammaCount = 0;
		  int othersCount = 0; int pionCount = 0;
		  
		  PhysVars->fillVertexMap(vertexMap,verts,1,volName);
		  std::map<int,NpolVertex *>::iterator mapIt;
		  for(mapIt = vertexMap.begin(); mapIt != vertexMap.end(); mapIt++){
			int pType = mapIt->second->particleId;
			if(mapIt->second->parentId == 0) continue;
			if(pType == 2112) neutronCount++;
			else if(pType == 2212) protonCount++;
			else if(pType == 22) gammaCount++;
			else if(pType == 111 || pType == 211 || pType == -211) pionCount++;
			else othersCount++;
		  }

		  if(pionCount != 0) {
			inelasticFlag = true;
		  } else if(neutronCount >= 0 && protonCount >= 0 && gammaCount >= 0){
			//Quasi-elastic check call
			quasielasticFlag = PhysVars->checkQuasiElasticScattering(vertexMap);  
		  } else {
			continue;
		  }
		} else {
		  continue;
		}
		
		if(elasticFlag || quasielasticFlag || inelasticFlag){
		  npSOI = section;
		  npPID = TID;   // Track ID of neutron is Parent ID of the generated secondary(ies)
		  break;         // Break out of the loop once the original neutron interacts!
		} else {
		  continue;
		}
	  }
	}
	

  
	// ***** This statement checks to see if the flags of events NOT of interest  ***** //
	// are set. If so then the event processing is canceled by the continue statment
	// which cycles the loop. 
	// This kills the EVENT as a WHOLE; Beware!! //
	
	// Check if NO flags set; kill event processing
	if(!(elasticFlag) && !(inelasticFlag) && !(quasielasticFlag)) continue;  

	// Comment out the corresponding statement below in order to select
	// the events you want to analyze.  Comment out all to keep all events

	//if((quasielasticFlag || inelasticFlag) && !(elasticFlag)) continue;
	// Elastic events only
	//if((elasticFlag || inelasticFlag) && !(quasielasticFlag)) continue;
	// Quasielastic events only
	//if((quasielasticFlag || elasticFlag) && !(inelasticFlag)) continue;
	// Inelastic events only
	
  
	// ****** This section computes the (P_leading - P_elastic) ******
	// ****** value and saves into histogram                    ******
	double neutronMomentum = PhysVars->computeInitialNeutronMomentum(vertexMap);
	int leadingTID = PhysVars->findLeadingParticle(vertexMap);
	double computedRecoilAngle = PhysVars->
	  computeRecoilParticleAngle(vertexMap,leadingTID);
	double leadingParticleMomentum =
	  PhysVars->computeLeadingParticleMomentum(vertexMap,leadingTID);
	double elasticMomentum = PhysVars->
	  computeElasticMomentum(neutronMomentum, computedRecoilAngle*TMath::DegToRad());
  	
	HistoMan->FillHistograms("selectedRecoilMomentum",
							 (leadingParticleMomentum - elasticMomentum));  
	// ****** End (P_leading - P_elastic) code ******


	// After the proton track has been found, this code will compute values,
	// fill histograms
	int bestProtonTID = PhysVars->findBestProtonTrackID(vertexMap,steps,npSOI);
	if(bestProtonTID == 0) continue;
	
	// Extract out Initial Neutron Information from the vertexMap
	// Initial Neutron histograms for diagnostics
	std::map<int,NpolVertex *>::iterator mapIt2;
	int asym = 0;
	double initialNeutronMomentum = 0.;
	
	// fill initial neutron histograms
	mapIt2 = vertexMap.find(1);
	initialNeutronMomentum = PhysVars->computeInitialNeutronMomentum(vertexMap);
	HistoMan->FillHistograms("NeutronMomentumInitial",initialNeutronMomentum);
	HistoMan->FillHistograms("NeutronEnergyInitial",(mapIt2->second->energy));

	// fill selected proton track histograms
	mapIt2 = vertexMap.find(bestProtonTID);
	double computedProtonAngle = PhysVars->
	  computeRecoilParticleAngle(vertexMap,bestProtonTID);
	if(computedProtonAngle >= angleLow && computedProtonAngle <= angleHigh) {
	  HistoMan->FillHistograms("recoilAngleReal",computedProtonAngle);
	  double protonVertexEnergy = mapIt2->second->energy;
	  HistoMan->FillHistograms("recoilEnergyReal",protonVertexEnergy);
	  double sPower = PhysVars->
		computeBetheBloch(protonVertexEnergy,938.27205,1,1.032,12.929,7,64.7e-6);  
	  double dEenergyLost = PhysVars->
		computeEnergyLoss(protonVertexEnergy,TMath::DegToRad()*computedProtonAngle,1./*cm*/);
	  Double_t dEenergyLost2 = rand->Gaus(dEenergyLost, 0.10*dEenergyLost);
	  double EenergyLost = PhysVars->
		computeEnergyLoss(protonVertexEnergy,TMath::DegToRad()*computedProtonAngle,10. /*cm*/);
	  Double_t EenergyLost2 = rand->Gaus(EenergyLost, 0.10*EenergyLost);
	  HistoMan->FillHistograms("dEvsEReal",EenergyLost,dEenergyLost);
	  HistoMan->FillHistograms("dEvsEReal2",EenergyLost2,dEenergyLost2);
	  // An attempt at "energy" resolution of the scints
	  std::cout << "Stopping Power = " << sPower
				<< "   Proton Energy Loss in dE-array = " << dEenergyLost
				<< "   Proton Energy Loss in E-array = " << EenergyLost << std::endl;
	  
	  // Assign asymmetry in a simple way (for now)
	  double momY = mapIt2->second->momY;
	  std::cout << "Y-momentum: " << momY << std::endl;
	  if(momY < 0) asym = -1;
	  if(momY > 0) asym = +1;
	  if(momY == 0) asym = 0;
	  HistoMan->FillHistograms("asymmetryReal",asym);
	}
	
	// END TRACK LOOP
	// >>>>>>>>>>>>>>> This ends the "real NP scattering" part of the code <<<<<<<<<<<<<<<<<<<
	
	
	// >>>>>>>>>>>>>>> This begins the "Experimental Processing" part of the code <<<<<<<<<<<<<<<<<<<
    // BEGIN STEPS LOOP: Fills the detEvent map with volumes and total energy, etc.
	// This allows only events that pass NP real to be run through "our" cuts
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

	  if((!eventFlag) && (aStep->parentId == 0) && (aStep->trackId == 1) && (aStep->particleId == 2112) 
		&& (AVNum == 11) && (imprintNum == 1)) {
		taggedEvents++;
		eventFlag = true;
		
		// Neutron Diagnostics!	
		
		double xMom = aStep->momX; double yMom = aStep->momY; double zMom = aStep->momZ;
		double totMom = PhysVars->computeMomentum(xMom,yMom,zMom);
		
		double Rxy = TMath::Sqrt(TMath::Power(xMom,2)+TMath::Power(yMom,2));
		double neutronAngle = TMath::ATan(Rxy/zMom)*TMath::RadToDeg();

		t_it = tagEvent->begin();
		NpolTagger *tTemp = *t_it;
		HistoMan->FillHistograms("NeutronEnergy",(tTemp->energy));

		HistoMan->FillHistograms("NeutronMomentum",totMom);
		HistoMan->FillHistograms("NeutronThetaAngle",neutronAngle);	
	  }

	  PhysVars->fillDetectorEventMap(detEvents,aStep);
	  // ****** End of the hit position computations section ******* //
	} // END STEPS LOOP
	

	// ***** This Section will retrieve SOI and EOI and then make event selections based on cuts ******* //
    // First, we determine the SOI; then we check the other requirements and fill the histograms
	int sectionOfInterest = Process->getSectionOfInterest(&detEvents); // call method to determine SOI
	
	if(sectionOfInterest != -1) {
	  HistoMan->FillHistograms("sectionEfficiency1",(sectionOfInterest+1)); // Fill first SOI histogram
	  // Fill the Energy Deposited per array map for the SOI
	  Process->getEDepArrayTotal(&detEvents, &eDepArrayTotal, sectionOfInterest); 
	  
	  // E-array of Interest (EOI) needs some serious work.  Andrei suggestion receieved 3/27/2018. 
      PolarimeterDetector EArrayOfInterest =
		Process->getEArrayOfInterest(&detEvents,&eDepArrayTotal,sectionOfInterest);
      PolarimeterDetector dEArrayOfInterest = unknown;	  
	  
      if(EArrayOfInterest == topEArray){ dEArrayOfInterest = topdEArray; }
	  else if(EArrayOfInterest == botEArray){ dEArrayOfInterest = botdEArray; }

      if((EArrayOfInterest != unknown) && (dEArrayOfInterest != unknown)) {
		HistoMan->FillHistograms("sectionEfficiency2",(sectionOfInterest+1)); //Fill
		double eDepAnalyzer = eDepArrayTotal[analyzer]; 
		double eDepE = eDepArrayTotal[EArrayOfInterest]; 
		double eDepdE = eDepArrayTotal[dEArrayOfInterest];
		double eDepTotal = eDepAnalyzer + eDepE + eDepdE;
		double dTOF = -100.0;
		double azAngle = PhysVars->ReturnAngle(verts->at(1),&detEvents,sectionOfInterest,EArrayOfInterest,&dTOF);

		if(eDepAnalyzer >= 4.0 /*MeV*/ && eDepE >= 5.0 /*MeV */ && eDepTotal >= 50.0 /*MeV*/) { // Req. 3 and 4
		  HistoMan->FillHistograms("sectionEfficiency3",(sectionOfInterest+1)); //FILL
		  HistoMan->FillHistograms("recoilAngleRaw",azAngle);
		  if (azAngle >= angleLow && azAngle <= angleHigh){  // checks Requirement 6 
			HistoMan->FillHistograms("sectionEfficiency4",(sectionOfInterest+1)); //FILL
			HistoMan->FillHistograms("totalEnergy",eDepTotal);
			HistoMan->FillHistograms("dTOF",dTOF); //FILL
			HistoMan->FillHistograms("recoilAngle",azAngle); // Fill the proton recoil angle histo
			if(EArrayOfInterest == topEArray) {// this is where histo for top dE/E gets filled
			  HistoMan->FillHistograms("dEoverEtop",eDepE,eDepdE); //FILL
			} else if(EArrayOfInterest == botEArray) { 
			  HistoMan->FillHistograms("dEoverEbot",eDepE,eDepdE);//FILL
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

	std::map<int,NpolVertex *>::iterator v_it2;
	for(v_it2 = vertexMap.begin(); v_it2 != vertexMap.end(); v_it2++) delete v_it2->second;
	vertexMap.clear();
	
  } // END EVENT LOOP
  
  std::cout << taggedEvents << " of the initial " << totalEvents << " neutrons have crossed the Npol Tagger."
			<< std::endl;
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
  HistoMan->WriteHistograms();
  HistoMan->CloseFile();
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


	// Attempt at identifying the "good" proton track
	/*std::map<int,NpolVertex *>::iterator part_it2;
	double initNeutronEnergy = 0.0;
	for(part_it2 = vertexMap.begin(); part_it2 != vertexMap.end(); part_it2++){
	  int TID2 = part_it2->first;
	  double particleEnergy = part_it2->second->energy;
	  int pType = part_it2->second->particleId;
	  if(TID2 == 1) initNeutronEnergy = particleEnergy;

	  
	  if(TID2 > 1){
		if(pType == 2112 && particleEnergy >= 0.90*initNeutronEnergy){
		  std::cout << "TID = " << TID2 << " is our neutron of interest" << std::endl;
	  }
		
	  }
	  }*/



		// Compute Q-squared for the recoil proton?
		/*if(elasticFlag || inelasticFlag || quasielasticFlag){
		  std::map<int,NpolVertex *>::iterator mapIt;
		  double partEnergy = 0.0;
		  for(mapIt = vertexMap.begin(); mapIt != vertexMap.end(); mapIt++){
			if(mapIt->second->energy >= partEnergy
			   && mapIt->second->particleId == 2212)
			  partEnergy = mapIt->second->energy;
		  }
		  HistoMan->FillHistograms("Qsquared",PhysVars->computeQsquared(partEnergy,2212));
		  //PhysVars->printVertexMap(vertexMap,i);
		  }*/
