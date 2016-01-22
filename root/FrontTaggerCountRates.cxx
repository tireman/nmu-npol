
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

void CanvasPartition(TCanvas *C,const Int_t Nx = 2,const Int_t Ny = 2, 
		     Float_t lMargin = 0.15, Float_t rMargin = 0.05,
                     Float_t bMargin = 0.15, Float_t tMargin = 0.05,
		     Float_t vSpacing = 0.0, Float_t hSpacing = 0.0);

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

void FrontTaggerCountRates() {

  Long_t TotalElectrons = 0, TotalEventsRecorded = 0; 

  std::string histoNames[3][2]={{"av_11_impr_1_FrontTagLV_pv_1","av_11_impr_1_FrontTagLV_pv_0"},{"av_11_impr_1_FrontTagLV_pv_3","av_11_impr_1_FrontTagLV_pv_2"},{"av_11_impr_1_FrontTagLV_pv_5","av_11_impr_1_FrontTagLV_pv_4"}};
   
  TString OutputDir = "Plots/";
  TString InputDir = "Output/";

  TString OutputFile = OutputDir + "JLAB4.4GeV_Lead10cm_4Bdl_FrontTaggerRates.root";
  TString InputFile = InputDir + "JLAB4.4GeV_Lead10cm_4Bdl_Histos.root";

  TFile *inFile = TFile::Open(InputFile);
  TFile *outFile = new TFile(OutputFile,"RECREATE");

  // Retrieve the object with the total number of electrons on target and calculate 
  // effective electron time on target per micro amp of beam

  TVectorD *v = (TVectorD*)inFile->Get("TVectorT<double>");
  Double_t totalElectrons = ((*v))[0];
  Double_t electronTime = totalElectrons/(6.242e12); //6.242e12 e-/s at 1 microAmp
  Double_t fluxscaling = 1/(totalElectrons*1.602e-13*(98*60));
  std::cout << "Electron beam time at 1 micro-amp is " << electronTime << " s " << std::endl;
  std::cout << "Total electrons on target: " << totalElectrons/1e6 << " Million" << std::endl;

  TCanvas *c1 = new TCanvas("c1","Polarimeter Angle 28.0 Deg, E = 4.4 GeV",1000,900);
  TCanvas *c2 = new TCanvas("c2","Front Taggger Threshold plots",1000,900);

  Int_t Nx = 3, Ny = 2, nThresh = 10, fillStyle = 1001;
  int pvNum, avNum, imprNum;
  Float_t lMargin = 0.10, rMargin = 0.05, bMargin = 0.10, tMargin = 0.05;
  Float_t vSpacing = 0.0; Float_t hSpacing = 0.0;
  double CTagger[Nx][Ny];
  double Thresholds[10]={0.5,1.0,1.5,2.0,2.5,3.0,3.5,4.0,4.5,5.0};
  double CountRates [nThresh][Nx][Ny];

  CanvasPartition(c1,Nx,Ny,lMargin,rMargin,bMargin,tMargin,vSpacing,hSpacing);
  CanvasPartition(c2,Nx,Ny,lMargin,rMargin,bMargin,tMargin,vSpacing,hSpacing);

  TPad *pad[Nx][Ny];
  TPad *pad1[Nx][Ny];
    
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
     hFrame->SetStats(false); 
     hFrame->SetFillColor(kBlue);
     hFrame->SetTitleFont(16);
     hFrame->SetFillStyle(fillStyle);
     hFrame->Draw();
     // Set Good Histogram Title
     avNum = GetAVNumber(hname);
     imprNum = GetImprNumber(hname);
     pvNum = GetPlacementNumber(hname);
     char htitle[80];
     sprintf(htitle,"#splitline{Energy Deposited}{Front Tagger %i, Layer %i}",pvNum+1, imprNum);
     hFrame->SetTitle(htitle);     
     // y axis range
     hFrame->GetYaxis()->SetRangeUser(0.05,25000);
     
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
     hFrame->GetXaxis()->SetRangeUser(0.0,24);

     // TICKS X Axis
     hFrame->GetXaxis()->SetTickLength(yFactor*0.06/xFactor);
     
     // Count up events in Front layer of taggers above Threshold
     int nBins = hFrame->GetNbinsX();
     double binWidth = hFrame->GetXaxis()->GetBinWidth(10);
     

     for(int k = 0; k < nThresh; k++){
       double Threshold = Thresholds[k];
       
       CTagger[i][j] = hFrame->Integral((Threshold/binWidth),nBins);    
       CountRates[k][i][j] = CTagger[i][j];
       cout << "First Tagger layer, detector " << pvNum << " counts/s for 1 microAmp of Beam " 
	    << CTagger[i][j]/electronTime/(1e6) << " MHz" << endl;
       cout << "First Tagger layer, detector " << pvNum << " counts/s for 80 microAmp of Beam " 
	    << 80*CTagger[i][j]/electronTime/(1e6) << " MHz" << endl;    
       cout << " " << endl;
     }

     Double_t x[nThresh], y[nThresh];
     c2->cd(0);
     char pname2[16];
     sprintf(pname2,"pad_%i_%i",i,j);
     pad1[i][j] = (TPad*) gROOT->FindObject(pname2);
     pad1[i][j]->Draw();
     pad1[i][j]->cd();
     for(int k = 0; k < nThresh; k++){
       x[k] = Thresholds[k];
       y[k] = CountRates[k][i][j]/electronTime/(1e6);
     }
     TGraph *gr = new TGraph(nThresh,x,y); 
     // Set Good Graph Title
     char gtitle[80];
     sprintf(gtitle,"#splitline{Count Rate VS. Threshold}{Front Tagger %i, Layer %i}",pvNum+1, imprNum);
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
     gr->GetYaxis()->SetRangeUser(-0.005,.11);

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
     
   }
  }
  
  
  c1->Write();
  c2->Write();
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
