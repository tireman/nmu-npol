
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

int PVnumMax = 0, AVnum = 1, ImprNumMax = 2, layerNum = 1;
TString BaseName = "";  TString JobNum = "";  TString Lead = ""; TString Energy = ""; 
TString Bfield = ""; TString OutputDir = ""; TString InputDir = "";
std::string FancyName = ""; std::string RealName = "";

void NpolDetectorCountRates() {

  RetrieveENVvariables();
  Long_t TotalElectrons = 0, TotalEventsRecorded = 0; 
  TString InputFile = FormInputFile(InputDir);
  TString OutputFile = FormOutputFile(OutputDir);
  TFile *inFile = TFile::Open(InputFile);
  TFile *outFile = new TFile(OutputFile,"RECREATE");

  // Retrieve the object with the total number of electrons on target and calculate 
  // effective electron time on target per micro amp of beam
  TVectorD *v = (TVectorD*)inFile->Get("TVectorT<double>");
  Double_t totalElectrons = 100*19.995e9; //((*v))[0]; //1e11; //((*v))[0];
  Double_t electronTime = totalElectrons/(6.242e12); //6.242e12 e-/s at 1 microAmp
  std::cout << "Electron beam time at 1 micro-amp is " << electronTime << " s " << std::endl;
  std::cout << "Total electrons on target: " << totalElectrons/1e9 << " Billion" << std::endl;

  int pvNum = 1, avNum = 1, imprNum = 1;
  Int_t Nx = 16, Ny = 2, nThresh = 10, fillStyle = 1001;
  Float_t lMargin = 0.05, rMargin = 0.05, bMargin = 0.10, tMargin = 0.05;
  Float_t vSpacing = 0.0; Float_t hSpacing = 0.0;
  double DetCounts[Nx][Ny];
  double CountRates [nThresh][Nx][Ny];
  std::ofstream txtOut, thresh1MeV, thresh10MeV;
  txtOut.open(OutputDir + "/Output/NpolDetectorCountRates" + Energy + "GeV_" + Lead + "cm.out");
  thresh1MeV.open(OutputDir + "/Output/Threshold1MeVCountRates" + Energy + "GeV_" + Lead + "cm.out");
  thresh10MeV.open(OutputDir + "/Output/Threshold10MeVCountRates" + Energy + "GeV_" + Lead + "cm.out");
  //***************************  Main portion of code ***************************************
  // The main job of this code is to take in a file containing all the energy histograms of
  // the CGEN polarimeter and generate 2 canvas plots for each assembly volume/imprint group.
  // The first canvas is just formatted energy depositied plots.  The second canvas is a set
  // of graphs plotting raw count rates versus threshold setting.  These graphs have been 
  // scaled to 1 micro-amp of electron beam on target for convenience.  All canvases are 
  // saved to a ROOT file and the threshold values are saved to text file as well.
  //
  // Three loops are needed. Loop over 'n' is for all the assembly volume/imprint combos.
  // There are currently 22 of them.  Loop over 'i' and 'j' fill the canvases with the 
  // individual plots.  
  //*****************************************************************************************

  TCanvas *C1[22], *C2[22], *C3[22];
  TPad *pad1[22][7][2]; TPad *pad2[22][7][2]; TPad *pad3[22][7][2];
  char histoName[60], tempName[9], tempName2[8], tempName3[9]; 
  for(int n = 0; n < 22; n++){	
	pvNum = 0;
	GetCanvasParameters(n);
	if(PVnumMax == 13){ Nx = 7; } else { Nx = PVnumMax/2; }
	sprintf(tempName,"canvas%i",n);
	sprintf(tempName2,"graph%i",n);
	sprintf(tempName3,"Counts%i",n);
	C1[n]= new TCanvas(tempName,"Energy Plots at Polarimeter Angle 28.0 Deg, E = 4.4 GeV",1500,900);
	C2[n]= new TCanvas(tempName2,"Count Rate vs. Threshold plots",1500,900);
	C3[n]= new TCanvas(tempName3,"Counts vs. Threshold plots",1500,900);
	
	CanvasPartition(C1[n],Nx,Ny,lMargin,rMargin,bMargin,tMargin,vSpacing,hSpacing);
	CanvasPartition(C2[n],Nx,Ny,lMargin,rMargin,bMargin,tMargin,vSpacing,hSpacing);
	CanvasPartition(C3[n],Nx,Ny,lMargin,rMargin,bMargin,tMargin,vSpacing,hSpacing);

	thresh1MeV << RealName << ": AV Number " << AVnum << " Imprint Number " << imprNum << std::endl;
	thresh10MeV << RealName << ": AV Number " << AVnum << " Imprint Number " << imprNum <<  std::endl;

	for(int i = 0; i < Nx; i++){
	  for(int j = 0; j < Ny; j++){
		sprintf(histoName,"av_%i_impr_%i_%sLV_pv_%i",AVnum,imprNum,FancyName.c_str(),pvNum);
		std::cout << "Detector name = " << histoName << std::endl;

		double Thresholds[10];
		switch(AVnum){
		case 1: case 2: case 5: case 6: case 9: case 10:
		  Thresholds[0] = 1.0; Thresholds[1] = 2.0;
		  for(int AB = 2; AB < 10; AB++){
			Thresholds[AB] = Thresholds[AB-1] + 2.0;
		  }
		  break;
		case 3: case 4: case 7: case 8: case 11: case 12: case 13:
		  Thresholds[0] = 0.5;
		  for(int AB = 1; AB < 10; AB++){
			Thresholds[AB] = Thresholds[AB-1] + 0.5;
		  }
		  break;
		default:
		  break;
		}
		
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

		// Get the histogram and begin preparing it and placing on the TPad
		TH1F *hFrame = (TH1F*) inFile->Get(histoName);
		if(hFrame == NULL){
		  std::cout << "Histogram is Empty" << std::endl;
		  txtOut << "Histogram is Empty" << std::endl;
		  continue;}
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
		sprintf(htitle,"#splitline{Energy Deposited}{%s %i, Layer %i}",RealName.c_str(),pvNum+1, layerNum);
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
		
		// This loop computes the thresholds and prints them to the screen.
		// This is needed for the second canvas
		int nBins = hFrame->GetNbinsX();
		double binWidth = hFrame->GetXaxis()->GetBinWidth(10);
		double largestY = 0.0, smallestY = 100000.0;
		double largestY2 = 0.0, smallestY2 = 10000.0;
		Double_t x[nThresh], y[nThresh], x2[nThresh], y2[nThresh];
		txtOut << RealName << ": AV Number " << AVnum << " Imprint Number " << imprNum << 
		  "  PV Number " << pvNum << std::endl;
		for(int k = 0; k < nThresh; k++){
		  double Threshold = Thresholds[k];
		  DetCounts[i][j] = hFrame->Integral((Threshold/binWidth),nBins);    
		  CountRates[k][i][j] = DetCounts[i][j]/electronTime/(1e3);
		  std::cout << "Threshold = " << Thresholds[k] << " MeV" << std::endl;
		  cout << RealName << ", detector # " << pvNum << " counts/s for 1 microAmp of Beam " 
			   << CountRates[k][i][j] << " kHz" << endl;
		  cout << RealName << ", detector # " << pvNum << " counts/s for 80 microAmp of Beam " 
			   << 80*CountRates[k][i][j] << " kHz" << endl;    
		  cout << " " << endl;
		  // THis creates the x,y vectors for TGraph later on
		  x[k] = Thresholds[k];
		  y[k] = CountRates[k][i][j]; // scaled to 1 uA beam and 1 MHz
		  if(y[k] > largestY) largestY = y[k];
		  if(y[k] < smallestY) smallestY = y[k];
		  
		  x2[k] = Thresholds[k];
		  y2[k] = DetCounts[i][j]; // scaled to 1 uA beam and 1 MHz
		  if(y2[k] > largestY2) largestY2 = y2[k];
		  if(y2[k] < smallestY2) smallestY2 = y2[k];
		  
		  // Output to text file for getting the numbers 
		  txtOut << Thresholds[k] << "      " << 
			DetCounts[i][j] << " Counts" << "      " << 
			CountRates[k][i][j] << " kHz" <<std::endl;

		  if(Thresholds[k] == 1){
			thresh1MeV << pvNum << "      " << Thresholds[k] << "      " << 
			DetCounts[i][j] << " Counts" << "      " << 
			CountRates[k][i][j] << " kHz" <<std::endl;
		  } else if(Thresholds[k] == 10){
			thresh10MeV << pvNum  << "      " << Thresholds[k] << "      " << 
			DetCounts[i][j] << " Counts" << "      " << 
			CountRates[k][i][j] << " kHz" <<std::endl;
		  }
		}
		txtOut << std::endl;

		// Plot the threshold data!	
		C2[n]->cd(0);
		char pname2[16];
		sprintf(pname2,"pad_%i_%i",i,j);
		pad2[n][i][j] = (TPad*) gROOT->FindObject(pname2);
		pad2[n][i][j]->Draw();     
		pad2[n][i][j]->SetLogy();
		pad2[n][i][j]->cd();

		//  Generate the graph and format it ... somewhat nicely
		TGraph *gr = new TGraph(nThresh,x,y); 
		char gtitle[90];
		sprintf(gtitle,"#splitline{Count Rate VS. Threshold}{%s %i, Layer %i}",
				RealName.c_str(),pvNum+1, layerNum);
		gr->SetTitle(gtitle);   

		// Clean up Y axis
		gr->GetYaxis()->SetTitle("Count Rate at 1 #muA Beam (kHz)");   
		gr->GetYaxis()->SetLabelFont(43);
		gr->GetYaxis()->SetLabelSize(16);
		gr->GetYaxis()->SetLabelOffset(0.02);
		gr->GetYaxis()->SetTitleFont(43);
		gr->GetYaxis()->SetTitleSize(16);
		gr->GetYaxis()->SetTitleOffset(5);
		gr->GetYaxis()->CenterTitle(); 
		gr->SetMinimum(0.6*smallestY);
		gr->SetMaximum(2*largestY);
		//gr->Fit("expo","FQ");
		//gr->SetLineColor(4);
		//gr->GetFunction("expo")->SetLineColor(4);
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
		
		C3[n]->cd(0);
		char pname3[16];
		sprintf(pname3,"pad_%i_%i",i,j);
		pad3[n][i][j] = (TPad*) gROOT->FindObject(pname3);
		pad3[n][i][j]->Draw();     
		pad3[n][i][j]->SetLogy();
		pad3[n][i][j]->cd();

		//  Generate the graph and format it ... somewhat nicely
		TGraph *gr2 = new TGraph(nThresh,x2,y2); 
		//char gtitle[90];
		sprintf(gtitle,"#splitline{Detector Counts VS. Threshold}{%s %i, Layer %i}",
				RealName.c_str(),pvNum+1, layerNum);
		gr2->SetTitle(gtitle);   

		// Clean up Y axis
		gr2->GetYaxis()->SetTitle("Total Counts at 1 #muA Beam (Counts)");   
		gr2->GetYaxis()->SetLabelFont(43);
		gr2->GetYaxis()->SetLabelSize(16);
		gr2->GetYaxis()->SetLabelOffset(0.02);
		gr2->GetYaxis()->SetTitleFont(43);
		gr2->GetYaxis()->SetTitleSize(16);
		gr2->GetYaxis()->SetTitleOffset(5);
		gr2->GetYaxis()->CenterTitle(); 
		gr2->SetMinimum(0.6*smallestY2);
		gr2->SetMaximum(2*largestY2);
		//gr2->Fit("expo","FQ");
		//gr2->SetLineColor(4);
		//gr2->GetFunction("expo")->SetLineColor(4);
		// Clean up X axis
		gr2->GetXaxis()->SetTitle("Threshold Energy (MeV)");
		gr2->GetXaxis()->SetLabelFont(43);
		gr2->GetXaxis()->SetLabelSize(16);
		gr2->GetXaxis()->SetLabelOffset(0.02);
		gr2->GetXaxis()->SetTitleFont(43);
		gr2->GetXaxis()->SetTitleSize(16);
		gr2->GetXaxis()->SetTitleOffset(3);
		gr2->GetXaxis()->CenterTitle();
		
		// Go for the plot
		gr2->SetMarkerStyle(21);
		gr2->Draw("APC");

		// Cycle the Physical Volume number and check if you are at the end of a row.
		// If so, increase imprint number by 1 and break the loop otherwise continue
		pvNum++;
		if(pvNum == PVnumMax) { imprNum++; break; }
	  }
	}
	// Check imprNum against max allowed; cycle to next detector if greater than max
	if(imprNum > ImprNumMax) { imprNum = 1; continue; }
  }
  
  for(int i = 0; i < 22; i++){
	C1[i]->Write();
	C2[i]->Write();
	C3[i]->Write();
  }
  txtOut.close();
  thresh1MeV.close();
  thresh10MeV.close();
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
         hposl = 0.03;
         hposr = lMargin + hStep;
         hfactor = hposr-hposl;
         hmarl = lMargin / hfactor;
         hmarr = 0.05;
      } else if (i == Nx-1) {
         hposl = hposr + hSpacing;
         hposr = hposl + hStep + rMargin;
         hfactor = hposr-hposl;
         hmarl = 0.20;
         hmarr = rMargin / (hposr-hposl);
      } else {
         hposl = hposr + hSpacing;
         hposr = hposl + hStep;
         hfactor = hposr-hposl;
         hmarl = 0.20;
         hmarr = 0.05;
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
  
  TString fileName =  OutputDir + "/Plots/" + BaseName + "_NpolRates.root";
  
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

// This method returns a set of values (av, pvMax imprintMax, names) for each 'n'
// value that is cycled through in the main program.  It's a poor man's decoder
// to be honest.
void GetCanvasParameters(int n){
  ImprNumMax = 2;
  if(n == 0 || n == 1 || n == 2 || n == 3){
	if(n == 0 || n == 1){ AVnum = 1; PVnumMax = 13; }
	if(n == 2 || n == 3){ AVnum = 2; PVnumMax = 14; }
	FancyName = "TopDet"; RealName = "Top Detector";
	layerNum = 1;
  } else if(n == 6 || n == 7 || n == 8 || n == 9){
	if(n == 6 || n == 7){ AVnum = 5; PVnumMax = 13; }
	if(n == 8 || n == 9){ AVnum = 6; PVnumMax = 14; }
	FancyName = "BottomDet"; RealName = "Bottom Detector";
	layerNum = 1;
  } else if(n == 4 || n == 5){
	ImprNumMax = 1;
	if(n == 4){ AVnum = 3; PVnumMax = 13; }
	if(n == 5){ AVnum = 4; PVnumMax = 14; }
	FancyName = "TopVeto"; RealName = "Top Veto";
	layerNum = 1;
  } else if(n == 10 || n ==11){
	ImprNumMax = 1;
	if(n == 10){ AVnum = 7; PVnumMax = 13; }
	if(n == 11){ AVnum = 8; PVnumMax = 14; }
	FancyName = "BottomVeto"; RealName = "Bottom Veto";
	layerNum = 1;
  } else if(n == 12 || n == 13 || n == 14 || n == 15){
	if(n == 12){ AVnum = 9; PVnumMax = 6; layerNum = 1;}
	if(n == 13){ AVnum = 9; PVnumMax = 6; layerNum = 2;}
	if(n == 14){ AVnum = 10; PVnumMax = 8; layerNum = 3;}
	if(n == 15){ AVnum = 10; PVnumMax = 8; layerNum = 4;}
	FancyName = "FrontDet"; RealName = "Front Detector";
  } else if(n == 16 || n == 17 || n == 18 || n == 19){
	if(n == 16){ AVnum = 11; PVnumMax = 6; layerNum = 1; }
	if(n == 17){ AVnum = 11; PVnumMax = 6; layerNum = 2; }
	if(n == 18){ AVnum = 12; PVnumMax = 8; layerNum = 3; }
	if(n == 19){ AVnum = 12; PVnumMax = 8; layerNum = 4; }
	FancyName = "FrontVeto"; RealName = "Front Veto";
  } else if(n == 20 || n == 21){
	if(n == 20){ AVnum = 13; PVnumMax = 16; layerNum = 1; }
	if(n == 21){ AVnum = 13; PVnumMax = 16; layerNum = 2; }
	FancyName = "BackTag"; RealName = "Back Tagger";
  }
  return;
}
