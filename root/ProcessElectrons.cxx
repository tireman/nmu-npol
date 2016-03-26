
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>

#include <TMath.h>
#include <TFile.h>
#include <TChain.h>
#include <TBranch.h>
#include <TH1.h>
#include <TH2.h>
#include <TSystem.h>
#include <TROOT.h>
#include <TObject.h>
#include <TVectorD.h>
#include <TString.h>

#include "../include/NpolVertex.hh"
#include "../include/NpolTagger.hh"
#include "../include/NpolStatistics.hh"
#include "../include/NpolStep.hh"

void RetrieveENVvariables();
TString FormInputFile(TString InputDir);
TString FormOutputFile(TString OutputDir);
void PrintEventNumber(int nentries, int i);
int GetAVNumber(const std::string &volName);
int GetImprNumber(const std::string &volName);
int GetPlacementNumber(const std::string &volName);
double *AntilogBins(const int nbins, const double xmin, const double xmax); 

TString BaseName = "";
TString JobNum = "";
TString Lead = ""; 
TString Energy = "";
TString Bfield = "";
TString OutputDir = "";
TString InputDir = "";

void ProcessElectrons() {
  gSystem->Load("NpolClass.so"); 

  // Set up the TTrees and their branch addresses
  TChain *npolTree = new TChain("T");
  TChain *statsTree = new TChain("T2");

  RetrieveENVvariables();

  TString InputFile = FormInputFile(InputDir);
  TString OutputFile = FormOutputFile(OutputDir);

  TFile *outFile = new TFile(OutputFile,"RECREATE"); 

  npolTree->Add(InputFile);
  statsTree->Add(InputFile);

  std::vector<NpolVertex *> *vertexEntry = NULL;
  std::vector<NpolTagger *> *npolEntry = NULL;
  std::vector<NpolTagger *> *targetEntry = NULL;
  std::vector<NpolTagger *> *shmsEntry = NULL;
  std::vector<NpolStatistics *> *anStat = NULL;
  std::vector<NpolStep *> *anStep = NULL;

  npolTree->SetBranchAddress("NPOLTagger",&npolEntry);
  npolTree->SetBranchAddress("SHMSTagger",&shmsEntry);
  npolTree->SetBranchAddress("ParticleTagger",&targetEntry);
  npolTree->SetBranchAddress("tracks",&vertexEntry);
  npolTree->SetBranchAddress("steps",&anStep);
  statsTree->SetBranchAddress("stats",&anStat);

  // Count the total number of electrons on target and total events 
  // saved to the files 
  Long_t TotalElectrons = 0;
  Long_t TotalEventsRecorded = 0;
  std::cout << "Number of enteries " << statsTree->GetEntries() << std::endl;
  for(int i = 0; i < statsTree->GetEntries(); i++){ 
    statsTree->GetEntry(i); 
    TotalElectrons += ((*anStat)[0])->totalEvents;
    TotalEventsRecorded += ((*anStat)[0])->eventsSaved;   
  }
  
  std::cout << "Total electrons: " << TotalElectrons/1e6 << " Million" << std::endl;
  
  // The particle names as they will appear in the histogram titles
  std::map<std::string,std::string> fancyNames;
  fancyNames["gamma"] = "Photon";
  fancyNames["e-"] = "Electron";
  fancyNames["e+"] = "Positron";
  fancyNames["mu-"] = "Negative Muon";
  fancyNames["mu+"] = "Positive Muon";
  fancyNames["pi-"] = "Negative Pion";
  fancyNames["pi+"] = "Positive Pion";
  fancyNames["proton"] = "Protron";
  fancyNames["neutron"] = "Neutron";
  
  const int nbins = 200;
  double *bins = AntilogBins(nbins,1e-1,1e4);
  std::map<std::string,TH1 *> histograms;
  std::map<std::string,TH1F *> targetParticleKE;
  std::map<std::string,TH1F *> npolParticleKE;
  std::map<std::string,TH1F *> correlateKE;
  std::map<std::string,TH2F *> targetParticlePOS;
  std::map<std::string,TH2F *> npolParticlePOS;
  std::map<std::string,TH2F *> correlatePOS;

  // Allocate KE histograms and Position Histograms
  std::map<std::string,std::string>::iterator it;
  for(it = fancyNames.begin(); it != fancyNames.end(); it++) {
    std::string targetHistoName = "TargetFlux_" + it->first; 
    std::string targetHistoTitle = it->second + " Flux vs. KE in Target Tagger"; 
    std::string npolHistoName = "NpolFlux_" + it->first; 
    std::string npolHistoTitle = it->second + " Flux vs. KE in NPOL Tagger";
    std::string correlateHistoName = "Correlated_TargetFlux_" + it->first;
    std::string correlateHistoTitle = it->second + " Flux vs. KE Target Tagger Correlated to NPOL Tagger";

    std::string targetXYHistoName = "targetXY_" + it->first;
    std::string npolXYHistoName = "npolXY_" + it->first;
    std::string targetXYHistoTitle = it->second + " XY Position in Target Tagger";
    std::string npolXYHistoTitle = it->second + " XY Position in NPOL Tagger";
    std::string correlateXYHistoName = "Correlated_targetXY_" + it->first;
    std::string correlateXYTitle = it->second + " XY position in Target Tagger Correlated to NPOL Tagger";

    targetParticleKE[it->first] = new TH1F(
     targetHistoName.c_str(), targetHistoTitle.c_str(),nbins,bins);
    targetParticlePOS[it->first] = new TH2F(targetXYHistoName.c_str(),
     targetXYHistoTitle.c_str(),70,40.0,75.0,60,-15,15);  

    npolParticleKE[it->first] = new TH1F(
	  npolHistoName.c_str(), npolHistoTitle.c_str(),nbins,bins);
    npolParticlePOS[it->first] = new TH2F(npolXYHistoName.c_str(),
     npolXYHistoTitle.c_str(),280,220.0,360.0,320,-80,80);

    correlateKE[it->first] = new TH1F(
     correlateHistoName.c_str(), correlateHistoTitle.c_str(),nbins,bins);
    correlatePOS[it->first] = new TH2F(correlateXYHistoName.c_str(),
     correlateXYTitle.c_str(),70,40.0,75.0,60,-15,15);

  }  

  // Allocate the dTOF histogram
  TH1F *delta_TOF = 
    new TH1F("dToF", "Time of flight between Analyzer and Top/Bottom Detectors", 
	     500, -15.0, 200.0);
  delta_TOF->GetYaxis()->SetTitle("Events");
  delta_TOF->GetXaxis()->SetTitle("Analyzer to Top/Bottom Array Time-of-Flight (ns)");

  // loop over all entries (one per event)
  Int_t nentries = npolTree->GetEntries();
  //for(int i = 0; i < 100000; i++) {
  for(int i = 0; i < nentries; i++) {
    npolTree->GetEntry(i);
	
	PrintEventNumber(nentries,i);

	std::set<int> npolTrackIDs;
	//std::set<int> vertexTrackIDs;

	// loop over all vertex entries to find particles created in
	// the lead curtain or front wall of shield hut
	/*std::vector<NpolVertex *>::iterator v_it;
	for(v_it = vertexEntry->begin(); v_it != vertexEntry->end(); v_it++){
	  NpolVertex *aVertex = *v_it;
	  if(aVertex == NULL) continue;
	  std::string volumeName = aVertex->volume;
	  if((volumeName == "HutFrontWall") || (volumeName == "LeadCurtain") || (volumeName == "ExpHall") 
	    || (volumeName == "NPOLTagger")){
		vertexTrackIDs.insert(aVertex->trackId);
	  }
	  }*/
	
    // loop over all tagged particles (one step in NPOL tagger volume)
    std::vector<NpolTagger *>::iterator n_it;
    for(n_it = npolEntry->begin(); n_it != npolEntry->end(); n_it++){
      NpolTagger *npolTagged = *n_it;
      if(npolTagged == NULL)  continue;
	  std::string particleName = npolTagged->particle;
      if(npolParticleKE.find(particleName) == npolParticleKE.end())
		continue;
	  Double_t fluxscaling = 1;///(pow(618.0,2)*4*asin(sin(73.74e-3)*sin(61.82e-3))*log10(npolTagged->energy*1e6));

	  //if(vertexTrackIDs.find(npolTagged->trackId) == vertexTrackIDs.end()){
		(npolParticleKE[particleName])->
		  Fill(npolTagged->energy,fluxscaling);
		(npolParticlePOS[particleName])->
		  Fill(abs(npolTagged->posX),npolTagged->posY);
		
		//}
	  npolTrackIDs.insert(npolTagged->trackId);
	}
	//vertexTrackIDs.clear();

    // loop over all tagged particles (one step in target tagger volume)
    Double_t xOffSet = 112.0*sin(28.0*3.1416/180);
    std::vector<NpolTagger *>::iterator t_it;
    for(t_it = targetEntry->begin(); t_it != targetEntry->end(); t_it++){
      NpolTagger *targetTagged = *t_it;
      if(targetTagged == NULL) continue;  
      std::string particleName = targetTagged->particle;
      if(targetParticleKE.find(particleName) == targetParticleKE.end())
		continue;
	  Double_t fluxscaling = 1;///(TotalElectrons*pow(112.0,2)*4*asin(sin(73.74e-3)*sin(61.82e-3))*log10(targetTagged->energy*1e6));
      Double_t Xcenter = abs(targetTagged->posX) - xOffSet;
      if((abs(targetTagged->posY) <= 8.115) && (abs(Xcenter) <= 9.750)){
		(targetParticleKE[particleName])->
		  Fill(targetTagged->energy,fluxscaling);
		(targetParticlePOS[particleName])->
		  Fill(abs(targetTagged->posX),targetTagged->posY);
		
		// Here the correlated histograms are to be filled
		if(npolTrackIDs.find(targetTagged->trackId) 
		   != npolTrackIDs.end()){  

		  (correlateKE[particleName])->
			Fill(targetTagged->energy,fluxscaling);
		  (correlatePOS[particleName])->
			Fill(abs(targetTagged->posX),targetTagged->posY);
		}
	  }
	}
	npolTrackIDs.clear();
  
    // This section is designed to file up the histograms for hits in
    // the Scintillator detectors
    std::map<std::string, double> eDep;
    std::map<std::string, double> hitTime;

    // loop over vector elements (one per vertex)
    
    int avNum, imprNum, pvNum;
    std::vector<NpolStep *>::iterator s_it;
    for(s_it = anStep->begin(); s_it != anStep->end(); s_it++){
      NpolStep *aStep = *s_it;
      double Ethreshold;
      
      if(aStep == NULL) continue;
      
      if(eDep.find(aStep->volume) == eDep.end())
		eDep[aStep->volume] = 0;
      eDep[aStep->volume] += aStep->eDep;
  
      avNum = GetAVNumber(aStep->volume);
      switch(avNum){
      case 1: case 2: case 5: case 6:
		Ethreshold = 1.0;
		break;
      case 3: case 4: case 7: case 8:
		Ethreshold = 1.0;
		break;
      case 9: case 10:
		Ethreshold = 1.0;
		break;
      default:
		break;
      }
      
      if(avNum != 0){
		if(hitTime.find(aStep->volume) == hitTime.end() 
		   && eDep[aStep->volume] >= Ethreshold)
		  hitTime[aStep->volume] = aStep->time;
      }
    }

    double t2 = 0.0, t1 = 0.0, dTOF = 0.0;
    std::map<std::string,double>::iterator itt;
	
    for(itt = hitTime.begin(); itt != hitTime.end(); itt++){
      avNum = GetAVNumber(itt->first);
      if((avNum == 1 || avNum == 2 || avNum == 5 || avNum == 6) 
		 && t2 == 0.0)
		t2 = itt->second;
      if((avNum == 9 || avNum == 10) && t1 == 0.0)
		t1 = itt->second;
    }
    dTOF = (t2-t1);
    if(dTOF != 0.0) delta_TOF->Fill(dTOF);   

    std::map<std::string,double>::iterator it;
    for(it = eDep.begin(); it != eDep.end(); it++) {
      if(histograms.find(it->first) == histograms.end()) {
		avNum = GetAVNumber(it->first);
		if((avNum == 1 || avNum == 2 || avNum == 5 || avNum == 6 || 
			avNum == 9 || avNum == 10)){
		  histograms[it->first] = 
			new TH1F((it->first).c_str(),(it->first).c_str(),1000,0,200);
		}else if((avNum == 3 || avNum == 4 || avNum == 7 || avNum == 8 || 
				  avNum == 11 || avNum == 12)){
		  histograms[it->first] = 
			new TH1F((it->first).c_str(),(it->first).c_str(),200,0,20);
		}else if(avNum == 0){
		  histograms[it->first] = 
			new TH1F((it->first).c_str(),(it->first).c_str(),1000,0,200);
		}
      }
      if(it->second != 0.0) (histograms[it->first])->Fill(it->second);
    } 
    eDep.clear();	
	hitTime.clear();
  }
  
  // Write it all out to the ROOT file.
  
  TVectorD totalElectrons(2);
  totalElectrons[0] = TotalElectrons;
  totalElectrons[1] = TotalEventsRecorded;
  totalElectrons.Write();
  std::map<std::string,TH1F *>::iterator it2;
  for(it2 = targetParticleKE.begin(); it2 != targetParticleKE.end(); it2++) {
    it2->second->Write();
  }
  
  std::map<std::string,TH2F *>::iterator it3;
  for(it3 = targetParticlePOS.begin(); it3 != targetParticlePOS.end(); it3++) {
    it3->second->Write();
  }
  
  std::map<std::string,TH1F *>::iterator it4;
  for(it4 = npolParticleKE.begin(); it4 != npolParticleKE.end(); it4++) {
    it4->second->Write();
  }

  std::map<std::string,TH2F *>::iterator it5;
  for(it5 = npolParticlePOS.begin(); it5 != npolParticlePOS.end(); it5++) {
    it5->second->Write();
  }

  std::map<std::string,TH1F *>::iterator it6;
  for(it6 = correlateKE.begin(); it6 != correlateKE.end(); it6++) {
    it6->second->Write();
  }

  std::map<std::string,TH2F *>::iterator it7;
  for(it7 = correlatePOS.begin(); it7 != correlatePOS.end(); it7++) {
    it7->second->Write();
  }

  std::map<std::string, TH1 *>::iterator it8;
  for(it8 = histograms.begin(); it8 != histograms.end(); it8++) {
    it8->second->Write();
  }

  delta_TOF->Write();

  std::cout << "Total electrons: " << TotalElectrons/1e6 << 
	" Million"  << std::endl;
  
  delete outFile;
  delete bins;
}

