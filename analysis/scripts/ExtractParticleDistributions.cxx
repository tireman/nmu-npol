
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>

#include <TLatex.h>
#include <TAttAxis.h>
#include <TMath.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TVector.h>
#include <TH1.h>
#include <TSystem.h>
#include <TROOT.h>
#include <TObject.h>
#include <TString.h>

TString FormInputFile(TString InputDir);
TString FormOutputFile(TString OutputDir);
void RetrieveENVvariables();


TString BaseName = "";  TString JobNum = "";  TString Lead = ""; TString Energy = ""; 
TString Bfield = ""; TString OutputDir = ""; TString InputDir = "";

// main function
void ExtractParticleDistributions() {

  std::string histoNames[3][3]={{"pi-","mu+","gamma"},
								{"neutron","mu-","e+"},
								{"proton","pi+","e-"}};
  std::string leadName[1][8] = {{"NpolFlux","TargetFlux","targetTheta","targetPhi","npolTheta","npolPhi","npolXY","targetXY"}};
  
  RetrieveENVvariables();
  
  TString InputFile = FormInputFile(InputDir);
  TFile *inFile = TFile::Open(InputFile);
  
  // Algorthm to load the histograms and generate the *.dat files from the 
  // histogram data.
  //std::string leadName = "NpolFlux";
  Int_t Nx = 3;  Int_t Ny = 3; Int_t Nk = 8;
  for(int k = 0; k < Nk; k++){
	for(int i = 0; i < Nx; i++){
	  for(int j = 0; j < Ny; j++){
		char hname[32];
		sprintf(hname,"%s_%s",leadName[0][k].c_str(),
				histoNames[i][j].c_str());
		TH1F *hFrame = (TH1F*) inFile->Get(hname);
		// Output the bin contents for Simulation
		if(k < 6){
		  std::ofstream txtOut; std::string fileName;
		  if(k == 0) fileName = "npolEnergy_" + histoNames[i][j] + ".dat";
		  if(k == 1) fileName = "targetEnergy_" + histoNames[i][j] + ".dat";
		  if(k == 2) fileName = "targetTheta_" + histoNames[i][j] + ".dat";
		  if(k == 3) fileName = "targetPhi_" + histoNames[i][j] + ".dat";
		  if(k == 4) fileName = "npolTheta_" + histoNames[i][j] + ".dat";
		  if(k == 5) fileName = "npolPhi_" + histoNames[i][j] + ".dat";
		  txtOut.open(fileName);
		  txtOut << "/gps/hist/point " << "0.0 " << " 0.0" << std::endl;
		  Double_t totalCounts = hFrame->Integral();
		  Double_t binOut = 0.0;
		  for(int i = 1; i <= hFrame->GetNbinsX()-2; i++){
			Double_t oldbincontent = hFrame->GetBinContent(i);
			Double_t binWidth = hFrame->GetBinWidth(i);
			if(k == 0 || k == 1){
			  binOut += binWidth;
			  txtOut << "/gps/hist/point " << binOut << "  " << 
				oldbincontent << std::endl;
			}else if(k == 2 || k == 4){
			  binOut += binWidth;
			  txtOut << "/gps/hist/point " << /*binOut*/ (1-TMath::Cos(binOut))/2 << "  " << 
				oldbincontent << std::endl;
			}else if(k == 3 || k == 5){
			  binOut += binWidth;
			  txtOut << "/gps/hist/point " << binOut/(2*TMath::Pi()) << "  " << 
				oldbincontent << std::endl;
			} 
		  }
		  txtOut.close();
		}else if(k >= 6){
		  for(int l = 0; l < 2; l++){
			std::ofstream txtOut; std::string fileName;
			if(l == 0 && k == 6) fileName = "npolX_" + histoNames[i][j] + ".dat";
			if(l == 1 && k == 6) fileName = "npolY_" + histoNames[i][j] + ".dat";
			if(l == 0 && k == 7) fileName = "targetX_" + histoNames[i][j] + ".dat";
			if(l == 1 && k == 7) fileName = "targetY_" + histoNames[i][j] + ".dat";
			txtOut.open(fileName);
			txtOut << "/gps/hist/point " << "0.0 " << " 0.0" << std::endl;
			Int_t NbinsX = hFrame->GetNbinsX()-2;
			Int_t NbinsY = hFrame->GetNbinsY()-2;
			if(l == 1){
			  NbinsX ^= NbinsY;
			  NbinsY ^= NbinsX;
			  NbinsX ^= NbinsY;
			} 
			for(int i = 1; i <= NbinsX; i++){
			  double oldbincontent = 0.0; double binWidth = 0.0;
			  for(int j = 1; j <= NbinsY; j++){
				if(l != 1) oldbincontent += hFrame->GetBinContent(i,j);
				else oldbincontent += hFrame->GetBinContent(j,i);
			  }
			  binWidth = hFrame->GetXaxis()->GetBinWidth(i);
			  txtOut << "/gps/hist/point " << ((float)i/NbinsX) << "  " << oldbincontent << std::endl; 
			}
			txtOut.close();
		  }
		}
	  }
	}
  }
  inFile->Close();
}


TString FormInputFile(TString InputDir){
  
  TString fileName = InputDir + "/" + BaseName + "_" + Energy + "GeV_" + "Lead" + Lead + "cm_" + Bfield + "Bdl_Histos.root";

  return fileName;
  
}

TString FormOutputFile(TString OutputDir){
  
  TString fileName =  OutputDir + "/" + BaseName + Energy + "GeV_Lead" + Lead + "cm_" + Bfield + "Bdl_Fig21-23.root";
  
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
  
  if(getenv("WorkOutputDir")){
	OutputDir = getenv("WorkOutputDir");
  }else{
	std::cout << "Output Directory environmental varilable not set" << std::endl;
	return;
  }

  if(getenv("WorkInputDir")){
	InputDir = getenv("WorkInputDir");
  }else{
	std::cout << "Input Directory environmental varilable not set" << std::endl;
	return;
  }
}

