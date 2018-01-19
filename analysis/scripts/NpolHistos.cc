#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <TStyle.h>
#include <tgmath.h>

#include "TSystem.h"
#include "TVector3.h"
#include "TVector.h"
#include "TMath.h"
#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH1.h"
#include "TH2.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TFrame.h"

void NpolHistos() {

//will now only need one infile -- since the output contains all the histos

TFile *sourceNeutron = new TFile("sourceNeutron_Lead15cm_4.4GeV_4Bdl_NpolEff.root", "READ");
TFile *outFile = new TFile(("~efficiencyHistograms_Test.root"), "RECREATE");

std::ofstream txtOut;
txtOut.open("/dev/null");

//Read in histos, and scale by the event interaction - stored in TVector[1]
TVectorT<double> *stats_sourceNeutron;
stats_sourceNeutron = (TVectorT<double>*)sourceNeutron->Get("TVectorT<double>;1");

//Event Interaction
 double eventInteraction = ((*stats_sourceNeutron)[0]);

//Histos
TH2F *h_dEoverE_AllEvents_Top;
TH2F *h_dEoverE_Elastic_Top;
TH2F *h_dEoverE_Inelastic_Top;
TH2F *h_dEoverE_AllEvents_Bot;
TH2F *h_dEoverE_Elastic_Bot;
TH2F *h_dEoverE_Inelastic_Bot;
TH1F *h_dTOF_AllEvents;
TH1F *h_dTOF_Elastic;
TH1F *h_dTOF_Inelastic; 
TH1F *h_sectionEff_AllEvents[4];
TH1F *h_sectionEff_Elastic[4];
TH1F *h_sectionEff_Inelastic[4];

//Assign Histos
//dE/E
h_dEoverE_AllEvents_Top = (TH2F*)sourceNeutron->Get("dEoverEtop;1");
h_dEoverE_Elastic_Top = (TH2F*)sourceNeutron->Get("dEoverEtop_Elastic;1");
h_dEoverE_Inelastic_Top = (TH2F*)sourceNeutron->Get("dEoverEtop_Inelastic;1");
h_dEoverE_AllEvents_Bot = (TH2F*)sourceNeutron->Get("dEoverEbot;1");
h_dEoverE_Elastic_Bot = (TH2F*)sourceNeutron->Get("dEoverEbot_Elastic;1");
h_dEoverE_Inelastic_Bot = (TH2F*)sourceNeutron->Get("dEoverEbot_Inelastic;1");

//dTof
h_dTOF_AllEvents = (TH1F*)sourceNeutron->Get("dTOF;1");
h_dTOF_Elastic = (TH1F*)sourceNeutron->Get("dTOF_Elastic;1");
h_dTOF_Inelastic = (TH1F*)sourceNeutron->Get("dTOF_Inelastic;1");

//Section Efficiency 
//All Events
  for(int i = 0; i <= 3; i++){ 
    char hname[48];
    sprintf(hname,"sectionEfficiency%i;1",(i+1));
    //std::string beginName = "sectionEfficiency";
	  //std::string beginEndAll = ";1";
	  //std::string beginName = std::to_string(i+1);
	  //beginName = beginName + beginEndAll;
	  //const char *name = beginName.c_str(); 
	  h_sectionEff_AllEvents[i] = (TH1F*)sourceNeutron->Get(hname);
	  h_sectionEff_AllEvents[i]->Scale(100/eventInteraction);
   }
//Elastic
  /*for(int i = 0; i <= 3; i++){ 
	  std::string beginName = "sectionEfficiency";
	  std::string beginEndAll = "_Elastic;1";
	  beginName = beginName + std::to_string(i+1);
	  beginName = beginName + beginEndAll;
	  const char *name = beginName.c_str(); 
	  h_sectionEff_Elastic[i] = (TH1F*)sourceNeutron->Get(name);
	  h_sectionEff_Elastic[i]->Scale(100/eventInteraction);
   }
//Inelastic
  for(int i = 0; i <= 3; i++){ 
	  std::string beginName = "sectionEfficiency";
	  std::string beginEndAll = "_Inelastic;1";
	  beginName = beginName + std::to_string(i+1);
	  beginName = beginName + beginEndAll;
	  const char *name = beginName.c_str(); 
	  h_sectionEff_Inelastic[i] = (TH1F*)sourceNeutron->Get(name);
	  h_sectionEff_Inelastic[i]->Scale(100/eventInteraction);
	  }*/



//CREATE TCANVAS
TCanvas *dEoverE_One = new TCanvas("dEoverE_One","dE Over E - Elastic and Inelastic",1800, 800);
TCanvas *dEoverE_Two = new TCanvas("dEoverE_Two","dE Over E - All Events",1800, 800);
TCanvas *dTOF = new TCanvas("dTOF","dTOF",1800, 800);
TCanvas *sectionEff_AllEvents = new TCanvas("sectionEff_AllEvents","Section Efficiency - All Events",1800, 800);
TCanvas *sectionEff_Elastic = new TCanvas("sectionEff_Elastic","Section Efficiency - Elastic Events",1800, 800);
TCanvas *sectionEff_Inelastic = new TCanvas("sectionEff_Inelastic","Section Efficiency - Inelastic Events",1800, 800);
//Stats Removed**
 gStyle->SetOptStat(0);
 gStyle->SetOptLogz();
 
//dEoverE_One
dEoverE_One->Divide(2,2,0.0001,0.00001,0);
dEoverE_One->cd(1);
h_dEoverE_Elastic_Top->GetXaxis()->SetTitle("E-Array Energy Deposited (MeV)");
h_dEoverE_Elastic_Top->GetXaxis()->SetTitleSize(0.05);
h_dEoverE_Elastic_Top->GetYaxis()->SetTitleSize(0.05);
h_dEoverE_Elastic_Top->GetYaxis()->SetTitle("dE-Array Energy Deposited (MeV)");
h_dEoverE_Elastic_Top->DrawCopy("cont4");
dEoverE_One->cd(2);
h_dEoverE_Inelastic_Top->GetXaxis()->SetTitle("E-Array Energy Deposited (MeV)");
h_dEoverE_Inelastic_Top->GetXaxis()->SetTitleSize(0.05);
h_dEoverE_Inelastic_Top->GetYaxis()->SetTitleSize(0.05);
h_dEoverE_Inelastic_Top->GetYaxis()->SetTitle("dE-Array Energy Deposited (MeV)");
h_dEoverE_Inelastic_Top->DrawCopy("cont4");
dEoverE_One->cd(3);
h_dEoverE_Elastic_Bot->GetXaxis()->SetTitle("E-Array Energy Deposited (MeV)");
h_dEoverE_Elastic_Bot->GetXaxis()->SetTitleSize(0.05);
h_dEoverE_Elastic_Bot->GetYaxis()->SetTitleSize(0.05);
h_dEoverE_Elastic_Bot->GetYaxis()->SetTitle("dE-Array Energy Deposited (MeV)");
h_dEoverE_Elastic_Bot->DrawCopy("cont4");
dEoverE_One->cd(4);
h_dEoverE_Inelastic_Bot->DrawCopy("cont4");
h_dEoverE_Inelastic_Bot->GetXaxis()->SetTitle("E-Array Energy Deposited (MeV)");
h_dEoverE_Inelastic_Bot->GetXaxis()->SetTitleSize(0.05);
h_dEoverE_Inelastic_Bot->GetYaxis()->SetTitleSize(0.05);
h_dEoverE_Inelastic_Bot->GetYaxis()->SetTitle("dE-Array Energy Deposited (MeV)");
h_dEoverE_Elastic_Bot->DrawCopy("cont4");

//dEoverE_Two
dEoverE_Two->Divide(2,2,0.0001,0.00001,0);
dEoverE_Two->cd(1);
h_dEoverE_AllEvents_Top->GetXaxis()->SetTitle("E-Array Energy Deposited (MeV)");
h_dEoverE_AllEvents_Top->GetXaxis()->SetTitleSize(0.05);
h_dEoverE_AllEvents_Top->GetYaxis()->SetTitleSize(0.05);
h_dEoverE_AllEvents_Top->GetYaxis()->SetTitle("dE-Array Energy Deposited (MeV)");
h_dEoverE_AllEvents_Top->DrawCopy("cont4");
dEoverE_Two->cd(2);
h_dEoverE_AllEvents_Bot->GetXaxis()->SetTitle("E-Array Energy Deposited (MeV)");
h_dEoverE_AllEvents_Bot->GetXaxis()->SetTitleSize(0.05);
h_dEoverE_AllEvents_Bot->GetYaxis()->SetTitleSize(0.05);
h_dEoverE_AllEvents_Bot->GetYaxis()->SetTitle("dE-Array Energy Deposited (MeV)");
h_dEoverE_AllEvents_Bot->DrawCopy("cont4");

//dTOF
dTOF->Divide(2,2,0.0001,0.00001,0);
dTOF->cd(1);
h_dTOF_AllEvents->GetXaxis()->SetTitle("Time (ns)");
h_dTOF_AllEvents->GetXaxis()->SetTitleSize(0.05);
h_dTOF_AllEvents->GetYaxis()->SetTitleSize(0.05);
h_dTOF_AllEvents->GetYaxis()->SetTitle("Number of Events");
h_dTOF_AllEvents->SetFillColor(kBlue);
h_dTOF_AllEvents->DrawCopy();
dTOF->cd(2);
h_dTOF_Elastic->GetXaxis()->SetTitle("Time (ns)");
h_dTOF_Elastic->GetXaxis()->SetTitleSize(0.05);
h_dTOF_Elastic->GetYaxis()->SetTitleSize(0.05);
h_dTOF_Elastic->GetYaxis()->SetTitle("Number of Events");
h_dTOF_Elastic->SetFillColor(kBlue);
h_dTOF_Elastic->DrawCopy();
dTOF->cd(3);
h_dTOF_Inelastic->GetXaxis()->SetTitle("Time (ns)");
h_dTOF_Inelastic->GetXaxis()->SetTitleSize(0.05);
h_dTOF_Inelastic->GetYaxis()->SetTitleSize(0.05);
h_dTOF_Inelastic->GetYaxis()->SetTitle("Number of Events");
h_dTOF_Inelastic->SetFillColor(kBlue);
h_dTOF_Inelastic->DrawCopy();

//Section Efficiencies
//All events
sectionEff_AllEvents->Divide(2,2,0.0001,0.00001,0);
	  sectionEff_AllEvents->cd(1);
	  h_sectionEff_AllEvents[0]->SetOption("bar");
	  h_sectionEff_AllEvents[0]->GetXaxis()->SetTitle("Section of Polarimeter");
	  h_sectionEff_AllEvents[0]->GetXaxis()->SetRangeUser(0,5);
	  h_sectionEff_AllEvents[0]->GetXaxis()->SetNdivisions(9);
	  h_sectionEff_AllEvents[0]->GetXaxis()->SetTitleSize(0.05);
	  h_sectionEff_AllEvents[0]->GetYaxis()->SetTitleSize(0.05);
	  h_sectionEff_AllEvents[0]->GetYaxis()->SetTitle("Percent Efficiency");
	  h_sectionEff_AllEvents[0]->SetFillColor(kBlue);
	  h_sectionEff_AllEvents[0]->DrawCopy();
	  sectionEff_AllEvents->cd(2);
	  h_sectionEff_AllEvents[1]->SetOption("bar");
	  h_sectionEff_AllEvents[1]->GetXaxis()->SetTitle("Section of Polarimeter");
	  h_sectionEff_AllEvents[1]->GetXaxis()->SetRangeUser(0,5);
	  h_sectionEff_AllEvents[1]->GetXaxis()->SetNdivisions(9);
	  h_sectionEff_AllEvents[1]->GetXaxis()->SetTitleSize(0.05);
	  h_sectionEff_AllEvents[1]->GetYaxis()->SetTitleSize(0.05);
	  h_sectionEff_AllEvents[1]->GetYaxis()->SetTitle("Percent Efficiency");
	  h_sectionEff_AllEvents[1]->SetFillColor(kBlue);
	  h_sectionEff_AllEvents[1]->DrawCopy();
	  sectionEff_AllEvents->cd(3);
	  h_sectionEff_AllEvents[2]->SetOption("bar");
	  h_sectionEff_AllEvents[2]->GetXaxis()->SetTitle("Section of Polarimeter");
	  h_sectionEff_AllEvents[2]->GetXaxis()->SetRangeUser(0,5);
	  h_sectionEff_AllEvents[2]->GetXaxis()->SetNdivisions(9);
	  h_sectionEff_AllEvents[2]->GetXaxis()->SetTitleSize(0.05);
	  h_sectionEff_AllEvents[2]->GetYaxis()->SetTitleSize(0.05);
	  h_sectionEff_AllEvents[2]->GetYaxis()->SetTitle("Percent Efficiency");
	  h_sectionEff_AllEvents[2]->SetFillColor(kBlue);
	  h_sectionEff_AllEvents[2]->DrawCopy();
	  sectionEff_AllEvents->cd(4);
	  h_sectionEff_AllEvents[3]->SetOption("bar");
	  h_sectionEff_AllEvents[3]->GetXaxis()->SetTitle("Section of Polarimeter");
	  h_sectionEff_AllEvents[3]->GetXaxis()->SetRangeUser(0,5);
	  h_sectionEff_AllEvents[3]->GetXaxis()->SetNdivisions(9);
	  h_sectionEff_AllEvents[3]->GetXaxis()->SetTitleSize(0.05);
	  h_sectionEff_AllEvents[3]->GetYaxis()->SetTitleSize(0.05);
	  h_sectionEff_AllEvents[3]->GetYaxis()->SetTitle("Percent Efficiency");
	  h_sectionEff_AllEvents[3]->SetFillColor(kBlue);
	  h_sectionEff_AllEvents[3]->DrawCopy();
//Elastic
	  sectionEff_Elastic->Divide(2,2,0.0001,0.00001,0);
	  sectionEff_Elastic->cd(1);
	  h_sectionEff_Elastic[0]->GetXaxis()->SetTitle("Section of Polarimeter");
	  h_sectionEff_Elastic[0]->GetXaxis()->SetRangeUser(0,5);
	  h_sectionEff_Elastic[0]->GetXaxis()->SetNdivisions(9);
	  h_sectionEff_Elastic[0]->GetXaxis()->SetTitleSize(0.05);
	  h_sectionEff_Elastic[0]->GetYaxis()->SetTitleSize(0.05);
	  h_sectionEff_Elastic[0]->GetYaxis()->SetTitle("Percent Efficiency");
	  h_sectionEff_Elastic[0]->SetFillColor(kBlue);
	  h_sectionEff_Elastic[0]->DrawCopy();
	  sectionEff_Elastic->cd(2);
	  h_sectionEff_Elastic[1]->GetXaxis()->SetTitle("Section of Polarimeter");
	  h_sectionEff_Elastic[1]->GetXaxis()->SetRangeUser(0,5);
	  h_sectionEff_Elastic[1]->GetXaxis()->SetNdivisions(9);
	  h_sectionEff_Elastic[1]->GetXaxis()->SetTitleSize(0.05);
	  h_sectionEff_Elastic[1]->GetYaxis()->SetTitleSize(0.05);
	  h_sectionEff_Elastic[1]->GetYaxis()->SetTitle("Percent Efficiency");
	  h_sectionEff_Elastic[1]->SetFillColor(kBlue);
	  h_sectionEff_Elastic[1]->DrawCopy();
	  sectionEff_Elastic->cd(3);
	  h_sectionEff_Elastic[2]->GetXaxis()->SetTitle("Section of Polarimeter");
	  h_sectionEff_Elastic[2]->GetXaxis()->SetRangeUser(0,5);
	  h_sectionEff_Elastic[2]->GetXaxis()->SetNdivisions(9);
	  h_sectionEff_Elastic[2]->GetXaxis()->SetTitleSize(0.05);
	  h_sectionEff_Elastic[2]->GetYaxis()->SetTitleSize(0.05);
	  h_sectionEff_Elastic[2]->GetYaxis()->SetTitle("Percent Efficiency");
      h_sectionEff_Elastic[2]->SetFillColor(kBlue);
	  h_sectionEff_Elastic[2]->DrawCopy();
	  sectionEff_Elastic->cd(4);
	  h_sectionEff_Elastic[3]->GetXaxis()->SetTitle("Section of Polarimeter");
	  h_sectionEff_Elastic[3]->GetXaxis()->SetRangeUser(0,5);
	  h_sectionEff_Elastic[3]->GetXaxis()->SetNdivisions(9);
	  h_sectionEff_Elastic[3]->GetXaxis()->SetTitleSize(0.05);
	  h_sectionEff_Elastic[3]->GetYaxis()->SetTitleSize(0.05);
	  h_sectionEff_Elastic[3]->GetYaxis()->SetTitle("Percent Efficiency");
	  h_sectionEff_Elastic[3]->SetFillColor(kBlue);
	  h_sectionEff_Elastic[3]->DrawCopy();
//Inelastic
	  sectionEff_Inelastic->Divide(2,2,0.0001,0.00001,0);
	  sectionEff_Inelastic->cd(1);
	  h_sectionEff_Inelastic[0]->GetXaxis()->SetTitle("Section of Polarimeter");
	  h_sectionEff_Inelastic[0]->GetXaxis()->SetRangeUser(0,5);
	  h_sectionEff_Inelastic[0]->GetXaxis()->SetNdivisions(9);
	  h_sectionEff_Inelastic[0]->GetXaxis()->SetTitleSize(0.05);
	  h_sectionEff_Inelastic[0]->GetYaxis()->SetTitleSize(0.05);
	  h_sectionEff_Inelastic[0]->GetYaxis()->SetTitle("Percent Efficiency");
	  h_sectionEff_Inelastic[0]->SetFillColor(kBlue);
	  h_sectionEff_Inelastic[0]->DrawCopy();
	  sectionEff_Inelastic->cd(2);
	  h_sectionEff_Inelastic[1]->GetXaxis()->SetTitle("Section of Polarimeter");
	  h_sectionEff_Inelastic[1]->GetXaxis()->SetRangeUser(0,5);
	  h_sectionEff_Inelastic[1]->GetXaxis()->SetNdivisions(9);
	  h_sectionEff_Inelastic[1]->GetXaxis()->SetTitleSize(0.05);
	  h_sectionEff_Inelastic[1]->GetYaxis()->SetTitleSize(0.05);
	  h_sectionEff_Inelastic[1]->GetYaxis()->SetTitle("Percent Efficiency");
	  h_sectionEff_Inelastic[1]->SetFillColor(kBlue);
	  h_sectionEff_Inelastic[1]->DrawCopy();
	  sectionEff_Inelastic->cd(3);
	  h_sectionEff_Inelastic[2]->GetXaxis()->SetTitle("Section of Polarimeter");
	  h_sectionEff_Inelastic[2]->GetXaxis()->SetRangeUser(0,5);
	  h_sectionEff_Inelastic[2]->GetXaxis()->SetNdivisions(9);
	  h_sectionEff_Inelastic[2]->GetXaxis()->SetTitleSize(0.05);
	  h_sectionEff_Inelastic[2]->GetYaxis()->SetTitleSize(0.05);
	  h_sectionEff_Inelastic[2]->GetYaxis()->SetTitle("Percent Efficiency");
	  h_sectionEff_Inelastic[2]->SetFillColor(kBlue);
	  h_sectionEff_Inelastic[2]->DrawCopy();
	  sectionEff_Inelastic->cd(4);
	  h_sectionEff_Inelastic[3]->GetXaxis()->SetTitle("Section of Polarimeter");
	  h_sectionEff_Inelastic[3]->GetXaxis()->SetRangeUser(0,5);
	  h_sectionEff_Inelastic[3]->GetXaxis()->SetNdivisions(9);
	  h_sectionEff_Inelastic[3]->GetXaxis()->SetTitleSize(0.05);
	  h_sectionEff_Inelastic[3]->GetYaxis()->SetTitleSize(0.05);
	  h_sectionEff_Inelastic[3]->GetYaxis()->SetTitle("Percent Efficiency");
	  h_sectionEff_Inelastic[3]->SetFillColor(kBlue);
	  h_sectionEff_Inelastic[3]->DrawCopy();

dEoverE_One->Write();
dEoverE_Two->Write();
dTOF->Write();
sectionEff_AllEvents->Write();
sectionEff_Elastic->Write();
sectionEff_Inelastic->Write();

//Close files
outFile->Close();
sourceNeutron->Close();
txtOut.close();

}
