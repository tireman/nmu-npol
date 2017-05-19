
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
#include <TH3.h>
#include <TSystem.h>
#include <TROOT.h>
#include <TObject.h>
#include <TVectorD.h>
#include <TString.h>

#include "../../npollib/include/NpolVertex.hh"
#include "../../npollib/include/NpolTagger.hh"
#include "../../npollib/include/NpolStatistics.hh"
#include "../../npollib/include/NpolStep.hh"

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

int main(int argc, char *argv[]) {
  TString analysisDir = getenv("NPOLLIB_DIR");
  gSystem->Load(analysisDir + "/" + "libNpolClasses.so"); 
  
  // Set up the TTrees and their branch addresses
  TChain *npolTree = new TChain("T");
  TChain *statsTree = new TChain("T2");

  Double_t NpolAng = 0.488692; // radians (28 degrees) NPOL angle from beamline
  Double_t targetTaggerPos = 150.0;  // Position of target tagger (cm)
  Double_t npolTaggerPos = 681.50;  // Position of NPOL Tagger (cm) 681.50 for 7 meter
  Double_t theta = 0.160; // NPOL horizontal angular accecptance (radians)
  Double_t phi = 0.100; // NPOL vertical angular acceptance (radians)
  
  // ******* Tagger acceptance computations.  ALL SIZES ARE FULL WIDTH/HEIGHT ******** //
  // Compute or Manually set the size of the histograms and cuts for filling histograms (X,Y) position
  // Must be done carefully to assure you are extracting the desired area of the tagger in question
  // This may be a different acceptance for both NPOL and Target Taggers

  // This version uses the fixed angular acceptance for the sizing of the collimator
  //Double_t targetW = 2*targetTaggerPos*TMath::Tan(theta/2);  // width of target tagger (cm)
  //Double_t targetH = 2*targetTaggerPos*TMath::Tan(phi/2);   // height of target tagger (cm)
  //Double_t npolW = 2*npolTaggerPos*TMath::Tan(theta/2);  // width of npol tagger (cm)
  //Double_t npolH = 2*npolTaggerPos*TMath::Tan(phi/2);  // height of npol tagger (cm)

  // This version allows for fixing the size manually
  Double_t targetW = 70.00;  // width of target tagger (cm) Dipole 1 opening 56.0 cm
  Double_t targetH = 26.00;  // height of target tagger (cm) Dipole 1 opeing 20.955 cm
  Double_t npolW = 2*npolTaggerPos*TMath::Tan(theta/2);  // width of npol tagger (cm)
  Double_t npolH = 2*npolTaggerPos*TMath::Tan(phi/2);  // height of npol tagger (cm)
  
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
  std::map<std::string,TH1F *> targetTheta;
  std::map<std::string,TH1F *> targetPhi;
  std::map<std::string,TH1F *> npolTheta;
  std::map<std::string,TH1F *> npolPhi;
  std::map<std::string,TH3F *> particleOrigin;

  // Allocate Theta, Phi, KE and Position Histograms
  std::map<std::string,std::string>::iterator it;
  for(it = fancyNames.begin(); it != fancyNames.end(); it++) {
    std::string targetHistoName = "TargetFlux_" + it->first; 
    std::string targetHistoTitle = it->second + " Flux vs. KE in Target Tagger"; 
    std::string npolHistoName = "NpolFlux_" + it->first; 
    std::string npolHistoTitle = it->second + " Flux vs. KE in NPOL Tagger";
    std::string correlateHistoName = "Correlated_TargetFlux_" + it->first;
    std::string correlateHistoTitle = it->second + " Flux vs. KE Target Tagger Correlated to NPOL Tagger";

   	std::string targetThetaName = "targetTheta_" + it->first;
	std::string targetThetaTitle = it->second + " Particles vs. Theta Angle in Target Tagger";
	std::string targetPhiName = "targetPhi_" + it->first;
	std::string targetPhiTitle = it->second + " Particles vs. Phi Angle in Target Tagger";

	std::string npolThetaName = "npolTheta_" + it->first;
	std::string npolThetaTitle = it->second + " Particles vs. Theta Angle in Npol Tagger";
	std::string npolPhiName = "npolPhi_" + it->first;
	std::string npolPhiTitle = it->second + " Particles vs. Phi Angle in Npol Tagger";

    std::string targetXYHistoName = "targetXY_" + it->first;
    std::string npolXYHistoName = "npolXY_" + it->first;
    std::string targetXYHistoTitle = it->second + " XY Position in Target Tagger";
    std::string npolXYHistoTitle = it->second + " XY Position in NPOL Tagger";
    std::string correlateXYHistoName = "Correlated_targetXY_" + it->first;
    std::string correlateXYTitle = it->second + " XY position in Target Tagger Correlated to NPOL Tagger";

	std::string origin3DHistoTitle = it->second + " 3D position of Vertex in System from NPOL Tagger hit";
	std::string origin3DHistoName = "Origin3D_Position_" + it->first;

	particleOrigin[it->first] = 
	  new TH3F(origin3DHistoName.c_str(), origin3DHistoTitle.c_str(),200,-100.,+700.,200,-700., +100.,200,-300., +300.);

    targetParticleKE[it->first] = 
	  new TH1F(targetHistoName.c_str(), targetHistoTitle.c_str(),nbins,bins);
    targetParticlePOS[it->first] = 
	  new TH2F(targetXYHistoName.c_str(),targetXYHistoTitle.c_str()
			   ,400,-targetW/2,targetW/2,400,-targetH/2,targetH/2);

    npolParticleKE[it->first] = 
	  new TH1F(npolHistoName.c_str(), npolHistoTitle.c_str(),nbins,bins);
    npolParticlePOS[it->first] = 
	  new TH2F(npolXYHistoName.c_str(),npolXYHistoTitle.c_str()
			   ,400,-npolW/2,npolW/2, 400, -npolH/2,npolH/2);

    correlateKE[it->first] = 
	  new TH1F(correlateHistoName.c_str(), correlateHistoTitle.c_str(),nbins,bins);
    correlatePOS[it->first] = 
	  new TH2F(correlateXYHistoName.c_str(),correlateXYTitle.c_str()
			   ,400,-npolW/2,npolW/2,400,-npolH/2,npolH/2); 

	targetTheta[it->first] = new TH1F(targetThetaName.c_str(),targetThetaTitle.c_str(),300,0.0,TMath::Pi());
	targetPhi[it->first] = new TH1F(targetPhiName.c_str(),targetPhiTitle.c_str(),600,0.0,2*TMath::Pi());
	npolTheta[it->first] = new TH1F(npolThetaName.c_str(),npolThetaTitle.c_str(),300,0.0,TMath::Pi());
	npolPhi[it->first] = new TH1F(npolPhiName.c_str(),npolPhiTitle.c_str(),600,0.0,2*TMath::Pi());
  }  

  // Allocate the dTOF histogram
  TH1F *delta_TOF = 
    new TH1F("dToF", "Time of flight between Analyzer and Top/Bottom Detectors",500, -15.0, 200.0);
  delta_TOF->GetYaxis()->SetTitle("Events");
  delta_TOF->GetXaxis()->SetTitle("Analyzer to Top/Bottom Array Time-of-Flight (ns)");


  // ****************  Done with Hisotgram allocation ***********************
  std::set<int> npolTrackIDs;
  std::set<int> vertexTrackIDs;
  // loop over all entries (one per event)
  Int_t nentries = npolTree->GetEntries();
  for(int i = 0; i < nentries; i++) {
    npolTree->GetEntry(i);
	PrintEventNumber(nentries,i);
	
	// ****** This section fills a std::set with the trackIDs of particles ****
	// ****** generated in the specified volume(s).  This 'set' is then    ****
	// ****** used to cut events later.
	/*std::vector<NpolVertex *>::iterator v_it2;
	for(v_it2 = vertexEntry->begin(); v_it2 != vertexEntry->end(); v_it2++){
	  NpolVertex *aVertex = *v_it2;
	  if(aVertex == NULL) continue;
	  
	  std::string volName = aVertex->volume;
	  if(!(volName == "LeadCurtain")) vertexTrackIDs.insert(aVertex->trackId);
	   
	  }*/
	
    // loop over all tagged particles (min. one step in NPOL tagger volume)
	Double_t npolxMax = npolW/2; // acceptance limit
	Double_t npolyMax = npolH/2; // acceptance limit
    std::vector<NpolTagger *>::iterator n_it;	
    for(n_it = npolEntry->begin(); n_it != npolEntry->end(); n_it++){
      NpolTagger *npolTagged = *n_it;
      if(npolTagged == NULL)  continue;
	  
	  // Is it 1 of 9 particles we want?
	  std::string particleName = npolTagged->particle;
	  if(npolParticleKE.find(particleName) == npolParticleKE.end()) continue;
	  
	  // ******* Cut out neutral, charged or keep all particles ******* //
	  // Reject if charged(use !) reject if not charged (remove !) 
	  //or comment out if you want all included
	  int pType = npolTagged->particleId;
	  if((pType == 2112 || pType == 22)) continue; 

	  //******** Cut out particles generated in a chosen volume ******* //
	  if(vertexTrackIDs.find(npolTagged->trackId) != vertexTrackIDs.end()) continue;
	  
	  // Insert Track ID into a set so we can cut later
	  if(npolTrackIDs.find(npolTagged->trackId) == npolTrackIDs.end())
		npolTrackIDs.insert(npolTagged->trackId);
	  		
	  Double_t fluxscaling = 1;	
	  if((abs(npolTagged->lPosX) <= npolxMax) && (abs(npolTagged->lPosY) <= npolyMax)){
		
		// Calculating the theta and phi angles and saving to histograms
		Double_t momx = npolTagged->momX*TMath::Cos(NpolAng) + npolTagged->momZ*TMath::Sin(NpolAng);
		Double_t momy = npolTagged->momY;
		Double_t momz = (-1)*npolTagged->momX*TMath::Sin(NpolAng) + npolTagged->momZ*TMath::Cos(NpolAng);
		Double_t momTotal = TMath::Sqrt(TMath::Power(momx,2)+TMath::Power(momy,2)+TMath::Power(momz,2));
		Double_t theta = TMath::ACos(momz/momTotal);		
		Double_t phi=0;
		if(momy >= 0) phi = TMath::ACos(momx/(momTotal*TMath::Sin(theta)));
		if(momy < 0) phi = 2*TMath::Pi()-TMath::ACos(momx/(momTotal*TMath::Sin(theta)));
		if(theta > TMath::Pi()/2) continue;
		(npolParticleKE[particleName])->
		  Fill(npolTagged->energy,fluxscaling);
		(npolParticlePOS[particleName])->
		  Fill(npolTagged->lPosX,npolTagged->lPosY);
		(npolTheta[particleName])->Fill(theta);
		(npolPhi[particleName])->Fill(phi);
	  }
	}
	
	// ************ Fill the 3D plot ***********************
	std::vector<NpolVertex *>::iterator v_it;
	for(v_it = vertexEntry->begin(); v_it != vertexEntry->end(); v_it++){
	  NpolVertex *aVertex = *v_it;
	  if(aVertex == NULL) continue;
	  
	  if(npolTrackIDs.find(aVertex->trackId) != npolTrackIDs.end()){ 
		std::string particleName = aVertex->particle;
		(particleOrigin[particleName])->Fill(aVertex->posZ,aVertex->
											 posX,aVertex->posY);
	  }
	}
	
	
    // loop over all tagged particles (min. one step in target tagger volume)
	Double_t targetxMax = targetW/2; // acceptance limit 
	Double_t targetyMax = targetH/2; // acceptance limit
    std::vector<NpolTagger *>::iterator t_it;
    for(t_it = targetEntry->begin(); t_it != targetEntry->end(); t_it++){
      NpolTagger *targetTagged = *t_it;
      if(targetTagged == NULL) continue;  
      
	  std::string particleName = targetTagged->particle;
      if(targetParticleKE.find(particleName) == targetParticleKE.end())
		continue;
	  Double_t fluxscaling = 1;
	  if((abs(targetTagged->lPosX) <= targetxMax) && (abs(targetTagged->lPosY) <= targetyMax)){
		
		// Calculating the theta and phi angles and saving to histograms
		Double_t momx = targetTagged->momX*TMath::Cos(NpolAng) + targetTagged->momZ*TMath::Sin(NpolAng);
		Double_t momy = targetTagged->momY;
		Double_t momz = (-1)*targetTagged->momX*TMath::Sin(NpolAng) + targetTagged->momZ*TMath::Cos(NpolAng);
		Double_t momTotal = TMath::Sqrt(TMath::Power(momx,2)+TMath::Power(momy,2)+TMath::Power(momz,2));
		Double_t theta = TMath::ACos(momz/momTotal);
		Double_t phi=0;
		if(momy >= 0) phi = TMath::ACos(momx/(momTotal*TMath::Sin(theta)));
		if(momy < 0) phi = 2*TMath::Pi()-TMath::ACos(momx/(momTotal*TMath::Sin(theta)));
		if(theta > TMath::Pi()/2) continue;
		(targetParticleKE[particleName])->
		  Fill(targetTagged->energy,fluxscaling);
		(targetParticlePOS[particleName])->
		  Fill(targetTagged->lPosX,targetTagged->lPosY);
		(targetTheta[particleName])->Fill(theta);
		(targetPhi[particleName])->Fill(phi);
		
		// Here the correlated histograms are to be filled
		if(npolTrackIDs.find(targetTagged->trackId) 
		   != npolTrackIDs.end()){  
		  
		  (correlateKE[particleName])->
			Fill(targetTagged->energy,fluxscaling);
		  (correlatePOS[particleName])->
			Fill(targetTagged->lPosX,targetTagged->lPosY);
		}
	  }
	}
	
    // This section is designed to file up the histograms for hits in
    // the Scintillator detectors
	std::map<std::string, double> eDep;
	std::map<std::string, double> hitTime;
	
	int avNum; //, imprNum, pvNum; 
	//int stepCount = 0;
    std::vector<NpolStep *>::iterator s_it;
    for(s_it = anStep->begin(); s_it != anStep->end(); s_it++){
      NpolStep *aStep = *s_it;
      double Ethreshold;
	  
      if(aStep == NULL) continue;
	  
	  if((npolTrackIDs.find(aStep->trackId) != npolTrackIDs.end()) 
		 || (npolTrackIDs.find(aStep->parentId) != npolTrackIDs.end())){
		
		npolTrackIDs.insert(aStep->trackId);
		
	  } else {
		std::cout << "Rejected Step!" << std::endl;
		continue;
	  }
	  
	  if(eDep.find(aStep->volume) == eDep.end())
		eDep[aStep->volume] = 0;
	  eDep[aStep->volume] += aStep->eDep;
	  
	  avNum = GetAVNumber(aStep->volume);
	  switch(avNum){
	  case 1: case 2: case 5: case 6:
		Ethreshold = 1.0;
		break;
	  case 3: case 4: case 7: case 8: case 13:
		Ethreshold = 1.0;
		break;
	  case 9: case 10:
		Ethreshold = 1.0;
		break;
	  default:
		break;
	  }
	  
	  if(avNum != 0) {
		if(hitTime.find(aStep->volume) == hitTime.end() 
		   && eDep[aStep->volume] >= Ethreshold)
		  hitTime[aStep->volume] = aStep->time;
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
					avNum == 11 || avNum == 12 || avNum == 13)){
			histograms[it->first] = 
			  new TH1F((it->first).c_str(),(it->first).c_str(),200,0,20);
		  }else if(avNum == 0){
			histograms[it->first] = 
			  new TH1F((it->first).c_str(),(it->first).c_str(),1000,0,200);
		  }
		}
		if(it->second != 0.0) (histograms[it->first])->Fill(it->second);
	  }
	  eDep.clear();	  // Clear the maps BEFORE cycling the steps loop
	  hitTime.clear();  // This MUST be done before the next } - brace
	}
	npolTrackIDs.clear(); // clear the sets before cycling the event loop
	vertexTrackIDs.clear(); 
  }
  
  /* // Write it all out to the ROOT file.*/
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

  std::map<std::string,TH1F *>::iterator it9;
  for(it9 = targetTheta.begin(); it9 != targetTheta.end(); it9++){
	it9->second->Write();
  }

  std::map<std::string,TH1F *>::iterator it10;
  for(it10 = targetPhi.begin(); it10 != targetPhi.end(); it10++){
	it10->second->Write();
  }

  std::map<std::string,TH1F *>::iterator it11;
  for(it11 = npolTheta.begin(); it11 != npolTheta.end(); it11++){
	it11->second->Write();
  }

  std::map<std::string,TH1F *>::iterator it12;
  for(it12 = npolPhi.begin(); it12 != npolPhi.end(); it12++){
	it12->second->Write();
  }

  std::map<std::string,TH3F *>::iterator it13;
  for(it13 = particleOrigin.begin(); it13 != particleOrigin.end(); it13++){
	it13->second->Write();
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
  
  TString fileName = InputDir + "/" + BaseName + "_" + JobNum + ".root";
  
  return fileName;
}

TString FormOutputFile(TString OutputDir){
  
  TString fileName =  OutputDir + "/" + BaseName + "_Histos_" + JobNum + ".root";
  
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

//if((volName == "LeadCurtain"))
//if((volName == "HutFrontWall")) 
//if((volName == "Dipole1" || volName == "Dipole2" || volName == "Dipole1CuBar" || volName == "Dipole2CuBar"))

//Double_t targetAlpha = targetW/(4*targetTaggerPos);  // Constant needed for solid angle
//Double_t npolAlpha = npolW/(4*npolTaggerPos);  // Constant needed for solid angle

// Solid angle calculation! 
//Double_t targetSolidAngle = 8*(TMath::ATan(targetH/targetW)-TMath::ASin(targetH/TMath::Sqrt((1+TMath::Power(targetAlpha,2))*(TMath::Power(targetH,2)+TMath::Power(targetW,2)))));
//Double_t npolSolidAngle = 8*(TMath::ATan(npolH/npolW)-TMath::ASin(npolH/TMath::Sqrt((1+TMath::Power(npolAlpha,2))*(TMath::Power(npolH,2)+TMath::Power(npolW,2)))));

//npolTree->SetCacheSize(50000000);
//statsTree->SetCacheSize(50000000);
