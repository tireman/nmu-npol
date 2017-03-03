
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#include <TLatex.h>
#include <TAttAxis.h>
#include <TMath.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TBranch.h>
#include <TVector.h>
#include <TH1.h>
#include <TH2.h>
#include <TSystem.h>
#include <TROOT.h>
#include <TObject.h>
#include <TString.h>

void CanvasPartition(TCanvas *C,const Int_t Nx = 2,const Int_t Ny = 2, 
		     Float_t lMargin = 0.15, Float_t rMargin = 0.05,
                     Float_t bMargin = 0.15, Float_t tMargin = 0.05,
		     Float_t vSpacing = 0.0, Float_t hSpacing = 0.0);

int GetAVNumber(const std::string &volName);
int GetImprNumber(const std::string &volName);
int GetPlacementNumber(const std::string &volName);
TString FormInputFile(TString InputDir);
TString FormOutputFile(TString OutputDir);
void RetrieveENVvariables();
void GetCanvasParameters(int n);

int PVnumMax = 0, AVnum = 1, ImprNumMax = 2;
TString BaseName = "";  TString JobNum = "";  TString Lead = ""; TString Energy = ""; 
TString Bfield = ""; TString OutputDir = ""; TString InputDir = "";
std::string FancyName = ""; std::string RealName = "";