void PrintEventNumber(int nentries, int i){
  
  if(nentries > 10000){
	if(i%10000 == 0)
	  std::cout << "Processing event no. " << i << std::endl;
  }else if(nentries > 1000){
	if(i%1000 == 0)
	  std::cout << "Processing event no. " << i << std::endl;
  }else if (nentries > 100){
	if(i%100 == 0)
	  std::cout << "Processing event no. " << i << std::endl;
  }else {
	if(i%10 == 0)
	  std::cout << "Processing event no. " << i << std::endl;
  }
}

int GetAVNumber(const std::string &volName) {
  if(volName.substr(0,3) == "av_") {
    int underscoreLocation = volName.find_first_of("_",3);
    return atoi(volName.substr(3,underscoreLocation-3).c_str());
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

// Return a (dynamically allocated, remember to delete!) array of doubles 
// that contain the boundaries of the bins that make the histogram suitable 
// for a log-log plot
double *AntilogBins(const int nbins, const double xmin, const double xmax) {
  double logxmin = TMath::Log10(xmin);
  double logxmax = TMath::Log10(xmax);
  double binwidth = (logxmax - logxmin)/nbins;
  double *new_bins = new double[nbins+1];
  
  for(int i = 0; i <= nbins; i++){
    new_bins[i] = TMath::Power(10,logxmin + i*binwidth);
  }
  
  return new_bins;
}


TString FormInputFile(TString InputDir){
  
  TString fileName = InputDir + "/" + BaseName + "_Lead" + Lead + "cm_" + Energy + "GeV_" + Bfield + "Bdl_" + JobNum + ".root";
  
  return fileName;
}

TString FormOutputFile(TString OutputDir){
  
  TString fileName =  OutputDir + "/" + BaseName + "_Lead" + Lead + "cm_" + Energy + "GeV_" + Bfield + "Bdl_Histos_" + JobNum + ".root";
  
  return fileName;
}

void RetrieveENVvariables() {

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
  
  if(getenv("OutputDir")){
	OutputDir = getenv("OutputDir");
  }else{
	std::cout << "Output Directory environmental varilable not set" << std::endl;
	return;
  }

  if(getenv("InputDir")){
	InputDir = getenv("InputDir");
  }else{
	std::cout << "Input Directory environmental varilable not set" << std::endl;
	return;
  }
}

