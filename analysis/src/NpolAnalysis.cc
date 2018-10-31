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

  std::vector<NpolStep *> *steps = NULL;
  std::vector<NpolVertex *> *verts = NULL;
  std::vector<NpolTagger *> *tagEvent = NULL;
  std::vector<NpolStatistics *> *stats = NULL;
  npolTree->SetBranchAddress("steps",&steps);
  npolTree->SetBranchAddress("tracks",&verts);
  npolTree->SetBranchAddress("NPOLTagger",&tagEvent);
  statsTree->SetBranchAddress("stats",&stats);
  
  //********************************* Define your Histograms Here *******************************
  // NP real event histograms filled from code that uses G4 super knowledge to ID event type
  //    1D-Histograms
  HistoMan->CreateHistograms("NP_recoilAngle_Elastic","NP Real Elastic Proton Recoil Angle",200, 0.0, 180.0);
  HistoMan->CreateHistograms("NP_recoilAngle_InElastic","NP Real InElastic Proton Recoil Angle",200, 0.0, 180.0);
  HistoMan->CreateHistograms("NP_recoilAngle_QuasiElastic","NP Real QuasiElastic Proton Recoil Angle",200, 0.0, 180.0);
  HistoMan->CreateHistograms("NP_recoilEnergy_Elastic","NP Real Elastic Proton Recoil Energy",200, 0.0, 350.0);
  HistoMan->CreateHistograms("NP_recoilEnergy_InElastic","NP Real InElastic Proton Recoil Energy",200, 0.0, 350.0);
  HistoMan->CreateHistograms("NP_recoilEnergy_QuasiElastic","NP Real QuasiElastic Proton Recoil Energy",200, 0.0, 350.0);
  HistoMan->CreateHistograms("NP_asymmetry_Elastic","NP Real Elastic Asymmetry from Recoil Proton",5, -2,+2);
  HistoMan->CreateHistograms("NP_asymmetry_InElastic","NP Real InElastic Asymmetry from Recoil Proton",5, -2,+2);
  HistoMan->CreateHistograms("NP_asymmetry_QuasiElastic","NP Real QuasiElastic Asymmetry from Recoil Proton",5, -2,+2);
  HistoMan->CreateHistograms("NP_RecoilMomentum_Elastic","NP Real Elastic Scattering (P_leading - P_elastic)",200, -3000.0, 1000.);
  HistoMan->CreateHistograms("NP_RecoilMomentum_InElastic","NP Real InElastic Scattering (P_leading - P_elastic)",200, -3000.0, 1000.);
  HistoMan->CreateHistograms("NP_RecoilMomentum_QuasiElastic","NP Real QuasiElastic Scattering (P_leading - P_elastic)",200, -3000.0, 1000.);
  //    2D-Histograms
  HistoMan->CreateHistograms("NP_dEvsE_Elastic", "dE over E for Real Events (Elastic)", 400,0,120,400,0,20);
  HistoMan->CreateHistograms("NP_dEvsE_InElastic", "dE over E for Real Events (InElastic)", 400,0,120,400,0,20);
  HistoMan->CreateHistograms("NP_dEvsE_QuasiElastic", "dE over E for Real Events (QuasiElastic)", 400,0,120,400,0,20);
  HistoMan->CreateHistograms("NP_dEvsE2_Elastic", "dE over E for Real Events with Energy Resolution (Elastic)", 400,0,120,400,0,20);
  HistoMan->CreateHistograms("NP_dEvsE2_InElastic", "dE over E for Real Events with Energy Resolution (InElastic)", 400,0,120,400,0,20);
  HistoMan->CreateHistograms("NP_dEvsE2_QuasiElastic", "dE over E for Real Events with Energy Resolution (QuasiElastic)", 400,0,120,400,0,20);
  
  // Analysis histograms filled from code that uses methods and cuts similar to experiment
  //    1D-Histograms
  HistoMan->CreateHistograms("RA_dTOF_Elastic","Delta time-of-flight (Elastic)",600,-30,120);
  HistoMan->CreateHistograms("RA_dTOF_InElastic","Delta time-of-flight (InElastic)",600,-30,120);
  HistoMan->CreateHistograms("RA_dTOF_QuasiElastic","Delta time-of-flight (QuasiElastic)",600,-30,120);
  HistoMan->CreateHistograms("RA_totalEnergy_Elastic","Total Energy Deposited (Elastic)", 100, 0.0, 350.0);
  HistoMan->CreateHistograms("RA_totalEnergy_InElastic","Total Energy Deposited (InElastic)", 100, 0.0, 350.0);
  HistoMan->CreateHistograms("RA_totalEnergy_QuasiElastic","Total Energy Deposited (QuasiElastic)", 100, 0.0, 350.0);
  HistoMan->CreateHistograms("RA_recoilAngleRaw_Elastic","Proton Recoil Angle Before Angle Cut (Elastic)", 200, 0.0, 180.0);
  HistoMan->CreateHistograms("RA_recoilAngleRaw_InElastic","Proton Recoil Angle Before Angle Cut (InElastic)", 200, 0.0, 180.0);
  HistoMan->CreateHistograms("RA_recoilAngleRaw_QuasiElastic","Proton Recoil Angle Before Angle Cut (QuasiElastic)", 200, 0.0, 180.0);
  HistoMan->CreateHistograms("RA_recoilAngle_Elastic","Proton Recoil Angle (Elastic)", 200, 0.0, 180.0);
  HistoMan->CreateHistograms("RA_recoilAngle_InElastic","Proton Recoil Angle (InElastic)", 200, 0.0, 180.0);
  HistoMan->CreateHistograms("RA_recoilAngle_QuasiElastic","Proton Recoil Angle (QuasiElastic)", 200, 0.0, 180.0);
  HistoMan->CreateHistograms("RA_dEoverEtop_Elastic", "dE over E for top array (Elastic)", 400,0,120,400,0,20);
  HistoMan->CreateHistograms("RA_dEoverEbot_Elastic", "dE over E for bottom array (Elastic)", 400,0,120,400,0,20);
  HistoMan->CreateHistograms("RA_dEoverEtop_InElastic", "dE over E for top array (InElastic)", 400,0,120,400,0,20);
  HistoMan->CreateHistograms("RA_dEoverEbot_InElastic", "dE over E for bottom array (InElastic)", 400,0,120,400,0,20);
  HistoMan->CreateHistograms("RA_dEoverEtop_QuasiElastic", "dE over E for top array (QuasiElastic)", 400,0,120,400,0,20);
  HistoMan->CreateHistograms("RA_dEoverEbot_QuasiElastic", "dE over E for bottom array (QuasiElastic)", 400,0,120,400,0,20);
   
  HistoMan->CreateHistograms("RA_section1Efficiency_Elastic","NPOL Efficiency after SOI Selection (Elastic)",13,0.25,6.75);
  HistoMan->CreateHistograms("RA_section2Efficiency_Elastic","#splitline{NPOL Efficiency after EOI Selection}{and Asymmetry Cut} (Elastic)",13,0.25,6.75);
  HistoMan->CreateHistograms("RA_section3Efficiency_Elastic","#splitline{NPOL Efficiency after Array}{Total Energy Cuts} (Elastic)",13,0.25,6.75);
  HistoMan->CreateHistograms("RA_section4Efficiency_Elastic","NPOL Efficiency after Angle Cut (Elastic)",13,0.25,6.75);
  HistoMan->CreateHistograms("RA_section1Efficiency_InElastic","NPOL Efficiency after SOI Selection (InElastic)",13,0.25,6.75);
  HistoMan->CreateHistograms("RA_section2Efficiency_InElastic","#splitline{NPOL Efficiency after EOI Selection}{and Asymmetry Cut} (InElastic)",13,0.25,6.75);
  HistoMan->CreateHistograms("RA_section3Efficiency_InElastic","#splitline{NPOL Efficiency after Array}{Total Energy Cuts} (InElastic)",13,0.25,6.75);
  HistoMan->CreateHistograms("RA_section4Efficiency_InElastic","NPOL Efficiency after Angle Cut (InElastic)",13,0.25,6.75);
  HistoMan->CreateHistograms("RA_section1Efficiency_QuasiElastic","NPOL Efficiency after SOI Selection (QuasiElastic)",13,0.25,6.75);
  HistoMan->CreateHistograms("RA_section2Efficiency_QuasiElastic","#splitline{NPOL Efficiency after EOI Selection}{and Asymmetry Cut} (QuasiElastic)",13,0.25,6.75);
  HistoMan->CreateHistograms("RA_section3Efficiency_QuasiElastic","#splitline{NPOL Efficiency after Array}{Total Energy Cuts} (QuasiElastic)",13,0.25,6.75);
  HistoMan->CreateHistograms("RA_section4Efficiency_QuasiElastic","NPOL Efficiency after Angle Cut (QuasiElastic)",13,0.25,6.75);
  
  // Neutron diagnostics histograms which are filled so we can observe the original neutron physics
  //    1D-Histograms
  HistoMan->CreateHistograms("ND_InitialNeutronMomentum_Elastic","Initial Neutron Momentum when Generated (Elastic)",100, 1600.0, 3200.0);
  HistoMan->CreateHistograms("ND_InitialNeutronMomentum_InElastic","Initial Neutron Momentum when Generated (InElastic)",100, 1600.0, 3200.0);
  HistoMan->CreateHistograms("ND_InitialNeutronMomentum_QuasiElastic","Initial Neutron Momentum when Generated (QuasiElastic)",100, 1600.0, 3200.0);
  HistoMan->CreateHistograms("ND_InitialNeutronEnergy_Elastic","Initial Neutron Energy when Generated (Elastic)",100, 1000.0, 2400.0); 	
  HistoMan->CreateHistograms("ND_InitialNeutronEnergy_InElastic","Initial Neutron Energy when Generated (InElastic)",100, 1000.0, 2400.0);
  HistoMan->CreateHistograms("ND_InitialNeutronEnergy_QuasiElastic","Initial Neutron Energy when Generated (QuasiElastic)",100, 1000.0, 2400.0);
  
  HistoMan->CreateHistograms("ND_NeutronThetaAngle_Elastic","Neutron Angle at first tagger (Elastic)", 100, 15.0, 40.0);
  HistoMan->CreateHistograms("ND_NeutronThetaAngle_InElastic","Neutron Angle at first tagger (InElastic)", 100, 15.0, 40.0);
  HistoMan->CreateHistograms("ND_NeutronThetaAngle_QuasiElastic","Neutron Angle at first tagger (QuasiElastic)", 100, 15.0, 40.0);
  HistoMan->CreateHistograms("ND_NeutronMomentum_Elastic","Neutron Momentum at the first tagger (Elastic)",100, 1600.0, 3200.0);
  HistoMan->CreateHistograms("ND_NeutronMomentum_InElastic","Neutron Momentum at the first tagger (InElastic)",100, 1600.0, 3200.0);
  HistoMan->CreateHistograms("ND_NeutronMomentum_QuasiElastic","Neutron Momentum at the first tagger (QuasiElastic)",100, 1600.0, 3200.0);
  HistoMan->CreateHistograms("ND_NeutronEnergy_Elastic","Neutron Energy at NPOL Tagger (Elastic)",100, 1000.0, 2400.0);
  HistoMan->CreateHistograms("ND_NeutronEnergy_InElastic","Neutron Energy at NPOL Tagger (InElastic)",100, 1000.0, 2400.0);
  HistoMan->CreateHistograms("ND_NeutronEnergy_QuasiElastic","Neutron Energy at NPOL Tagger (QuasiElastic)",100, 1000.0, 2400.0);

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
  int nentries = npolTree->GetEntries();
  TRandom *rand = new TRandom3();
  for(int i = 0; i < nentries; i++) {
	//for(int i = 0; i < 2000; i++) {
   	if(i % 1000 == 0) std::cout << "Processing event #" << i << std::endl;
	
    npolTree->GetEntry(i);
	
	std::pair<double,std::vector<double> > initNeutron4Vec;
	std::pair<double,std::vector<double> > recoilParticle4Vec;
	std::pair<double,std::vector<double> > projNeutron4Vec;
	std::pair<double,std::vector<double> > scattNeutron4Vec;
	std::pair<double,std::vector<double> > scattParticle4Vec;
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
	// two types of events.  Added in some changes to the inelastic routine (21-Sept-2018) to sort out the
	// quasielastic events via looking at the number of particles in the final state.  I first considered this
	// and Andrei confirmed my ideas and sent me how they sorted it out.
	// This is to analyze simulation output for "real" (n,p) scattering events
	
	int npSOI = -2; int npPID = -1; int stepCount = -1;
	bool inelasticFlag = false; bool elasticFlag = false;
	bool quasielasticFlag = false; 
	std::string physProcess = " ";
	std::vector<NpolStep *>::iterator ps_it;
	std::vector<NpolStep *>::iterator oneBack_it;
	for(ps_it = steps->begin(); ps_it != steps->end(); ps_it++) {
	  stepCount++;
	  NpolStep *aStep = *ps_it;
	  NpolStep *aStepOrig = new NpolStep();
	  if(aStep == NULL) continue;
	  if(stepCount >= 1) aStepOrig = steps->at(stepCount - 1); // sub. 1 to get previous pointer(entry)
	  if(aStepOrig == NULL) continue;
	  
	  physProcess = aStep->process;
	  std::string volName = aStep->volume;
	  double time = aStep->time;
	  int AVNum = PProcess->GetAVNumber(volName);
	  int section = Process->sectionNumber(volName);
	  int PID = aStep->parentId;
	  int TID = aStep->trackId;
	  
	  if((PID == 0 && TID == 1) && (AVNum == 9 || AVNum == 10)){			
		if(physProcess == "hadElastic"){
		  elasticFlag = true;
		  Process->fillFourVector(aStepOrig,projNeutron4Vec);
		  Process->fillVertexMap(vertexMap,verts,1,volName,physProcess,time);
		 
		} else if(physProcess == "neutronInelastic"){
		  int neutronCount = 0; int protonCount = 0; int gammaCount = 0;
		  int othersCount = 0; int pionCount = 0;
		  
		  Process->fillFourVector(aStepOrig,projNeutron4Vec);
		  Process->fillVertexMap(vertexMap,verts,1,volName,physProcess,time);
		  
		  std::map<int,NpolVertex *>::iterator mapIt;
		  for(mapIt = vertexMap.begin(); mapIt != vertexMap.end(); mapIt++){
			int pType = mapIt->second->particleId;
			if(mapIt->second->parentId == 0) continue;
			if(pType == 2112){
			  neutronCount++;
			} else if(pType == 2212){
			  protonCount++;
			} else if(pType == 22){
			  gammaCount++;
			} else if(pType == 111 || pType == 211 || pType == -211){
			  pionCount++;
			} else {
			  othersCount++;
			}
		  }
		  
		  if(pionCount > 0) {
			inelasticFlag = true;
		  } else if(neutronCount >= 0 && protonCount >= 0 && gammaCount >= 0){
			quasielasticFlag = PhysVars->checkQuasiElasticScattering(vertexMap, projNeutron4Vec);
			if(!(quasielasticFlag)) inelasticFlag = true; 
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
	}  // End Step Loop
	
	
	
	// ***** This statement checks to see if the flags of events NOT of interest  ***** //
	// are set. If so then the event processing is canceled by the continue statment
	// which cycles the loop. This kills the EVENT as a WHOLE; Beware!! //
	
	// Check if NO flags set; kill event processing
	if(!(elasticFlag) && !(inelasticFlag) && !(quasielasticFlag)) continue;  
	
	
	// ******* This section checks/fills histograms for the NP "real" portion ********* //
	
	// ****** This section computes the (P_leading - P_elastic) ******
	// ****** value and saves into histogram                    ******
	int leadingTID = PhysVars->findLeadingParticle(vertexMap,physProcess);
	if(leadingTID != -1){
	  double computedRecoilAngle = PhysVars->computeRecoilParticleAngle(projNeutron4Vec,vertexMap,leadingTID);
	  Process->fillFourVector(vertexMap.find(leadingTID)->second,recoilParticle4Vec);
	  double leadingParticleMomentum = PhysVars->returnParticleMomentum(recoilParticle4Vec);
	  double elasticMomentum = PhysVars->computeElasticMomentum(projNeutron4Vec, computedRecoilAngle*TMath::DegToRad());
	  double projectileNeutronMom = PhysVars->returnParticleMomentum(projNeutron4Vec);
	  
	  if(elasticFlag) std::cout << "Elastic Event" << std::endl;
	  if(inelasticFlag) std::cout << "InElastic Event" << std::endl;
	  if(quasielasticFlag) std::cout << "Quasi-elastic Event" << std::endl;
	  std::cout << "  Leading TID: " << leadingTID << std::endl;
	  std::cout << "  Recoil Angle Computed: " << computedRecoilAngle << std::endl;
	  std::cout << "  Neutron Momentum before: " <<  projectileNeutronMom << std::endl;
	  std::cout << "  Leading Particle Momentum: " << leadingParticleMomentum << std::endl;
	  std::cout << "  Elastic Momentum: " << elasticMomentum << std::endl;
	  Process->printVertexMap(vertexMap,i);
	  
	  if(elasticFlag) HistoMan->FillHistograms("NP_RecoilMomentum_Elastic",(leadingParticleMomentum - elasticMomentum));
	  if(inelasticFlag) HistoMan->FillHistograms("NP_RecoilMomentum_InElastic",(leadingParticleMomentum - elasticMomentum));
	  if(quasielasticFlag) HistoMan->FillHistograms("NP_RecoilMomentum_QuasiElastic",(leadingParticleMomentum - elasticMomentum));
	}
	// ****** End (P_leading - P_elastic) code ******
	
	
	// **** After the proton track has been found, this code will compute values, **** //
	// fill histograms. Extract out Initial Neutron Information from the vertexMap
	int bestProtonTID = PhysVars->findBestProtonTrackID(vertexMap,steps,npSOI);
	if(bestProtonTID != 0){
	  
	  std::map<int,NpolVertex *>::iterator mapIt2;
	  int asym = 0;
	  double initialNeutronMomentum = 0.;
	  
	  //  Neutron Diagnostics!
	  mapIt2 = vertexMap.find(1);
	  Process->fillFourVector(mapIt2->second,initNeutron4Vec);
	  initialNeutronMomentum = PhysVars->returnParticleMomentum(initNeutron4Vec);
	  if(elasticFlag){
		HistoMan->FillHistograms("ND_InitialNeutronMomentum_Elastic",initialNeutronMomentum);
		HistoMan->FillHistograms("ND_InitialNeutronEnergy_Elastic",(mapIt2->second->energy));
	  }
	  if(inelasticFlag){
		HistoMan->FillHistograms("ND_InitialNeutronMomentum_InElastic",initialNeutronMomentum);
		HistoMan->FillHistograms("ND_InitialNeutronEnergy_InElastic",(mapIt2->second->energy));
	  }
	  if(quasielasticFlag){
		HistoMan->FillHistograms("ND_InitialNeutronMomentum_QuasiElastic",initialNeutronMomentum);
		HistoMan->FillHistograms("ND_InitialNeutronEnergy_QuasiElastic",(mapIt2->second->energy));
	  }
	  
	  // fill selected proton track histograms
	  mapIt2 = vertexMap.find(bestProtonTID);
	  double computedProtonAngle = PhysVars->
		computeRecoilParticleAngle(projNeutron4Vec,vertexMap,bestProtonTID);
	  if(computedProtonAngle >= angleLow && computedProtonAngle <= angleHigh) {
		double protonVertexEnergy = mapIt2->second->energy;
		double sPower = PhysVars->
		  computeBetheBloch(protonVertexEnergy,938.27205,1,1.032,12.929,7,64.7e-6);  
		double dEenergyLost = PhysVars->
		  computeEnergyLoss(protonVertexEnergy,TMath::DegToRad()*computedProtonAngle,1./*cm*/);
		Double_t dEenergyLost2 = rand->Gaus(dEenergyLost, 0.10*dEenergyLost);
		double EenergyLost = PhysVars->
		  computeEnergyLoss(protonVertexEnergy,TMath::DegToRad()*computedProtonAngle,10. /*cm*/);
		Double_t EenergyLost2 = rand->Gaus(EenergyLost, 0.10*EenergyLost);
		double momY = mapIt2->second->momY; 
		if(momY < 0) asym = -1;  // Assign asymmetry in a simple way (for now)
		if(momY > 0) asym = +1;
		if(momY == 0) asym = 0;
		
		if(elasticFlag){
		  HistoMan->FillHistograms("NP_recoilAngle_Elastic",computedProtonAngle);
		  HistoMan->FillHistograms("NP_recoilEnergy_Elastic",protonVertexEnergy);
		  HistoMan->FillHistograms("NP_dEvsE_Elastic",EenergyLost,dEenergyLost);
		  HistoMan->FillHistograms("NP_dEvsE2_Elastic",EenergyLost2,dEenergyLost2);
		  HistoMan->FillHistograms("NP_asymmetry_Elastic",asym);
		}
		if(inelasticFlag){
		  HistoMan->FillHistograms("NP_recoilAngle_InElastic",computedProtonAngle);
		  HistoMan->FillHistograms("NP_recoilEnergy_InElastic",protonVertexEnergy);
		  HistoMan->FillHistograms("NP_dEvsE_InElastic",EenergyLost,dEenergyLost);
		  HistoMan->FillHistograms("NP_dEvsE2_InElastic",EenergyLost2,dEenergyLost2);
		  HistoMan->FillHistograms("NP_asymmetry_InElastic",asym);
		}
		if(quasielasticFlag){
		  HistoMan->FillHistograms("NP_recoilAngle_QuasiElastic",computedProtonAngle);
		  HistoMan->FillHistograms("NP_recoilEnergy_QuasiElastic",protonVertexEnergy);
		  HistoMan->FillHistograms("NP_dEvsE_QuasiElastic",EenergyLost,dEenergyLost);
		  HistoMan->FillHistograms("NP_dEvsE2_QuasiElastic",EenergyLost2,dEenergyLost2);
		  HistoMan->FillHistograms("NP_asymmetry_QuasiElastic",asym);
	  }
		// An attempt at "energy" resolution of the scints
		std::cout << "Stopping Power = " << sPower
				  << "   Proton Energy Loss in dE-array = " << dEenergyLost
				  << "   Proton Energy Loss in E-array = " << EenergyLost << std::endl;
	  }
	}
	
	// >>>>>>>>>>>>> This ends the "real NP scattering" part of the code <<<<<<<<<<<< //
  
  
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
		
		// More Neutron Diagnostics!	
		double xMom = aStep->momX; double yMom = aStep->momY; double zMom = aStep->momZ;
		double totMom = PhysVars->computeMomentum(xMom,yMom,zMom);
		
		double Rxy = TMath::Sqrt(TMath::Power(xMom,2)+TMath::Power(yMom,2));
		double neutronAngle = TMath::ATan(Rxy/zMom)*TMath::RadToDeg();
		
		t_it = tagEvent->begin();
		NpolTagger *tTemp = *t_it;
		if(elasticFlag){
		  HistoMan->FillHistograms("ND_NeutronEnergy_Elastic",(tTemp->energy));
		  HistoMan->FillHistograms("ND_NeutronMomentum_Elastic",totMom);
		  HistoMan->FillHistograms("ND_NeutronThetaAngle_Elastic",neutronAngle);
		}
		if(inelasticFlag){
		  HistoMan->FillHistograms("ND_NeutronEnergy_InElastic",(tTemp->energy));
		  HistoMan->FillHistograms("ND_NeutronMomentum_InElastic",totMom);
		  HistoMan->FillHistograms("ND_NeutronThetaAngle_InElastic",neutronAngle);
		}
		if(quasielasticFlag){
		  HistoMan->FillHistograms("ND_NeutronEnergy_QuasiElastic",(tTemp->energy));
		  HistoMan->FillHistograms("ND_NeutronMomentum_QuasiElastic",totMom);
		  HistoMan->FillHistograms("ND_NeutronThetaAngle_QuasiElastic",neutronAngle);
		}
	  }
	  Process->fillDetectorEventMap(detEvents,aStep);
	  // ****** End of the hit position computations section ******* //
	} // END STEPS LOOP
	
	
	// ***** This Section will retrieve SOI and EOI and then make event selections based on cuts ******* //
    // First, we determine the SOI; then we check the other requirements and fill the histograms
	int sectionOfInterest = Process->getSectionOfInterest(&detEvents); // call method to determine SOI
	
	if(sectionOfInterest != -1) {
	  if(elasticFlag) HistoMan->FillHistograms("RA_section1Efficiency_Elastic",(sectionOfInterest+1)); // Fill first SOI histogram
	  if(inelasticFlag) HistoMan->FillHistograms("RA_section1Efficiency_InElastic",(sectionOfInterest+1));
	  if(quasielasticFlag) HistoMan->FillHistograms("RA_section1Efficiency_QuasiElastic",(sectionOfInterest+1));
	  // Fill the Energy Deposited per array map for the SOI
	  Process->getEDepArrayTotal(&detEvents, &eDepArrayTotal, sectionOfInterest); 
	  
	  // E-array of Interest (EOI) needs some serious work.  Andrei suggestion receieved 3/27/2018. 
      PolarimeterDetector EArrayOfInterest =
		Process->getEArrayOfInterest(&detEvents,&eDepArrayTotal,sectionOfInterest);
      PolarimeterDetector dEArrayOfInterest = unknown;	  
	  
      if(EArrayOfInterest == topEArray){ dEArrayOfInterest = topdEArray; }
	  else if(EArrayOfInterest == botEArray){ dEArrayOfInterest = botdEArray; }
	  
      if((EArrayOfInterest != unknown) && (dEArrayOfInterest != unknown)) {
		if(elasticFlag) HistoMan->FillHistograms("RA_section2Efficiency_Elastic",(sectionOfInterest+1)); //Fill
		if(inelasticFlag) HistoMan->FillHistograms("RA_section2Efficiency_InElastic",(sectionOfInterest+1));
		if(quasielasticFlag) HistoMan->FillHistograms("RA_section2Efficiency_QuasiElastic",(sectionOfInterest+1));
		double eDepAnalyzer = eDepArrayTotal[analyzer]; 
		double eDepE = eDepArrayTotal[EArrayOfInterest]; 
		double eDepdE = eDepArrayTotal[dEArrayOfInterest];
		double eDepTotal = eDepAnalyzer + eDepE + eDepdE;
		double dTOF = -100.0;
		double azAngle = PhysVars->ReturnAngle(verts->at(1),&detEvents,sectionOfInterest,EArrayOfInterest,&dTOF);
		
		if(eDepAnalyzer >= 4.0 /*MeV*/ && eDepE >= 5.0 /*MeV */ && eDepTotal >= 50.0 /*MeV*/) { // Req. 3 and 4
		  if(elasticFlag){
			HistoMan->FillHistograms("RA_section3Efficiency_Elastic",(sectionOfInterest+1)); //FILL
			HistoMan->FillHistograms("RA_recoilAngleRaw_Elastic",azAngle);
		  }
		  if(inelasticFlag){
			HistoMan->FillHistograms("RA_section3Efficiency_InElastic",(sectionOfInterest+1)); //FILL
			HistoMan->FillHistograms("RA_recoilAngleRaw_InElastic",azAngle);
		  }
		  if(quasielasticFlag){
			HistoMan->FillHistograms("RA_section3Efficiency_QuasiElastic",(sectionOfInterest+1)); //FILL
			HistoMan->FillHistograms("RA_recoilAngleRaw_QuasiElastic",azAngle);
		  }
		  		  
		  if (azAngle >= angleLow && azAngle <= angleHigh){  // checks Requirement 6 
			if(elasticFlag){
			  HistoMan->FillHistograms("RA_section4Efficiency_Elastic",(sectionOfInterest+1)); //FILL
			  HistoMan->FillHistograms("RA_totalEnergy_Elastic",eDepTotal);
			  HistoMan->FillHistograms("RA_dTOF_Elastic",dTOF); //FILL
			  HistoMan->FillHistograms("RA_recoilAngle_Elastic",azAngle); // Fill the proton recoil angle histo
			}
			if(inelasticFlag){
			  HistoMan->FillHistograms("RA_section4Efficiency_InElastic",(sectionOfInterest+1)); //FILL
			  HistoMan->FillHistograms("RA_totalEnergy_InElastic",eDepTotal);
			  HistoMan->FillHistograms("RA_dTOF_InElastic",dTOF); //FILL
			  HistoMan->FillHistograms("RA_recoilAngle_InElastic",azAngle); // Fill the proton recoil angle histo
			}
			if(quasielasticFlag){
			  HistoMan->FillHistograms("RA_section4Efficiency_QuasiElastic",(sectionOfInterest+1)); //FILL
			  HistoMan->FillHistograms("RA_totalEnergy_QuasiElastic",eDepTotal);
			  HistoMan->FillHistograms("RA_dTOF_QuasiElastic",dTOF); //FILL
			  HistoMan->FillHistograms("RA_recoilAngle_QuasiElastic",azAngle); // Fill the proton recoil angle histo
			}  
			
			if(EArrayOfInterest == topEArray) {// this is where histo for top dE/E gets filled
			  if(elasticFlag) HistoMan->FillHistograms("RA_dEoverEtop_Elastic",eDepE,eDepdE); //FILL
			  if(inelasticFlag) HistoMan->FillHistograms("RA_dEoverEtop_InElastic",eDepE,eDepdE); //FILL
			  if(quasielasticFlag) HistoMan->FillHistograms("RA_dEoverEtop_QuasiElastic",eDepE,eDepdE); //FILL
			  
			} else if(EArrayOfInterest == botEArray) { 
			  if(elasticFlag) HistoMan->FillHistograms("RA_dEoverEbot_Elastic",eDepE,eDepdE);//FILL
			  if(inelasticFlag) HistoMan->FillHistograms("RA_dEoverEbot_InElastic",eDepE,eDepdE);//FILL
			  if(quasielasticFlag) HistoMan->FillHistograms("RA_dEoverEbot_QuasiElastic",eDepE,eDepdE);//FILL
			  
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
	
	initNeutron4Vec.second.clear();
	recoilParticle4Vec.second.clear();
	projNeutron4Vec.second.clear();
	scattNeutron4Vec.second.clear();
	scattParticle4Vec.second.clear();
	
  }	// END EVENT LOOP
  
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



  //npolTree->SetCacheSize(50000000);  // This increases the amount of data loaded 
  //statsTree->SetCacheSize(50000000); // per call for more data to chew on. 
  