void NpolDetectorCountRates() {

  Long_t TotalElectrons = 0, TotalEventsRecorded = 0; 

  RetrieveENVvariables();
  
  TString InputFile = FormInputFile(InputDir);
  TString OutputFile = FormOutputFile(OutputDir);
  TFile *inFile = TFile::Open(InputFile);
  TFile *outFile = new TFile(OutputFile,"RECREATE");

  // Retrieve the object with the total number of electrons on target and calculate 
  // effective electron time on target per micro amp of beam

  TVectorD *v = (TVectorD*)inFile->Get("TVectorT<double>");
  Double_t totalElectrons = ((*v))[0];
  Double_t electronTime = totalElectrons/(6.242e12); //6.242e12 e-/s at 1 microAmp
  //Double_t fluxscaling = 1/(totalElectrons*1.602e-13*(98*60));
  std::cout << "Electron beam time at 1 micro-amp is " << electronTime << " s " << std::endl;
  std::cout << "Total electrons on target: " << totalElectrons/1e6 << " Million" << std::endl;


  int pvNum = 1, avNum = 1, imprNum = 1;
  Int_t Nx = 16, Ny = 2, nThresh = 10, fillStyle = 1001;
  Float_t lMargin = 0.10, rMargin = 0.05, bMargin = 0.10, tMargin = 0.05;
  Float_t vSpacing = 0.0; Float_t hSpacing = 0.0;
  double CTagger[Nx][Ny];
  double Thresholds[10]={0.5,1.0,1.5,2.0,2.5,3.0,3.5,4.0,4.5,5.0};
  double CountRates [nThresh][Nx][Ny];
  std::ofstream txtOut;
  txtOut.open(OutputDir + "/Output/NpolDetectorCountRates" + Lead + "cm.out");

  TCanvas *C1[22]; TCanvas *C2[22];
  for(int l = 0; l < 22; l++){
	char tempName[12], tempName2[12];
	sprintf(tempName,"canvas%i",l);
	sprintf(tempName2,"graph%i",l);
	C1[l]= new TCanvas(tempName,"Energy Plots at Polarimeter Angle 28.0 Deg, E = 4.4 GeV",1200,900);
	C2[l]= new TCanvas(tempName2,"Count Rate vs. Threshold plots",1200,900);
  }

  TPad *pad1[22][7][2]; TPad *pad2[22][7][2];
  char histoName[60];
  for(int n = 0; n < 22; n++){	
	pvNum = 0;
	GetCanvasParameters(n);
	if(PVnumMax == 13){ Nx = 7; } else { Nx = PVnumMax/2; }
	CanvasPartition(C1[n],Nx,Ny,lMargin,rMargin,bMargin,tMargin,vSpacing,hSpacing);
	CanvasPartition(C2[n],Nx,Ny,lMargin,rMargin,bMargin,tMargin,vSpacing,hSpacing);
	for(int i = 0; i < Nx; i++){
	  for(int j = 0; j < Ny; j++){
		sprintf(histoName,"av_%i_impr_%i_%sLV_pv_%i",AVnum,imprNum,FancyName.c_str(),pvNum);
		std::cout << "Detector name = " << histoName << std::endl;

		// Process the histogram here
		C1[n]->cd(0);
		// Get the pads previosly created.
		char pname1[16];
		sprintf(pname1,"pad_%i_%i",i,j);
		pad1[n][i][j] = (TPad*) gROOT->FindObject(pname1);
		pad1[n][i][j]->Draw();
		pad1[n][i][j]->SetLogy();
		pad1[n][i][j]->SetFillStyle(4000);
		pad1[n][i][j]->SetFrameFillStyle(4000);
		pad1[n][i][j]->cd();
		// Size factors
		Float_t xFactor = pad1[n][0][0]->GetAbsWNDC()/pad1[n][i][j]->GetAbsWNDC();
		Float_t yFactor = pad1[n][0][0]->GetAbsHNDC()/pad1[n][i][j]->GetAbsHNDC();

		TH1F *hFrame = (TH1F*) inFile->Get(histoName);
		hFrame->SetStats(false); 
		hFrame->SetFillColor(kBlue);
		hFrame->SetTitleFont(16);
		hFrame->SetFillStyle(fillStyle);
		hFrame->Draw();
		// Set Good Histogram Title
		avNum = GetAVNumber(histoName);
		imprNum = GetImprNumber(histoName);
		pvNum = GetPlacementNumber(histoName);
		char htitle[80];
		sprintf(htitle,"#splitline{Energy Deposited}{%s %i, Layer %i}",RealName.c_str(),pvNum+1, imprNum);
		hFrame->SetTitle(htitle);     
		// y axis range
		double FirstBinHeight= hFrame->GetBinContent(hFrame->GetMaximumBin());
		hFrame->GetYaxis()->SetRangeUser(0.2,5.0*FirstBinHeight);
		
		// Format for y axis
		hFrame->GetYaxis()->SetTitle("Events");
		hFrame->GetYaxis()->SetLabelFont(43);
		hFrame->GetYaxis()->SetLabelSize(16);
		hFrame->GetYaxis()->SetLabelOffset(0.02);
		hFrame->GetYaxis()->SetTitleFont(43);
		hFrame->GetYaxis()->SetTitleSize(16);
		hFrame->GetYaxis()->SetTitleOffset(5);  
		hFrame->GetYaxis()->CenterTitle();
		hFrame->GetYaxis()->SetNdivisions(505);
		
		// TICKS Y Axis
		hFrame->GetYaxis()->SetTickLength(xFactor*0.04/yFactor);
		
		// Format for x axis
		hFrame->GetXaxis()->SetTitle("Energy Deposited (MeV)");
		hFrame->GetXaxis()->SetLabelFont(43);
		hFrame->GetXaxis()->SetLabelSize(16);
		hFrame->GetXaxis()->SetLabelOffset(0.02);
		hFrame->GetXaxis()->SetTitleFont(43);
		hFrame->GetXaxis()->SetTitleSize(16);
		hFrame->GetXaxis()->SetTitleOffset(3);
		hFrame->GetXaxis()->CenterTitle();
		hFrame->GetXaxis()->SetNdivisions(505);
		
		// Set X axis range
		hFrame->GetXaxis()->SetRangeUser(0.012,20.0);
		
		// TICKS X Axis
		hFrame->GetXaxis()->SetTickLength(yFactor*0.06/xFactor);
		
		// Count up events in Front layer of taggers above Threshold
		int nBins = hFrame->GetNbinsX();
		double binWidth = hFrame->GetXaxis()->GetBinWidth(10);
		
		for(int k = 0; k < nThresh; k++){
		  double Threshold = Thresholds[k];
		  CTagger[i][j] = hFrame->Integral((Threshold/binWidth),nBins);    
		  CountRates[k][i][j] = CTagger[i][j];
		  cout << "First Veto layer, detector " << pvNum << " counts/s for 1 microAmp of Beam " 
			   << CTagger[i][j]/electronTime/(1e6) << " MHz" << endl;
		  cout << "First Veto layer, detector " << pvNum << " counts/s for 80 microAmp of Beam " 
			   << 80*CTagger[i][j]/electronTime/(1e6) << " MHz" << endl;    
		  cout << " " << endl;
		}
		
		// Plot the threshold data!
		Double_t x[nThresh], y[nThresh];
		C2[n]->cd(0);
		char pname2[16];
		sprintf(pname2,"pad_%i_%i",i,j);
		pad2[n][i][j] = (TPad*) gROOT->FindObject(pname2);
		pad2[n][i][j]->Draw();     
		pad2[n][i][j]->SetLogy();
		pad2[n][i][j]->cd();
		txtOut << RealName << ": AV Number " << AVnum << "  PV Number " << pvNum << std::endl;
		for(int k = 0; k < nThresh; k++){
		  x[k] = Thresholds[k];
		  y[k] = CountRates[k][i][j]/electronTime/(1e6);
		  txtOut << Thresholds[k] << "      " << 80*CountRates[k][i][j]/electronTime/(1e6) << std::endl;
		}
		TGraph *gr = new TGraph(nThresh,x,y); 
     // Set Good Graph Title
		char gtitle[90];
		sprintf(gtitle,"#splitline{Count Rate VS. Threshold}{%s %i, Layer %i}",RealName.c_str(),pvNum+1, imprNum);
		gr->SetTitle(gtitle);   
		
		// Clean up Y axis
		gr->GetYaxis()->SetTitle("Count Rate at 1 #muA Beam (MHz)");   
		gr->GetYaxis()->SetLabelFont(43);
		gr->GetYaxis()->SetLabelSize(16);
		gr->GetYaxis()->SetLabelOffset(0.02);
		gr->GetYaxis()->SetTitleFont(43);
		gr->GetYaxis()->SetTitleSize(16);
		gr->GetYaxis()->SetTitleOffset(5);
		gr->GetYaxis()->CenterTitle(); 
		gr->GetYaxis()->SetRangeUser(0.000,1.1*y[0]);
		
		// Clean up X axis
		gr->GetXaxis()->SetTitle("Threshold Energy (MeV)");
		gr->GetXaxis()->SetLabelFont(43);
		gr->GetXaxis()->SetLabelSize(16);
		gr->GetXaxis()->SetLabelOffset(0.02);
		gr->GetXaxis()->SetTitleFont(43);
		gr->GetXaxis()->SetTitleSize(16);
		gr->GetXaxis()->SetTitleOffset(3);
		gr->GetXaxis()->CenterTitle();
		
		// Go for the plot
		gr->SetMarkerStyle(21);
		gr->Draw("APC");
		
		// Cycle the Physical Volume number and check if you are at the end of a row.
		pvNum++;
		if(pvNum == PVnumMax) { imprNum++; break; }
	  }
	}
	if(imprNum > ImprNumMax) { imprNum = 1; continue; }
  }
  
  //*************** End new Code Looper *********************

  txtOut.close();
  for(int n = 0; n < 22; n++){
	C1[n]->Write();
	C2[n]->Write();
  }
  outFile->Close(); 
  //inFile->Close();

}

