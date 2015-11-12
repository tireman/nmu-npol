
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
#include <TSystem.h>
#include <TROOT.h>
#include <TObject.h>
#include <TVectorD.h>

#include "NpolVertex.hh"
#include "NpolTagger.hh"
#include "NpolStatistics.hh"

// Return a (dynamically allocated, remember to delete!) array of doubles that contain the boundaries
// of the bins that make the histogram suitable for a loglog plot
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

// Generate tagged particle KE  histograms with variable width bins suitable for a loglog plot
void TaggedParticles3() {
  gSystem->Load("NpolClass.so"); 
  
  // Set up the TTrees and their branch addresses
  TChain *npolTree = new TChain("T");
  TChain *statsTree = new TChain("T2");
  
  //npolTree->Add("/data2/cgen/NMUSimData/Lead10cm/npol*.root");
  //statsTree->Add("/data2/cgen/NMUSimData/Lead10cm/npol*.root");
  npolTree->Add("/data2/cgen/JlabSimData/MagField_2Bdl/Lead10cm/npolLeadOn10cmMedMag_*.root");
  statsTree->Add("/data2/cgen/JlabSimData/MagField_2Bdl/Lead10cm/npolLeadOn10cmMedMag_*.root");
  
  std::vector<NpolVertex *> *anEntry = NULL;
  std::vector<NpolTagger *> *npolEntry = NULL;
  std::vector<NpolTagger *> *shmsEntry = NULL;
  NpolStatistics *aStat = NULL;
  
  npolTree->SetBranchAddress("NPOL_Tagger",&npolEntry);
  npolTree->SetBranchAddress("SHMS_Tagger",&shmsEntry);
  npolTree->SetBranchAddress("tracks",&anEntry);
  statsTree->SetBranchAddress("stats",&aStat);
  
  // Count the total number of electrons on target and total events saved to the files
  Long_t TotalElectrons = 0;
  Long_t TotalEventsRecorded = 0;
  for(int i = 0; i < statsTree->GetEntries(); i++){
    statsTree->GetEntry(i);
    TotalElectrons += aStat->totalEvents;
    TotalEventsRecorded += aStat->eventsSaved;
  }
  
  std::cout << "Total electrons: " << TotalElectrons << std::endl;
  
  // Scale to (electrons^-1)(cm^-2) = (electrons on target)^-1 * (area of NPOL collimator)^-1
  Double_t fluxScaling = 1/((Double_t)TotalElectrons*98*60);
    
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
  std::map<std::string,TH1F *> taggedParticleKE;
  
  // Allocate KE histograms
  std::map<std::string,std::string>::iterator it;
  for(it = fancyNames.begin(); it != fancyNames.end(); it++) {
    std::string histoName = "h_" + it->first; 
    std::string histoTitle = it->second + " KE in Tagger";
    taggedParticleKE[it->first] = new TH1F(
					   histoName.c_str(), histoTitle.c_str(),nbins,bins);
  }
  
  // loop over all entries (one per event)
  Int_t nentries = npolTree->GetEntries();
  for(int i = 0; i < nentries; i++) {
    //	for(int i = 0; i < 10000; i++) {
    npolTree->GetEntry(i);
    
    if(nentries > 1000){
      if(i%1000 == 0)
	std::cout << "Processing event no. " << i << std::endl;
    }else{
      if(i%100 == 0)
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
    }
  }
  
  TFile *outFile = new TFile("JLABLead10cm_2Bdl_Histos.root","RECREATE");
  TVectorD totalElectrons(1);
  totalElectrons[0] = TotalElectrons;
  totalElectrons.Write();
  std::map<std::string,TH1F *>::iterator it2;
  for(it2 = taggedParticleKE.begin(); it2 != taggedParticleKE.end(); it2++) {
    
    //fluxScaling = (it2->second->Integral())/(TotalElectrons*100*60);
    //fluxScaling = 1/(TotalElectrons*100*60);
    //fluxScaling = (it2->second->Integral())/1e6;
    //it2->second->Scale(fluxScaling);
    
    it2->second->Write();
  }
  delete outFile;
  
  delete bins;
}

