
#include <cstdlib>
#include <iostream>
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

void FillCanvas(TCanvas *C, Double_t scaleFactor,TFile *F, std::string histoNames[3][3], struct histoParams plotSettings);
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
Long_t TotalElectrons = 0, TotalEventsRecorded = 0; 
Double_t solidAngle;

// main function
void EstimateCrossSection() {

  std::string histoNames[3][3]={{"pi-","mu+","gamma"},
								{"neutron","mu-","e+"},
								{"proton","pi+","e-"}};
  
  RetrieveENVvariables();
  
  TString InputFile = FormInputFile(InputDir);
  TString OutputFile = FormOutputFile(OutputDir);
  TFile *inFile = TFile::Open(InputFile);
  TFile *outFile = new TFile(OutputFile,"RECREATE");
  
  TVectorD *v = (TVectorD*)inFile->Get("TVectorT<double>");
  TotalElectrons = ((*v))[0];
  Double_t electronTime = TotalElectrons/(6.242e12); //6.242e12 e-/s at 1 microAmp
  
  Double_t targetTaggerPos = 150.0;
  Double_t npolTaggerPos = 683.86;
  Double_t theta = 138.12e-3;  // Vertical angle
  Double_t phi = 67.16e-3; //84.66e-3;    // Horizontal angle
  solidAngle = 4*asin(sin(theta/2)*sin(phi/2));
  
  // My scale for NPOL Tagger
  //Double_t fluxscaling1 = 1/(totalElectrons*1.602e-13*pow(618.0,2)*solidAngle)); 
  //Double_t fluxscaling2 = 1/(totalElectrons*1.602e-13*pow(112.0,2)*solidAngle)); 
  
  //Proposal Scale for NPOL Tagger
  Double_t fluxscaling1 = 1/(TotalElectrons*pow(npolTaggerPos,2)*solidAngle);  // full solid angle calculation for a pyramid
  Double_t fluxscaling2 = 1/(TotalElectrons*pow(targetTaggerPos,2)*solidAngle);  
   
  // Put out some statistics
  std::cout << "Electron beam time at 1 micro-amp is " << electronTime << " s " << std::endl;
  std::cout << "Total electrons on target: " << TotalElectrons/1e6 << " Million\n" << std::endl;
 
  
  // Plot the Target Tagger (just before the first magnet) flux plots
  TCanvas *c1 = new TCanvas("c2a","Target Tagger Flux vs. KE at Polarimeter Angle 28.0 Deg, E = 4.4 GeV",1000,900);
  Float_t lMargin = 0.10, rMargin = 0.05, bMargin = 0.07, tMargin = 0.05;
  Float_t vSpacing = 0.0; Float_t hSpacing = 0.0;
    
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
  plotSettings.Ranges.insert(std::pair<std::string, Double_t>("yLow",2e-18)); 
  plotSettings.Ranges.insert(std::pair<std::string, Double_t>("yHigh",2e-9));
  plotSettings.Ranges.insert(std::pair<std::string, Double_t>("zLow",0.0)); 
  plotSettings.Ranges.insert(std::pair<std::string, Double_t>("zHigh",1000.0));
 
  CanvasPartition(c1,plotSettings.Nx,plotSettings.Ny,lMargin,rMargin,bMargin,tMargin,vSpacing,hSpacing);
  FillCanvas(c1, fluxscaling2, inFile,  histoNames, plotSettings);
  


  c1->Write();
  //c2->Write();
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
	  
	  // section to calculate the absolute cross section?
	  Double_t targetTaggerPos = 150.0;
	  Double_t npolTaggerPos = 683.86;
	  Double_t molarMass = 2.014;         // LD2 molar mass (g/Mole)
	  Double_t AvogradoNumber = 6.0222e23; // that thing (nuclei per Mole)
	  Double_t thickness = 40.0;          // target thickness (cm)
	  Double_t densityD = 0.169;          // LD2 density (g/cm^3)
	  Double_t nucleiPerVolume = AvogradoNumber*densityD/molarMass; 
	  
	  Int_t totalCounts = 0;
	  for(int i = 1; i <= hFrame->GetNbinsX()-2; i++){
		if(hFrame->GetBinCenter(i) >= 100){
		  totalCounts += hFrame->GetBinContent(i);
		}
	  }
	  
	  std::cout << "Total Number of " << histoNames[i][j] << ": " 
				<< totalCounts << " Counts" << std::endl;
	  
	  std::cout << "Cross Section for " << histoNames[i][j]  << ": " 
				<< totalCounts/(TotalElectrons*solidAngle*nucleiPerVolume
				*thickness)/1e-24/1e-6 << " microbarns/sr\n" << endl; 
	  
	  
	  
	  // Bin-by-bin scaling to the width of the bins in eVs
	  if((*plotSettings.plotFlags.find("binScale")).second){	
		for(int i = 1; i <= hFrame->GetNbinsX()-2; i++){
		  double oldbincontent = hFrame->GetBinContent(i);
		  double scalingfactor = scaleFactor/TMath::Log10(hFrame->GetBinCenter(i)*1e6);
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
	  hFrame->GetYaxis()->SetRangeUser((*plotSettings.Ranges.find("yLow")).second ,
									   (*plotSettings.Ranges.find("yHigh")).second);
	  
	  // Format for y axis
	  hFrame->GetYaxis()->SetTitle(plotSettings.xTitle.c_str());
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
}


TString FormInputFile(TString InputDir){
  
  TString fileName = InputDir + "/" + BaseName + "_" 
	+ Energy + "GeV_" + "Lead" + Lead + "cm_" + Bfield + "Bdl_Histos.root";
  
  return fileName;
}

TString FormOutputFile(TString OutputDir){
  
  TString fileName =  OutputDir + "/" 
	+ BaseName + Energy + "GeV_Lead" + Lead + "cm_" + Bfield + "Bdl_CrossSection.root";
  
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

