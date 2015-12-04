
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <map>

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

#include "NpolVertex.hh"
#include "NpolTagger.hh"
#include "NpolStatistics.hh"
#include "NpolStep.hh"

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
// for a loglog plot
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

void ProcessElectrons() {
  gSystem->Load("NpolClass.so"); 
  
  // Set up the TTrees and their branch addresses
  TChain *npolTree = new TChain("T");
  TChain *statsTree = new TChain("T2");

  npolTree->Add("/data2/cgen/NMUSimData/4.4GeV/4Bdl/Lead10cm/npolLead10cm_4-4GeV_*.root");
  statsTree->Add("/data2/cgen/NMUSimData/4.4GeV/4Bdl/Lead10cm/npolLead10cm_4-4GeV_*.root");

  //npolTree->Add("/data2/cgen/JlabSimData/MagField_4Bdl/Lead10cm/npolLeadOn10cmHighMag_1_*.root");
  //statsTree->Add("/data2/cgen/JlabSimData/MagField_4Bdl/Lead10cm/npolLeadOn10cmHighMag_1_*.root");

  std::vector<NpolVertex *> *anEntry = NULL;
  std::vector<NpolTagger *> *npolEntry = NULL;
  std::vector<NpolTagger *> *shmsEntry = NULL;
  std::vector<NpolStatistics *> *anStat = NULL;
  std::vector<NpolStep *> *anStep = NULL;

  npolTree->SetBranchAddress("NPOL_Tagger",&npolEntry);
  npolTree->SetBranchAddress("SHMS_Tagger",&shmsEntry);
  npolTree->SetBranchAddress("tracks",&anEntry);
  npolTree->SetBranchAddress("Event_Steps",&anStep);
  statsTree->SetBranchAddress("stats",&anStat);

  // Count the total number of electrons on target and total events 
  // saved to the files 
  Long_t TotalElectrons = 0;
  Long_t TotalEventsRecorded = 0;
  cout << "Number of enteries " << statsTree->GetEntries() <<endl;
  for(int i = 0; i < statsTree->GetEntries(); i++){ 
    statsTree->GetEntry(i); 
   
    cout << "Got to this point" << endl;
    TotalElectrons += ((*anStat)[0])->totalEvents;
    TotalEventsRecorded += ((*anStat)[0])->eventsSaved;   
  }
  
  std::cout << "Total electrons: " << TotalElectrons << std::endl;
  
  // Scale to (microAmp^-1)(cm^-2) 
  Double_t fluxScaling = 1/((Double_t)TotalElectrons*1.609e-13*98*60);
    
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
  
  const int nbins = 500;
  double *bins = AntilogBins(nbins,1e-1,1e4);
  std::map<std::string,TH1 *> histograms;
  std::map<std::string,TH1F *> taggedParticleKE;
  std::map<std::string,TH2F *> taggedParticlePOS;

  // Allocate KE histograms and Position Histograms
  std::map<std::string,std::string>::iterator it;
  for(it = fancyNames.begin(); it != fancyNames.end(); it++) {
    std::string histoName = "h_" + it->first; 
    std::string histoTitle = it->second + " KE in Tagger"; 
    std::string posHistoName = "pos_" + it->first;
    std::string posHistoTitle = it->second + " Position in Tagger";
    taggedParticleKE[it->first] = new TH1F(
     histoName.c_str(), histoTitle.c_str(),nbins,bins);
    taggedParticlePOS[it->first] = new TH2F(posHistoName.c_str(),
     posHistoTitle.c_str(),120,-380,-260,160,-40,40);     
  }  

  // Allocate the dTOF histogram
  TH1F *delta_TOF = new TH1F("dToF", "Time of flight between Analyzer and Top/Bottom Detectors", 500, -15.0, 200.0);
  delta_TOF->GetYaxis()->SetTitle("Events");
  delta_TOF->GetXaxis()->SetTitle("Analyzer to Top/Bottom Array Time-of-Flight (ns)");

  // loop over all entries (one per event)
  Int_t nentries = npolTree->GetEntries();
  //for(int i = 0; i < 10000; i++) {
  for(int i = 0; i < nentries; i++) {
    npolTree->GetEntry(i);
    
    if(nentries > 1000){
      if(i%1000 == 0)
	std::cout << "Processing event no. " << i << std::endl;
    }else if(nentries >100){
      if(i%100 == 0)
	std::cout << "Processing event no. " << i << std::endl;
    }else{
      if(i%1 == 0)
	std::cout << "Processing event no. " << i << std::endl;
    }
    
    // loop over all tagged particles (one per step in tagger volume)
    int nTaggedParticles = npolEntry->size();
    for (Int_t j = 0; j < nTaggedParticles; j++){
      NpolTagger *aVertex = (*npolEntry)[j];
      if(aVertex == NULL)
	continue;
      std::string particleName = aVertex->particle;
      if(taggedParticleKE.find(particleName) == taggedParticleKE.end())
	continue;

      (taggedParticleKE[particleName])->Fill(aVertex->energy,fluxScaling);
      (taggedParticlePOS[particleName])->Fill(aVertex->posX,aVertex->posY,fluxScaling);
    }


    // This section is designed to file up the histograms for hits in
    // the Scintillator detectors
    std::map<std::string, double> eDep;
    std::map<std::string, double> hitTime;

    // loop over vector elements (one per vertex)
    //int p = 1;
    //int q = anEntry->size();
    //QSort(anEntry,p,q); // sort vertex vector in time!
    int avNum, imprNum, pvNum;
    
    Int_t nvertices = anStep->size();
    for(int j = 0; j < nvertices; j++) {
      NpolStep *aStep = (*anStep)[j];
      double Ethreshold = 0.0;
      
      if(aStep == NULL) continue;
            
      if(eDep.find(aStep->volume) == eDep.end())
	eDep[aStep->volume] = 0;
      eDep[aStep->volume] += aStep->eDep;
      
      avNum = GetAVNumber(aStep->volume);
      switch(avNum){
      case 1: case 2: case 5: case 6:
	Ethreshold = 5.0;
	break;
      case 3: case 4: case 7: case 8:
	Ethreshold = 1.0;
	break;
      case 9: case 10:
	Ethreshold = 4.0;
	break;
      default:
	break;
      }
      
      if(avNum != 0){
	if(hitTime.find(aStep->volume) == hitTime.end() && eDep[aStep->volume] >= Ethreshold)
	  hitTime[aStep->volume] = aStep->time;
      }
    }

    double t2 = 0.0, t1 = 0.0, dTOF = 0.0;
    std::map<std::string,double>::iterator itt;
    //if(hitTime.size() > 2) cout << "My Map size is: " << hitTime.size() << endl;
    for(itt = hitTime.begin(); itt != hitTime.end(); itt++){
      //if(hitTime.size() > 2) cout << " Volume: " << itt->first << " Time: " << itt->second << endl;
      avNum = GetAVNumber(itt->first);
      if((avNum == 1 || avNum == 2 || avNum == 5 || avNum == 6) && t2 == 0.0)
	t2 = itt->second;
      if((avNum == 9 || avNum == 10) && t1 == 0.0)
	t1 = itt->second;
    }
    dTOF = (t2-t1);
    if(dTOF != 0.0) delta_TOF->Fill(dTOF);
    
    
    std::map<std::string,double>::iterator it;
    for(it = eDep.begin(); it != eDep.end(); it++) {
      if(histograms.find(it->first) == histograms.end()) {
	histograms[it->first] = new TH1F((it->first).c_str(),(it->first).c_str(),500,0,200);
      }
      (histograms[it->first])->Fill(it->second);
    }
    
    eDep.clear();
		
  }
 
  TFile *outFile = new TFile("NMU4-4GeV_Lead10cm_4Bdl_Histos.root","RECREATE");  
  //TFile *outFile = new TFile("JLABLead10cm_4Bdl_Histos.root","RECREATE");
  TVectorD totalElectrons(1);
  totalElectrons[0] = TotalElectrons;
  totalElectrons.Write();
  std::map<std::string,TH1F *>::iterator it2;
  for(it2 = taggedParticleKE.begin(); it2 != taggedParticleKE.end(); it2++) {
    it2->second->Write();
  }

  std::map<std::string,TH2F *>::iterator it3;
  for(it3 = taggedParticlePOS.begin(); it3 != taggedParticlePOS.end(); it3++) {
    it3->second->Write();
  }

  std::map<std::string, TH1 *>::iterator it4;
  for(it4 = histograms.begin(); it4 != histograms.end(); it4++) {
    it4->second->Write();
  }

  delta_TOF->Write();

  std::cout << "Total electrons: " << TotalElectrons << std::endl;
  delete outFile;
  
  delete bins;
}