void CanvasPartition(TCanvas *C,const Int_t Nx,const Int_t Ny,
                     Float_t lMargin, Float_t rMargin,
                     Float_t bMargin, Float_t tMargin,
		     Float_t vSpacing, Float_t hSpacing)
{
   if (!C) return;

   // Setup Pad layout:
   //Float_t vSpacing = 0.0;
   Float_t vStep  = (1.- bMargin - tMargin - (Ny-1) * vSpacing) / Ny;

   //Float_t hSpacing = 0.0;
   Float_t hStep  = (1.- lMargin - rMargin - (Nx-1) * hSpacing) / Nx;

   Float_t vposd,vposu,vmard,vmaru,vfactor;
   Float_t hposl,hposr,hmarl,hmarr,hfactor;

   for (Int_t i=0;i<Nx;i++) {

      if (i==0) {
         hposl = 0.0;
         hposr = lMargin + hStep;
         hfactor = hposr-hposl;
         hmarl = lMargin / hfactor;
         hmarr = 0.0;
      } else if (i == Nx-1) {
         hposl = hposr + hSpacing;
         hposr = hposl + hStep + rMargin;
         hfactor = hposr-hposl;
         hmarl = 0.0;
         hmarr = rMargin / (hposr-hposl);
      } else {
         hposl = hposr + hSpacing;
         hposr = hposl + hStep;
         hfactor = hposr-hposl;
         hmarl = 0.0;
         hmarr = 0.0;
      }

      for (Int_t j=0;j<Ny;j++) {

         if (j==0) {
            vposd = 0.0;
            vposu = bMargin + vStep;
            vfactor = vposu-vposd;
            vmard = bMargin / vfactor;
            vmaru = 0.0;
         } else if (j == Ny-1) {
            vposd = vposu + vSpacing;
            vposu = vposd + vStep + tMargin;
            vfactor = vposu-vposd;
            vmard = 0.0;
            vmaru = tMargin / (vposu-vposd);
         } else {
            vposd = vposu + vSpacing;
            vposu = vposd + vStep;
            vfactor = vposu-vposd;
            vmard = 0.0;
            vmaru = 0.0;
         }

         C->cd(0);

         char name[16];
         sprintf(name,"pad_%i_%i",i,j);
         TPad *pad = (TPad*) gROOT->FindObject(name);
         if (pad) delete pad;
         pad = new TPad(name,"",hposl,vposd,hposr,vposu);
         pad->SetLeftMargin(hmarl);
         pad->SetRightMargin(hmarr);
         pad->SetBottomMargin(vmard);
	 pad->SetTopMargin(vmaru);

         pad->SetFrameBorderMode(0);
         pad->SetBorderMode(0);
         pad->SetBorderSize(0);

         pad->Draw();
      }
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

TString FormInputFile(TString InputDir){
  
  TString fileName = InputDir + "/histos/" + BaseName + "_Histos.root";
  
  return fileName;
}

TString FormOutputFile(TString OutputDir){
  
  TString fileName =  OutputDir + "/Plots/" + BaseName + "_FrontVetoRates.root";
  
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

void GetCanvasParameters(int n){
  ImprNumMax = 2;
  if(n == 0 || n == 1 || n == 2 || n == 3){
	if(n == 0 || n == 1){ AVnum = 1; PVnumMax = 13; }
	if(n == 2 || n == 3){ AVnum = 2; PVnumMax = 14; }
	FancyName = "TopDet";
	RealName = "Top Detector";
  } else if(n == 6 || n == 7 || n == 8 || n == 9){
	if(n == 6 || n == 7){ AVnum = 5; PVnumMax = 13; }
	if(n == 8 || n == 9){ AVnum = 6; PVnumMax = 14; }
	FancyName = "BottomDet";
	RealName = "Bottom Detector";
  } else if(n == 4 || n == 5){
	ImprNumMax = 1;
	if(n == 4){ AVnum = 3; PVnumMax = 13; }
	if(n == 5){ AVnum = 4; PVnumMax = 14; }
	FancyName = "TopVeto";
	RealName = "Top Veto";
  } else if(n == 10 || n ==11){
	ImprNumMax = 1;
	if(n == 10){ AVnum = 7; PVnumMax = 13; }
	if(n == 11){ AVnum = 8; PVnumMax = 14; }
	FancyName = "BottomVeto";
	RealName = "Bottom Veto";
  } else if(n == 12 || n == 13 || n == 14 || n == 15){
	if(n == 12 || n == 13){ AVnum = 9; PVnumMax = 6;}
	if(n == 14 || n == 15){ AVnum = 10; PVnumMax = 8; }
	FancyName = "FrontDet";
	RealName = "Front Detector";
  } else if(n == 16 || n == 17 || n == 18 || n == 19){
	if(n == 16 || n == 17){ AVnum = 11; PVnumMax = 6; }
	if(n == 18 || n == 19){ AVnum = 12; PVnumMax = 8; }
	FancyName = "FrontVeto";
	RealName = "Front Veto";
  } else if(n == 20 || n == 21){
	AVnum = 13;
	PVnumMax = 16;
	FancyName = "BackTag";
	RealName = "Back Tagger";
  }
  return;
}
