
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

void CanvasPartition(TCanvas *C,const Int_t Nxx = 2,const Int_t Nyy = 2, 
					 Float_t lMargin = 0.15, Float_t rMargin = 0.05,
                     Float_t bMargin = 0.15, Float_t tMargin = 0.05,
					 Float_t vSpacing = 0.0, Float_t hSpacing = 0.0);

void FillCanvas(TCanvas *C, Double_t scaleFactor,TFile *F, std::string histoNames[3][3], 
				struct histoParams plotSettings);
TString FormInputFile(TString InputDir);
TString FormOutputFile(TString OutputDir);
void RetrieveENVvariables();

struct histoParams {
  std::string leadName; 
  std::map<std::string, bool> plotFlags; 
  Int_t fillStyle; 
  Int_t Nx; Int_t Ny; 
  std::string plotStyle; 
  std::string xTitle; 
  std::string yTitle; 
  std::string zTitle; 
  std::map<std::string, Double_t> Ranges;
} plotSettings;

TString BaseName = "";  TString JobNum = "";  TString Lead = ""; TString Energy = ""; 
TString Bfield = ""; TString OutputDir = ""; TString InputDir = "";

// main function
void SimulationFigures() {

  Long_t TotalElectrons = 0, TotalEventsRecorded = 0; 
  std::string histoNames[3][3]={{"pi-","mu+","gamma"},
								{"neutron","mu-","e+"},
								{"proton","pi+","e-"}};
  
  RetrieveENVvariables();
  
  TString InputFile = FormInputFile(InputDir);
  TString OutputFile = FormOutputFile(OutputDir);
  TFile *inFile = TFile::Open(InputFile);
  TFile *outFile = new TFile(OutputFile,"RECREATE");
  
  TVectorD *v = (TVectorD*)inFile->Get("TVectorT<double>");
  Double_t totalElectrons = ((*v))[0];
  Double_t electronTime = totalElectrons/(6.242e12); //6.242e12 e-/s at 1 microAmp
  
  // Tagger sizes
  Double_t theta = 138.12e-3; //369.08e-3; // Target Tagger // 138.12e-3; // horizontal angular accecptance (radians)
  Double_t phi = 67.16e-3; //139.77e-3; // Target Tagger // 67.16e-3;  // using the Dipole 1 limit // 0.08466; // vertical angular acceptance (radians)
  Double_t targetD = 150.0;  // Position of target tagger (cm)
  Double_t targetW = 2*targetD*TMath::Tan(theta/2);  // height of target tagger (cm)
  Double_t targetL = 2*targetD*TMath::Tan(phi/2);   // width of target tagger (cm)
  Double_t npolD = 683.89;  // Position of Npol Tagger (cm)
  Double_t npolW = 2*npolD*TMath::Tan(theta/2);  // height of npol tagger (cm)
  Double_t npolL = 2*npolD*TMath::Tan(phi/2);  // width of npol tagger (cm)

  Double_t targetAlpha = targetW/(2*targetD);   // Constant needed for solid angle
  Double_t npolAlpha = npolW/(2*npolD);  // Constant needed for solid angle
  
  // Solid angle calculation! 
  Double_t targetSolidAngle = 8*(TMath::ATan(targetL/targetW)-TMath::ASin(targetL/TMath::Sqrt((1+TMath::Power(targetAlpha,2))*(TMath::Power(targetL,2)+TMath::Power(targetW,2)))));
  Double_t npolSolidAngle = 8*(TMath::ATan(npolL/npolW)-TMath::ASin(npolL/TMath::Sqrt((1+TMath::Power(npolAlpha,2))*(TMath::Power(npolL,2)+TMath::Power(npolW,2)))));
  
  std::cout << "Target solid angle = " << targetSolidAngle << std::endl;
  std::cout << "Npol solid angle = " << npolSolidAngle << std::endl;

  // My scale 
  //Double_t fluxscaling1 = 1/(totalElectrons*1.602e-13*pow(618.0,2)*npolSolidAngle)); 
  //Double_t fluxscaling2 = 1/(totalElectrons*1.602e-13*pow(112.0,2)*targetSolidAngle)); 
  
  //Proposal Scale
  Double_t fluxscaling1 = 1/(totalElectrons*pow(100.,2)*npolSolidAngle);  // G4 Version 683.86 cm // full solid angle calculation for a pyramid
  Double_t fluxscaling2 = 1/(totalElectrons*pow(100.,2)*targetSolidAngle);  // G4 Version 150.0 cm
   
  // Put out some statistics
  std::cout << "Electron beam time at 1 micro-amp is " << electronTime << " s " << std::endl;
  std::cout << "Total electrons on target: " << totalElectrons/1e6 << " Million" << std::endl;
  
  // Plot the Npol Tagger Flux plots
  TCanvas *c1 = new TCanvas("c1","NPOL Tagger Flux vs. KE at Polarimeter Angle 28.0 Deg, E = 4.4 GeV",1000,900);
  Float_t lMargin = 0.10, rMargin = 0.05, bMargin = 0.07, tMargin = 0.05;
  Float_t vSpacing = 0.0; Float_t hSpacing = 0.0;
  
  plotSettings.leadName = "NpolFlux"; plotSettings.Nx = 3; plotSettings.Ny = 3;
  plotSettings.plotFlags.insert(std::pair<std::string, bool>("xAxis",true)); 
  plotSettings.plotFlags.insert(std::pair<std::string, bool>("yAxis",true)); 
  plotSettings.plotFlags.insert(std::pair<std::string, bool>("zAxis",false)); 
  plotSettings.plotFlags.insert(std::pair<std::string, bool>("binScale",true));
  plotSettings.fillStyle = 1001; plotSettings.plotStyle = ""; 
  plotSettings.xTitle = "#frac{Particles}{electron #times cm^{2}#times Log_{10}T}"; 
  //plotSettings.xTitle = "#frac{Particles}{#muA #times cm^{2}}";  // my scale
  plotSettings.yTitle = "Kinetic Energy (MeV)"; 
  plotSettings.zTitle = "z title";
  plotSettings.Ranges.insert(std::pair<std::string, Double_t>("xLow",1e-1)); 
  plotSettings.Ranges.insert(std::pair<std::string, Double_t>("xHigh",1e4));
  plotSettings.Ranges.insert(std::pair<std::string, Double_t>("yLow",2e-15)); 
  plotSettings.Ranges.insert(std::pair<std::string, Double_t>("yHigh",8e-8));
  plotSettings.Ranges.insert(std::pair<std::string, Double_t>("zLow",0.0)); 
  plotSettings.Ranges.insert(std::pair<std::string, Double_t>("zHigh",1000.0));
  
  CanvasPartition(c1,plotSettings.Nx,plotSettings.Ny,lMargin,rMargin,bMargin,tMargin,vSpacing,hSpacing);
  FillCanvas(c1, fluxscaling1, inFile, histoNames, plotSettings);
  
  // Plot the Target Tagger (just before the first magnet) flux plots
  TCanvas *c2a = new TCanvas("c2a","Target Tagger Flux vs. KE at Polarimeter Angle 28.0 Deg, E = 4.4 GeV",1000,900);
  lMargin = 0.10; rMargin = 0.05; bMargin = 0.07; tMargin = 0.05;
  vSpacing = 0.0; hSpacing = 0.0;
  
  plotSettings.plotFlags.clear(); plotSettings.Ranges.clear();
  plotSettings.leadName = "TargetFlux"; plotSettings.Nx = 3; plotSettings.Ny = 3;
  plotSettings.plotFlags.insert(std::pair<std::string, bool>("xAxis",true)); 
  plotSettings.plotFlags.insert(std::pair<std::string, bool>("yAxis",true)); 
  plotSettings.plotFlags.insert(std::pair<std::string, bool>("zAxis",false)); 
  plotSettings.plotFlags.insert(std::pair<std::string, bool>("binScale",true));
  plotSettings.fillStyle = 1001; plotSettings.plotStyle = ""; 
  plotSettings.xTitle = "#frac{Particles}{electron #times cm^{2}#times Log_{10}T}"; 
  //plotSettings.xTitle = "#frac{Particles}{#muA #times cm^{2}}";  // my scale
  plotSettings.yTitle = "Kinetic Energy (MeV)"; 
  plotSettings.zTitle = "z title";
  plotSettings.Ranges.insert(std::pair<std::string, Double_t>("xLow",1e-1)); 
  plotSettings.Ranges.insert(std::pair<std::string, Double_t>("xHigh",1e4));
  plotSettings.Ranges.insert(std::pair<std::string, Double_t>("yLow",2e-15)); 
  plotSettings.Ranges.insert(std::pair<std::string, Double_t>("yHigh",8e-8));
  plotSettings.Ranges.insert(std::pair<std::string, Double_t>("zLow",0.0)); 
  plotSettings.Ranges.insert(std::pair<std::string, Double_t>("zHigh",1000.0));
 
  CanvasPartition(c2a,plotSettings.Nx,plotSettings.Ny,lMargin,rMargin,bMargin,tMargin,vSpacing,hSpacing);
  FillCanvas(c2a, fluxscaling2, inFile,  histoNames, plotSettings);
  
  // Plot the Target Tagger flux plot requiring a hit in the NPOLll tagger as well
  TCanvas *c2b = new TCanvas("c2b","Correlated Target Tagger Flux vs. KE at Polarimeter Angle 28.0 Deg, E = 4.4 GeV",1000,900);
  lMargin = 0.10; rMargin = 0.05; bMargin = 0.07; tMargin = 0.05;
  vSpacing = 0.0; hSpacing = 0.0;

  plotSettings.plotFlags.clear(); plotSettings.Ranges.clear();
  plotSettings.leadName = "Correlated_TargetFlux"; plotSettings.Nx = 3; plotSettings.Ny = 3;
  plotSettings.plotFlags.insert(std::pair<std::string, bool>("xAxis",true)); 
  plotSettings.plotFlags.insert(std::pair<std::string, bool>("yAxis",true)); 
  plotSettings.plotFlags.insert(std::pair<std::string, bool>("zAxis",false)); 
  plotSettings.plotFlags.insert(std::pair<std::string, bool>("binScale",true));
  plotSettings.fillStyle = 1001; plotSettings.plotStyle = ""; 
  plotSettings.xTitle = "#frac{Particles}{electron #times cm^{2}#times Log_{10}T}"; 
  //plotSettings.xTitle = "#frac{Particles}{#muA #times cm^{2}}";  // my scale
  plotSettings.yTitle = "Kinetic Energy (MeV)"; 
  plotSettings.zTitle = "z title";
  plotSettings.Ranges.insert(std::pair<std::string, Double_t>("xLow",1e-1)); 
  plotSettings.Ranges.insert(std::pair<std::string, Double_t>("xHigh",1e4));
  plotSettings.Ranges.insert(std::pair<std::string, Double_t>("yLow",2e-15)); 
  plotSettings.Ranges.insert(std::pair<std::string, Double_t>("yHigh",4e-10));
  plotSettings.Ranges.insert(std::pair<std::string, Double_t>("zLow",0.0)); 
  plotSettings.Ranges.insert(std::pair<std::string, Double_t>("zHigh",1000.0));
 
  CanvasPartition(c2b,plotSettings.Nx,plotSettings.Ny,lMargin,rMargin,bMargin,tMargin,vSpacing,hSpacing);
  FillCanvas(c2b, fluxscaling2, inFile, histoNames, plotSettings);
  
  // Plot of the xy position in the npol tagger volume for various particles   
  TCanvas *c3 = new TCanvas("c3","Position in Npol Tagger with Polarimeter Angle 28.0 Deg, E = 4.4 GeV",1000,900);
  lMargin = 0.07; rMargin = 0.03; bMargin = 0.08; tMargin = 0.05;
  vSpacing = 0.0; hSpacing = 0.0;
  
  plotSettings.plotFlags.clear(); plotSettings.Ranges.clear();
  plotSettings.leadName = "npolXY"; plotSettings.Nx = 3; plotSettings.Ny = 3;
  plotSettings.plotFlags.insert(std::pair<std::string, bool>("xAxis",false)); 
  plotSettings.plotFlags.insert(std::pair<std::string, bool>("yAxis",false)); 
  plotSettings.plotFlags.insert(std::pair<std::string, bool>("zAxis",false)); 
  plotSettings.plotFlags.insert(std::pair<std::string, bool>("binScale",false));
  plotSettings.fillStyle = 1001; plotSettings.plotStyle = "Cont1"; 
  plotSettings.xTitle = "Horizontal Position (cm)"; 
  plotSettings.yTitle = "Vertical Position (cm)"; 
  plotSettings.zTitle = "#frac{Particles}{#muA #times cm^{2}}";
  plotSettings.Ranges.insert(std::pair<std::string, Double_t>("xLow",220.0)); 
  plotSettings.Ranges.insert(std::pair<std::string, Double_t>("xHigh",370.0));
  plotSettings.Ranges.insert(std::pair<std::string, Double_t>("yLow",-70.0)); 
  plotSettings.Ranges.insert(std::pair<std::string, Double_t>("yHigh",70.0));
  plotSettings.Ranges.insert(std::pair<std::string, Double_t>("zLow",0.0)); 
  plotSettings.Ranges.insert(std::pair<std::string, Double_t>("zHigh",1000.0));
  
  CanvasPartition(c3, plotSettings.Nx, plotSettings.Ny,lMargin,rMargin,bMargin,tMargin,vSpacing,hSpacing);
  FillCanvas(c3, 1, inFile, histoNames, plotSettings);
  
  c1->Write();
  c2a->Write();
  c2b->Write();
  c3->Write();
  outFile->Close();
  inFile->Close();
}

  void CanvasPartition(TCanvas *C,const Int_t Nx,const Int_t Ny,Float_t lMargin, Float_t rMargin,Float_t bMargin, Float_t tMargin,Float_t vSpacing, Float_t hSpacing){

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

void FillCanvas(TCanvas *C, Double_t scaleFactor, TFile *inFile, std::string histoNames[3][3], struct histoParams plotSettings){
  
  Int_t Nx = plotSettings.Nx; Int_t Ny = plotSettings.Ny;
  TPad *pad[Nx][Ny];	  
  Double_t totalParticles = 0;
  for(int i = 0; i < Nx; i++){
	for(int j = 0; j < Ny; j++){
	  C->cd(0);
	  // Get the pads previosly created.
	  char pname[16];
	  sprintf(pname,"pad_%i_%i",i,j);
	  pad[i][j] = (TPad*) gROOT->FindObject(pname);
	  pad[i][j]->Draw();
	  if((*plotSettings.plotFlags.find("xAxis")).second) pad[i][j]->SetLogx();
  if((*plotSettings.plotFlags.find("yAxis")).second) pad[i][j]->SetLogy();
  if((*plotSettings.plotFlags.find("zAxis")).second) pad[i][j]->SetLogy();
  pad[i][j]->SetFillStyle(4000);
  pad[i][j]->SetFrameFillStyle(4000);
  pad[i][j]->SetGrid(1,1);
  pad[i][j]->cd();
  // Size factors
  Float_t xFactor = pad[0][0]->GetAbsWNDC()/pad[i][j]->GetAbsWNDC();
  Float_t yFactor = pad[0][0]->GetAbsHNDC()/pad[i][j]->GetAbsHNDC();
  
  char hname[32];
  sprintf(hname,"%s_%s",plotSettings.leadName.c_str(),histoNames[i][j].c_str());
  TH1F *hFrame = (TH1F*) inFile->Get(hname);
  hFrame->SetStats(false); 
  hFrame->SetFillColor(kBlue);
  hFrame->SetTitleFont(16);     
  hFrame->SetOption(plotSettings.plotStyle.c_str());
  hFrame->SetFillStyle(plotSettings.fillStyle);

  Double_t totalParticles = hFrame->Integral();

  // Bin-by-bin scaling to the width of the bins in eVs
  if((*plotSettings.plotFlags.find("binScale")).second){	
    for(int i = 1; i <= hFrame->GetNbinsX()-2; i++){
      double oldbincontent = hFrame->GetBinContent(i);
      double scalingfactor = scaleFactor/TMath::Log10(hFrame->GetBinWidth(i)*1e6);
	  hFrame->SetBinContent(i,oldbincontent*scalingfactor);
    }
  } else {
    hFrame->Scale(scaleFactor);
  }
  
  hFrame->Draw();
    
  if(plotSettings.leadName == "npolXY"){
  hFrame->GetZaxis()->SetTitle(plotSettings.zTitle.c_str());    // Format for Z axis
  hFrame->GetZaxis()->SetRangeUser(0.001,1.10*hFrame->GetMaximum()); // Z axis range
}
  // y axis range 
  hFrame->GetYaxis()->SetRangeUser((*plotSettings.Ranges.find("yLow")).second ,(*plotSettings.Ranges.find("yHigh")).second);
  
  // Format for y axis
  hFrame->GetYaxis()->SetTitle(plotSettings.xTitle.c_str());
  hFrame->GetYaxis()->SetLabelFont(43);
  hFrame->GetYaxis()->SetLabelSize(16);
  hFrame->GetYaxis()->SetLabelOffset(0.02);
  hFrame->GetYaxis()->SetTitleFont(43);
  hFrame->GetYaxis()->SetTitleSize(16);
  hFrame->GetYaxis()->SetTitleOffset(5);
  //hFrame->SetGridY();
  hFrame->GetYaxis()->CenterTitle();
  hFrame->GetYaxis()->SetNdivisions(505);
  
  // TICKS Y Axis
  hFrame->GetYaxis()->SetTickLength(xFactor*0.04/yFactor);
  
  // Format for x axis
  hFrame->GetXaxis()->SetTitle(plotSettings.yTitle.c_str());
  hFrame->GetXaxis()->SetLabelFont(43);
  hFrame->GetXaxis()->SetLabelSize(16);
  hFrame->GetXaxis()->SetLabelOffset(0.02);
  hFrame->GetXaxis()->SetTitleFont(43);
  hFrame->GetXaxis()->SetTitleSize(16);
  hFrame->GetXaxis()->SetTitleOffset(5);
  
  hFrame->GetXaxis()->CenterTitle();
  hFrame->GetXaxis()->SetNdivisions(505);
  
  // Set X axis range
  hFrame->GetXaxis()->SetRangeUser((*plotSettings.Ranges.find("xLow")).second ,(*plotSettings.Ranges.find("xHigh")).second);
  
  // TICKS X Axis
  hFrame->GetXaxis()->SetTickLength(yFactor*0.06/xFactor);
  hFrame->SetDirectory(0);
  }
}
  std::cout << "number of particles = " << totalParticles << std::endl;
}


TString FormInputFile(TString InputDir){
  
  TString fileName = InputDir + "/histos/" + BaseName + "_Histos.root";

  return fileName;
  
}

TString FormOutputFile(TString OutputDir){
  
  TString fileName =  OutputDir + "/Plots/" + BaseName + "_Fig21-23.root";
  
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

