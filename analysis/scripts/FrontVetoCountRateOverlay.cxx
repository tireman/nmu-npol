
#include <cstdlib>
#include <iostream>
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
#include <TMultiGraph.h>
#include <TLegend.h>

void CanvasPartition(TCanvas *C,const Int_t Nx = 2,const Int_t Ny = 2, 
		     Float_t lMargin = 0.15, Float_t rMargin = 0.05,
                     Float_t bMargin = 0.15, Float_t tMargin = 0.05,
		     Float_t vSpacing = 0.0, Float_t hSpacing = 0.0);

int GetAVNumber(const std::string &volName);
int GetImprNumber(const std::string &volName);
int GetPlacementNumber(const std::string &volName);
//TString FormInputFile(TString InputDir);
//TString FormOutputFile(TString OutputDir);
//void RetrieveENVvariables();

TString BaseName = "";  TString JobNum = "";  TString Lead = ""; TString Energy = ""; 
TString Bfield = ""; TString OutputDir = ""; TString InputDir = "";

void FrontVetoCountRateOverlay() {

  Long_t TotalElectrons = 0, TotalEventsRecorded = 0; 

  std::string histoNames[3][2]={{"av_11_impr_1_FrontTagLV_pv_1","av_11_impr_1_FrontTagLV_pv_0"},{"av_11_impr_1_FrontTagLV_pv_3","av_11_impr_1_FrontTagLV_pv_2"},{"av_11_impr_1_FrontTagLV_pv_5","av_11_impr_1_FrontTagLV_pv_4"}};
   
  //RetrieveENVvariables();
  //TString InputFile = FormInputFile(InputDir);
  //TString OutputFile = FormOutputFile(OutputDir);
  
  TFile *inFile = TFile::Open("/data2/cgen/JlabSimData/Summer2016Run/TargetTaggerSource/4.4GeV/1Bdl/SmallTargetTagger/histos/sourceTotal_Lead15cm_4.4GeV_1Bdl_Histos.root");
  TFile *inFile2 = TFile::Open("/data2/cgen/JlabSimData/Summer2016Run/TargetTaggerSource/4.4GeV/4Bdl/SmallTargetTagger/histos/sourceTotal_Lead15cm_4.4GeV_4Bdl_Histos.root");
  TFile *outFile = new TFile("FrontVetoComparison_4Bdl_1Bdl.root","RECREATE");

  // Retrieve the object with the total number of electrons on target and calculate 
  // effective electron time on target per micro amp of beam

  TVectorD *v = (TVectorD*)inFile->Get("TVectorT<double>");
  Double_t totalElectrons = 3.49967e10; //((*v))[0];
  Double_t electronTime = totalElectrons/(6.242e12); //6.242e12 e-/s at 1 microAmp
  //Double_t fluxscaling = 1/(totalElectrons*1.602e-13*(98*60));
  std::cout << "Electron beam time at 1 micro-amp is " << electronTime << " s " << std::endl;
  std::cout << "Total electrons on target: " << totalElectrons/1e6 << " Million" << std::endl;

  TCanvas *c1 = new TCanvas("c1","Front Veto Energy Plots at Polarimeter Angle 28.0 Deg, E = 4.4 GeV",1000,900);
  TCanvas *c2 = new TCanvas("c2","Front Veto Count Rate vs. Threshold plots",1000,900);
  TCanvas *c3 = new TCanvas("c3","Front Veto 1 Tm / 4 Tm Ratio vs. Threshold plots",1000,900);

  Int_t Nx = 3, Ny = 2, nThresh = 10, fillStyle = 1001;
  int pvNum, avNum, imprNum;
  Float_t lMargin = 0.10, rMargin = 0.05, bMargin = 0.10, tMargin = 0.05;
  Float_t vSpacing = 0.0; Float_t hSpacing = 0.0;
  double CTagger[Nx][Ny];
  double Thresholds[10]={0.5,1.0,1.5,2.0,2.5,3.0,3.5,4.0,4.5,5.0};
  double CountRates [nThresh][Nx][Ny];
  double CTagger2[Nx][Ny];
  double CountRates2 [nThresh][Nx][Ny];

  CanvasPartition(c1,Nx,Ny,lMargin,rMargin,bMargin,tMargin,vSpacing,hSpacing);
  CanvasPartition(c2,Nx,Ny,lMargin,rMargin,bMargin,tMargin,vSpacing,hSpacing);
  CanvasPartition(c3,Nx,Ny,lMargin,rMargin,bMargin,tMargin,vSpacing,hSpacing);

  TPad *pad[Nx][Ny];
  TPad *pad1[Nx][Ny];
  TPad *pad2[Nx][Ny];

  for(int i = 0; i < Nx; i++){
   for(int j = 0; j < Ny; j++){
     c1->cd(0);
     // Get the pads previosly created.
     char pname[16];
     sprintf(pname,"pad_%i_%i",i,j);
     pad[i][j] = (TPad*) gROOT->FindObject(pname);
     pad[i][j]->Draw();
     pad[i][j]->SetLogy();
     pad[i][j]->SetFillStyle(4000);
     pad[i][j]->SetFrameFillStyle(4000);
     pad[i][j]->cd();
     // Size factors
     Float_t xFactor = pad[0][0]->GetAbsWNDC()/pad[i][j]->GetAbsWNDC();
     Float_t yFactor = pad[0][0]->GetAbsHNDC()/pad[i][j]->GetAbsHNDC();

     char hname[60];
     //TString str = histoNames[i][j];
     sprintf(hname,"%s",histoNames[i][j].c_str());
     std::cout << "Name is: " << hname << std::endl;
	 TH1F *hFrame = (TH1F*) inFile->Get(hname);
	 TH1F *hFrame2 = (TH1F*) inFile2->Get(hname);
     hFrame->SetStats(false); 
     hFrame->SetFillColor(kBlue);
     hFrame->SetTitleFont(16);
     hFrame->SetFillStyle(fillStyle);
     hFrame->Draw();

	 hFrame2->SetLineColor(kRed);
	 hFrame2->SetFillStyle(fillStyle);
	 hFrame2->Draw("same");

     // Set Good Histogram Title
     avNum = GetAVNumber(hname);
     imprNum = GetImprNumber(hname);
     pvNum = GetPlacementNumber(hname);
     char htitle[80];
     sprintf(htitle,"#splitline{Energy Deposited}{Front Veto %i, Layer %i}",pvNum+1, imprNum);
     hFrame->SetTitle(htitle);     
     // y axis range
     hFrame->GetYaxis()->SetRangeUser(0.2,5e3);
     
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
     
	 TLegend *legend2 = new TLegend(0.7,0.7,0.9,0.8);
	 legend2->AddEntry(hFrame, "1 Tm Field","lp");
	 legend2->AddEntry(hFrame2, "4 Tm Field","lp");
	 legend2->Draw();

     // Count up events in Front layer of taggers above Threshold
     int nBins = hFrame->GetNbinsX();
     double binWidth = hFrame->GetXaxis()->GetBinWidth(10);
     

     for(int k = 0; k < nThresh; k++){
       double Threshold = Thresholds[k];
	   std::cout << "Current Threshold = " << Threshold << " MeV" << std::endl;

       CTagger[i][j] = hFrame->Integral((Threshold/binWidth),nBins);
	   CTagger2[i][j] = hFrame2->Integral((Threshold/binWidth),nBins);

       CountRates[k][i][j] = CTagger[i][j];
	   CountRates2[k][i][j] = CTagger2[i][j];
       
	   cout << "First Veto layer, detector " << pvNum << " counts/s for 1 microAmp of Beam " 
	    << CTagger[i][j]/electronTime/(1e6) << " MHz" << endl;
       cout << "First Veto layer, detector " << pvNum << " counts/s for 80 microAmp of Beam " 
	    << 80*CTagger[i][j]/electronTime/(1e6) << " MHz" << endl;    
       cout << " " << endl;
     }

	 Double_t x[nThresh], y[nThresh], x2[nThresh], y2[nThresh], ratiox[nThresh], ratioy[nThresh];
     c2->cd(0);
     char pname2[16];
     sprintf(pname2,"pad_%i_%i",i,j);
     pad1[i][j] = (TPad*) gROOT->FindObject(pname2);
     pad1[i][j]->Draw();
     pad1[i][j]->cd();
     for(int k = 0; k < nThresh; k++){
       x[k] = Thresholds[k];
       y[k] = CountRates[k][i][j]/electronTime/(1e6);
	   x2[k] = Thresholds[k];
       y2[k] = CountRates2[k][i][j]/electronTime/(1e6);
	   ratiox[k] = Thresholds[k];
	   ratioy[k] = y[k]/y2[k];
     }

     TGraph *gr = new TGraph(nThresh,x,y); 
	 TGraph *gr2 = new TGraph(nThresh,x2,y2);
     // Set Good Graph Title
 
	 sprintf(htitle,"#splitline{Count Rate}{Front Veto %i, Layer %i}",pvNum+1, imprNum);
     // Go for the plot
     gr->SetMarkerStyle(21);
	 gr->SetMarkerColor(4); 
	 gr2->SetMarkerStyle(22);
	 gr2->SetMarkerColor(2);
     //gr->Draw("APC");

	 TMultiGraph *mg = new TMultiGraph();
	 gr->Fit("expo","FQ");
	 gr->SetLineColor(4);
	 gr->GetFunction("expo")->SetLineColor(4);
	 mg->Add(gr,"p");
	 gr2->SetLineColor(2);
	 gr2->Fit("expo","FQ");
	 gr2->GetFunction("expo")->SetLineColor(2);
	 mg->Add(gr2,"p");
	 mg->Draw("a");

	 mg->SetTitle(htitle);
	 mg->GetYaxis()->SetTitle("Count Rate at 1 #muA Beam (MHz)");
	 mg->GetYaxis()->CenterTitle();
	 mg->GetYaxis()->SetLabelOffset(0.02);    
	 mg->GetYaxis()->SetTitleFont(43);
     mg->GetYaxis()->SetTitleSize(16);
     mg->GetYaxis()->SetTitleOffset(5); 
	 mg->GetYaxis()->SetRangeUser(0.000,.165);

	 mg->GetXaxis()->SetTitle("Threshold Energy (MeV)"); 
	 mg->GetXaxis()->CenterTitle();
	 mg->GetXaxis()->SetLabelOffset(0.02);	
	 mg->GetXaxis()->SetTitleFont(43);
     mg->GetXaxis()->SetTitleSize(16);
     mg->GetXaxis()->SetTitleOffset(3);
	 mg->GetXaxis()->SetLabelFont(43);
	 mg->GetXaxis()->SetLabelSize(16);

	 TLegend *legend = new TLegend(0.7,0.7,0.9,0.8);
	 legend->AddEntry(gr, "1 Tm Field","lp");
	 legend->AddEntry(gr2, "4 Tm Field","lp");
	 legend->Draw();

	 pad1[i][j]->Modified();

	 c3->cd(0);
	 // Get the pads previosly created.
     char pname3[16];
     sprintf(pname3,"pad_%i_%i",i,j);
     pad2[i][j] = (TPad*) gROOT->FindObject(pname3);
     pad2[i][j]->Draw();
	 pad2[i][j]->SetFillStyle(4000);
     pad2[i][j]->SetFrameFillStyle(4000);
     pad2[i][j]->cd();
     // Size factors
	 xFactor = pad2[0][0]->GetAbsWNDC()/pad[i][j]->GetAbsWNDC();
	 yFactor = pad2[0][0]->GetAbsHNDC()/pad[i][j]->GetAbsHNDC();

     TGraph *gr3 = new TGraph(nThresh,ratiox,ratioy); 
	 // Set Good Graph Title
   
	 sprintf(htitle,"#splitline{Ratio of Count Rates}{Front Veto %i, Layer %i}",pvNum+1, imprNum);
     // Go for the plot
     gr3->SetMarkerStyle(21);
	 gr3->SetMarkerColor(4); 

	 TMultiGraph *mg2 = new TMultiGraph();
	 gr3->Fit("pol1","FQ");
	 gr3->SetLineColor(4);
	 gr3->GetFunction("pol1")->SetLineColor(4);
	 mg2->Add(gr3,"p");
	 mg2->Draw("a");

	 mg2->SetTitle(htitle);
	 mg2->GetYaxis()->SetTitle("Ratio of 1 Tm Field to 4 Tm Field Count Rate");
	 mg2->GetYaxis()->CenterTitle();
	 mg2->GetYaxis()->SetLabelOffset(0.02);    
	 mg2->GetYaxis()->SetTitleFont(43);
     mg2->GetYaxis()->SetTitleSize(16);
     mg2->GetYaxis()->SetTitleOffset(5);  
	 mg2->GetYaxis()->SetLabelFont(43);
	 mg2->GetYaxis()->SetLabelSize(16);
	 mg2->GetYaxis()->SetRangeUser(1.15,3.75);

	 mg2->GetXaxis()->SetTitle("Threshold Energy (MeV)"); 
	 mg2->GetXaxis()->CenterTitle();
	 mg2->GetXaxis()->SetLabelOffset(0.02);	
	 mg2->GetXaxis()->SetTitleFont(43);
     mg2->GetXaxis()->SetTitleSize(16);
     mg2->GetXaxis()->SetTitleOffset(3);
	 mg2->GetXaxis()->SetLabelFont(43);
	 mg2->GetXaxis()->SetLabelSize(16);

	 //TLegend *legend3 = new TLegend(0.2,0.3,0.3,0.35);
	 //legend3->AddEntry(gr3, "1 Tm / 4 Tm Ratio","lp");
	 //legend3->Draw();

	 pad2[i][j]->Modified();

   }
  }
  
  
  c1->Write();
  c2->Write();
  c3->Write();
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

/*TString FormInputFile(TString InputDir){
  
  TString fileName = InputDir + "/histos/" + BaseName + "_Histos.root";
  
  return fileName;
}

TString FormOutputFile(TString OutputDir){
  
  TString fileName =  OutputDir + "/Plots/" + BaseName + "_FrontTaggerRates.root";
  
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
  }*/

