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

TString BaseName = "";
TString JobNum = "";
TString Lead = ""; 
TString Energy = "";
TString Bfield = "";
TString OutputDir = "";
TString InputDir = "";
TString CType = "";

void RetrieveENVvariables();
TString FormInputFile(TString InputDir);
TString FormOutputFile(TString OutputDir);
int vertScaleVal(int yVal);

void NpolEffHistos() {

  RetrieveENVvariables();
  
  // will now only need one infile -- since the output contains all the histos
  TString InputFile = FormInputFile(InputDir);
  TString OutputFile = FormOutputFile(OutputDir);

  TFile *sourceNeutron = new TFile(InputFile, "READ");
  TFile *outFile = new TFile(OutputFile,"RECREATE"); 

// Read in histos, and scale by the event interaction - stored in TVector[1]
  TVectorT<double> *stats_sourceNeutron;
  stats_sourceNeutron = (TVectorT<double>*)sourceNeutron->Get("TVectorT<double>;1");
  
// We want taggedEvents variable from NpolAnalysis code which is total QE neutrons entering NPOL.
  double eventInteraction = ((*stats_sourceNeutron)[1]);  // was [5] before 3/10/2018

  // Histogram Stats Removed**
  //gStyle->SetOptStat(0);
  gStyle->SetOptLogz();
  
  char TYPE[12];
  char hname[50];
  char htitle[80];
  sprintf(TYPE,"Elastic");  // What do you want to plot? QuasiElastic? Elastic? Inelastic?
  
  sprintf(hname,"RA_dEoverEtop%s;1",TYPE);
  
  // The Histograms we are going to make
  //   1D Histograms
  TH1F *h_ND_InitialNeutronEnergy_Elastic = (TH1F*)sourceNeutron->Get("ND_InitialNeutronEnergy_Elastic");
  TH1F *h_ND_InitialNeutronEnergy_InElastic = (TH1F*)sourceNeutron->Get("ND_InitialNeutronEnergy_InElastic");
  TH1F *h_ND_InitialNeutronEnergy_QuasiElastic = (TH1F*)sourceNeutron->Get("ND_InitialNeutronEnergy_QuasiElastic");
  TH1F *h_ND_InitialNeutronMomentum_Elastic = (TH1F*)sourceNeutron->Get("ND_InitialNeutronMomentum_Elastic");
  TH1F *h_ND_InitialNeutronMomentum_InElastic = (TH1F*)sourceNeutron->Get("ND_InitialNeutronMomentum_Elastic");
  TH1F *h_ND_InitialNeutronMomentum_QuasiElastic = (TH1F*)sourceNeutron->Get("ND_InitialNeutronMomentum_Elastic");
  TH1F *h_ND_NeutronEnergy_Elastic = (TH1F*)sourceNeutron->Get("ND_NeutronEnergy_Elastic");
  TH1F *h_ND_NeutronEnergy_InElastic = (TH1F*)sourceNeutron->Get("ND_NeutronEnergy_InElastic");
  TH1F *h_ND_NeutronEnergy_QuasiElastic = (TH1F*)sourceNeutron->Get("ND_NeutronEnergy_QuasiElastic");
  TH1F *h_ND_NeutronMomentum_Elastic = (TH1F*)sourceNeutron->Get("ND_NeutronMomentum_Elastic");
  TH1F *h_ND_NeutronMomentum_InElastic = (TH1F*)sourceNeutron->Get("ND_NeutronMomentum_Elastic");
  TH1F *h_ND_NeutronMomentum_QuasiElastic = (TH1F*)sourceNeutron->Get("ND_NeutronMomentum_Elastic");
  TH1F *h_ND_NeutronThetaAngle_Elastic = (TH1F*)sourceNeutron->Get("ND_NeutronThetaAngle_Elastic");
  TH1F *h_ND_NeutronThetaAngle_InElastic = (TH1F*)sourceNeutron->Get("ND_NeutronThetaAngle_InElastic");
  TH1F *h_ND_NeutronThetaAngle_QuasiElastic = (TH1F*)sourceNeutron->Get("ND_NeutronThetaAngle_QuasiElastic");
  TH1F *h_NP_RecoilMomentum_Elastic = (TH1F*)sourceNeutron->Get("NP_RecoilMomentum_Elastic");
  TH1F *h_NP_RecoilMomentum_InElastic = (TH1F*)sourceNeutron->Get("NP_RecoilMomentum_InElastic");
  TH1F *h_NP_RecoilMomentum_QuasiElastic = (TH1F*)sourceNeutron->Get("NP_RecoilMomentum_QuasiElastic");
  TH1F *h_NP_RecoilMomentum_All = new TH1F("NP_RecoilMomentum_All","NP Real Elastic Scattering (P_leading - P_elastic)",200, -3000.0, 1000.);
  h_NP_RecoilMomentum_All->Add(h_NP_RecoilMomentum_Elastic,h_NP_RecoilMomentum_InElastic);
  h_NP_RecoilMomentum_All->Add(h_NP_RecoilMomentum_All,h_NP_RecoilMomentum_QuasiElastic);
  TH1F *h_NP_asymmetry_Elastic = (TH1F*)sourceNeutron->Get("NP_asymmetry_Elastic");
  TH1F *h_NP_asymmetry_InElastic = (TH1F*)sourceNeutron->Get("NP_asymmetry_InElastic");
  TH1F *h_NP_asymmetry_QuasiElastic = (TH1F*)sourceNeutron->Get("NP_asymmetry_QuasiElastic");
  TH1F *h_NP_recoilAngle_Elastic = (TH1F*)sourceNeutron->Get("NP_recoilAngle_Elastic");
  TH1F *h_NP_recoilAngle_InElastic = (TH1F*)sourceNeutron->Get("NP_recoilAngle_InElastic");
  TH1F *h_NP_recoilAngle_QuasiElastic = (TH1F*)sourceNeutron->Get("NP_recoilAngle_QuasiElastic");
  TH1F *h_NP_recoilEnergy_Elastic = (TH1F*)sourceNeutron->Get("NP_recoilEnergy_Elastic");
  TH1F *h_NP_recoilEnergy_InElastic = (TH1F*)sourceNeutron->Get("NP_recoilEnergy_InElastic");
  TH1F *h_NP_recoilEnergy_QuasiElastic = (TH1F*)sourceNeutron->Get("NP_recoilEnergy_QuasiElastic");

  TH1F *h_RA_dTOF_Elastic = (TH1F*)sourceNeutron->Get("RA_dTOF_Elastic");
  TH1F *h_RA_dTOF_InElastic = (TH1F*)sourceNeutron->Get("RA_dTOF_InElastic");
  TH1F *h_RA_dTOF_QuasiElastic = (TH1F*)sourceNeutron->Get("RA_dTOF_QuasiElastic");
  TH1F *h_RA_recoilAngleRaw_Elastic = (TH1F*)sourceNeutron->Get("RA_recoilAngleRaw_Elastic");
  TH1F *h_RA_recoilAngleRaw_InElastic = (TH1F*)sourceNeutron->Get("RA_recoilAngleRaw_InElastic");
  TH1F *h_RA_recoilAngleRaw_QuasiElastic = (TH1F*)sourceNeutron->Get("RA_recoilAngleRaw_QuasiElastic");
  TH1F *h_RA_recoilAngle_Elastic = (TH1F*)sourceNeutron->Get("RA_recoilAngle_Elastic");
  TH1F *h_RA_recoilAngle_InElastic = (TH1F*)sourceNeutron->Get("RA_recoilAngle_InElastic");
  TH1F *h_RA_recoilAngle_QuasiElastic = (TH1F*)sourceNeutron->Get("RA_recoilAngle_QuasiElastic");
  TH1F *h_RA_section1Efficiency_Elastic = (TH1F*)sourceNeutron->Get("RA_section1Efficiency_Elastic");
  TH1F *h_RA_section1Efficiency_InElastic = (TH1F*)sourceNeutron->Get("RA_section1Efficiency_InElastic");
  TH1F *h_RA_section1Efficiency_QuasiElastic = (TH1F*)sourceNeutron->Get("RA_section1Efficiency_QuasiElastic");
  TH1F *h_RA_section2Efficiency_Elastic = (TH1F*)sourceNeutron->Get("RA_section2Efficiency_Elastic");
  TH1F *h_RA_section2Efficiency_InElastic = (TH1F*)sourceNeutron->Get("RA_section2Efficiency_InElastic");
  TH1F *h_RA_section2Efficiency_QuasiElastic = (TH1F*)sourceNeutron->Get("RA_section2Efficiency_QuasiElastic");
  TH1F *h_RA_section3Efficiency_Elastic = (TH1F*)sourceNeutron->Get("RA_section3Efficiency_Elastic");
  TH1F *h_RA_section3Efficiency_InElastic = (TH1F*)sourceNeutron->Get("RA_section3Efficiency_InElastic");
  TH1F *h_RA_section3Efficiency_QuasiElastic = (TH1F*)sourceNeutron->Get("RA_section3Efficiency_QuasiElastic");
  TH1F *h_RA_section4Efficiency_Elastic = (TH1F*)sourceNeutron->Get("RA_section4Efficiency_Elastic");
  TH1F *h_RA_section4Efficiency_InElastic = (TH1F*)sourceNeutron->Get("RA_section4Efficiency_InElastic");
  TH1F *h_RA_section4Efficiency_QuasiElastic = (TH1F*)sourceNeutron->Get("RA_section4Efficiency_QuasiElastic");
  TH1F *h_RA_totalEnergy_Elastic = (TH1F*)sourceNeutron->Get("RA_totalEnergy_Elastic");
  TH1F *h_RA_totalEnergy_InElastic = (TH1F*)sourceNeutron->Get("RA_totalEnergy_InElastic");
  TH1F *h_RA_totalEnergy_QuasiElastic = (TH1F*)sourceNeutron->Get("RA_totalEnergy_QuasiElastic");
  
  //   2D Histograms
  TH2F *h_NP_dEvsE_Elastic = (TH2F*)sourceNeutron->Get("NP_dEvsE_Elastic");
  TH2F *h_NP_dEvsE_InElastic = (TH2F*)sourceNeutron->Get("NP_dEvsE_InElastic");
  TH2F *h_NP_dEvsE_QuasiElastic = (TH2F*)sourceNeutron->Get("NP_dEvsE_QuasiElastic");
  TH2F *h_NP_dEvsE2_Elastic = (TH2F*)sourceNeutron->Get("NP_dEvsE2_Elastic");
  TH2F *h_NP_dEvsE2_InElastic = (TH2F*)sourceNeutron->Get("NP_dEvsE2_InElastic");
  TH2F *h_NP_dEvsE2_QuasiElastic = (TH2F*)sourceNeutron->Get("NP_dEvsE2_QuasiElastic");
  TH2F *h_RA_dEoverEbot_Elastic = (TH2F*)sourceNeutron->Get("RA_dEoverEbot_Elastic");
  TH2F *h_RA_dEoverEbot_InElastic = (TH2F*)sourceNeutron->Get("RA_dEoverEbot_InElastic");
  TH2F *h_RA_dEoverEbot_QuasiElastic = (TH2F*)sourceNeutron->Get("RA_dEoverEbot_QuasiElastic");
  TH2F *h_RA_dEoverEtop_Elastic = (TH2F*)sourceNeutron->Get("RA_dEoverEtop_Elastic");
  TH2F *h_RA_dEoverEtop_InElastic = (TH2F*)sourceNeutron->Get("RA_dEoverEtop_InElastic");
  TH2F *h_RA_dEoverEtop_QuasiElastic = (TH2F*)sourceNeutron->Get("RA_dEoverEtop_QuasiElastic");
  TH2F *h_RA_dEoverEBoth_Elastic = new TH2F("RA_dEoverEBoth_Elastic", "dE over E for top array (Elastic)", 400,0,120,400,0,20);
  h_RA_dEoverEBoth_Elastic->Add(h_RA_dEoverEtop_Elastic,h_RA_dEoverEbot_Elastic);
  TH2F *h_RA_dEoverEBoth_InElastic = new TH2F("RA_dEoverEBoth_InElastic", "dE over E for top array (InElastic)", 400,0,120,400,0,20);
  h_RA_dEoverEBoth_InElastic->Add(h_RA_dEoverEtop_InElastic,h_RA_dEoverEbot_InElastic);
  TH2F *h_RA_dEoverEBoth_QuasiElastic = new TH2F("RA_dEoverEBoth_QuasiElastic", "dE over E for top array (QuasiElastic)", 400,0,120,400,0,20);
  h_RA_dEoverEBoth_QuasiElastic->Add(h_RA_dEoverEtop_QuasiElastic,h_RA_dEoverEbot_QuasiElastic);

  TCanvas *neutronInformationElastic = new TCanvas("neutronInformationElastic","Elastic Events Neutron Diagnostics",800,900);
  neutronInformationElastic->Divide(2,3,0.0001, 0.00001,0);
  neutronInformationElastic->cd(1);
  h_ND_InitialNeutronMomentum_Elastic->GetXaxis()->SetTitle("Initial Neutron Momentum (MeV/c)");
  h_ND_InitialNeutronMomentum_Elastic->GetYaxis()->SetTitle("Neutron Count");
  h_ND_InitialNeutronMomentum_Elastic->GetXaxis()->SetNdivisions(9);
  h_ND_InitialNeutronMomentum_Elastic->GetXaxis()->SetTitleSize(0.05);
  h_ND_InitialNeutronMomentum_Elastic->GetYaxis()->SetTitleSize(0.05);
  h_ND_InitialNeutronMomentum_Elastic->DrawCopy();
  neutronInformationElastic->cd(2);
  h_ND_NeutronMomentum_Elastic->GetXaxis()->SetTitle("Neutron Momentum (MeV/c)");
  h_ND_NeutronMomentum_Elastic->GetYaxis()->SetTitle("Neutron Count");
  h_ND_NeutronMomentum_Elastic->GetXaxis()->SetNdivisions(9);
  h_ND_NeutronMomentum_Elastic->GetXaxis()->SetTitleSize(0.05);
  h_ND_NeutronMomentum_Elastic->GetYaxis()->SetTitleSize(0.05);
  h_ND_NeutronMomentum_Elastic->DrawCopy();
  neutronInformationElastic->cd(3);
  h_ND_InitialNeutronEnergy_Elastic->GetXaxis()->SetTitle("Initial Neutron Energy (MeV)");
  h_ND_InitialNeutronEnergy_Elastic->GetYaxis()->SetTitle("Neutron Count");
  h_ND_InitialNeutronEnergy_Elastic->GetXaxis()->SetNdivisions(9);
  h_ND_InitialNeutronEnergy_Elastic->GetXaxis()->SetTitleSize(0.05);
  h_ND_InitialNeutronEnergy_Elastic->GetYaxis()->SetTitleSize(0.05);
  h_ND_InitialNeutronEnergy_Elastic->DrawCopy();
  neutronInformationElastic->cd(4);
  h_ND_NeutronEnergy_Elastic->GetXaxis()->SetTitle("Neutron Energy (MeV)");
  h_ND_NeutronEnergy_Elastic->GetYaxis()->SetTitle("Neutron Count");
  h_ND_NeutronEnergy_Elastic->GetXaxis()->SetNdivisions(9);
  h_ND_NeutronEnergy_Elastic->GetXaxis()->SetTitleSize(0.05);
  h_ND_NeutronEnergy_Elastic->GetYaxis()->SetTitleSize(0.05);
  h_ND_NeutronEnergy_Elastic->DrawCopy();
  neutronInformationElastic->cd(5);
  h_ND_NeutronThetaAngle_Elastic->GetXaxis()->SetTitle("Initial Neutron Angle (degrees)");
  h_ND_NeutronThetaAngle_Elastic->GetYaxis()->SetTitle("Neutron Count");
  h_ND_NeutronThetaAngle_Elastic->GetXaxis()->SetNdivisions(9);
  h_ND_NeutronThetaAngle_Elastic->GetXaxis()->SetTitleSize(0.05);
  h_ND_NeutronThetaAngle_Elastic->GetYaxis()->SetTitleSize(0.05);
  h_ND_NeutronThetaAngle_Elastic->DrawCopy();
  TCanvas *neutronInformationInElastic = new TCanvas("neutronInformationInElastic","InElastic Events Neutron Diagnostics",800,900);
  neutronInformationInElastic->Divide(2,3,0.0001, 0.00001,0);
  neutronInformationInElastic->cd(1);
  h_ND_InitialNeutronMomentum_InElastic->GetXaxis()->SetTitle("Initial Neutron Momentum (MeV/c)");
  h_ND_InitialNeutronMomentum_InElastic->GetYaxis()->SetTitle("Neutron Count");
  h_ND_InitialNeutronMomentum_InElastic->GetXaxis()->SetNdivisions(9);
  h_ND_InitialNeutronMomentum_InElastic->GetXaxis()->SetTitleSize(0.05);
  h_ND_InitialNeutronMomentum_InElastic->GetYaxis()->SetTitleSize(0.05);
  h_ND_InitialNeutronMomentum_InElastic->DrawCopy();
  neutronInformationInElastic->cd(2);
  h_ND_NeutronMomentum_InElastic->GetXaxis()->SetTitle("Neutron Momentum (MeV/c)");
  h_ND_NeutronMomentum_InElastic->GetYaxis()->SetTitle("Neutron Count");
  h_ND_NeutronMomentum_InElastic->GetXaxis()->SetNdivisions(9);
  h_ND_NeutronMomentum_InElastic->GetXaxis()->SetTitleSize(0.05);
  h_ND_NeutronMomentum_InElastic->GetYaxis()->SetTitleSize(0.05);
  h_ND_NeutronMomentum_InElastic->DrawCopy();
  neutronInformationInElastic->cd(3);
  h_ND_InitialNeutronEnergy_InElastic->GetXaxis()->SetTitle("Initial Neutron Energy (MeV)");
  h_ND_InitialNeutronEnergy_InElastic->GetYaxis()->SetTitle("Neutron Count");
  h_ND_InitialNeutronEnergy_InElastic->GetXaxis()->SetNdivisions(9);
  h_ND_InitialNeutronEnergy_InElastic->GetXaxis()->SetTitleSize(0.05);
  h_ND_InitialNeutronEnergy_InElastic->GetYaxis()->SetTitleSize(0.05);
  h_ND_InitialNeutronEnergy_InElastic->DrawCopy();
  neutronInformationInElastic->cd(4);
  h_ND_NeutronEnergy_InElastic->GetXaxis()->SetTitle("Neutron Energy (MeV)");
  h_ND_NeutronEnergy_InElastic->GetYaxis()->SetTitle("Neutron Count");
  h_ND_NeutronEnergy_InElastic->GetXaxis()->SetNdivisions(9);
  h_ND_NeutronEnergy_InElastic->GetXaxis()->SetTitleSize(0.05);
  h_ND_NeutronEnergy_InElastic->GetYaxis()->SetTitleSize(0.05);
  h_ND_NeutronEnergy_InElastic->DrawCopy();
  neutronInformationInElastic->cd(5);
  h_ND_NeutronThetaAngle_InElastic->GetXaxis()->SetTitle("Initial Neutron Angle (degrees)");
  h_ND_NeutronThetaAngle_InElastic->GetYaxis()->SetTitle("Neutron Count");
  h_ND_NeutronThetaAngle_InElastic->GetXaxis()->SetNdivisions(9);
  h_ND_NeutronThetaAngle_InElastic->GetXaxis()->SetTitleSize(0.05);
  h_ND_NeutronThetaAngle_InElastic->GetYaxis()->SetTitleSize(0.05);
  h_ND_NeutronThetaAngle_InElastic->DrawCopy();
  TCanvas *neutronInformationQuasiElastic = new TCanvas("neutronInformationQuasiElastic","QuasiElastic Events Neutron Diagnostics",800,900);
  neutronInformationQuasiElastic->Divide(2,3,0.0001, 0.00001,0);
  neutronInformationQuasiElastic->cd(1);
  h_ND_InitialNeutronMomentum_QuasiElastic->GetXaxis()->SetTitle("Initial Neutron Momentum (MeV/c)");
  h_ND_InitialNeutronMomentum_QuasiElastic->GetYaxis()->SetTitle("Neutron Count");
  h_ND_InitialNeutronMomentum_QuasiElastic->GetXaxis()->SetNdivisions(9);
  h_ND_InitialNeutronMomentum_QuasiElastic->GetXaxis()->SetTitleSize(0.05);
  h_ND_InitialNeutronMomentum_QuasiElastic->GetYaxis()->SetTitleSize(0.05);
  h_ND_InitialNeutronMomentum_QuasiElastic->DrawCopy();
  neutronInformationQuasiElastic->cd(2);
  h_ND_NeutronMomentum_QuasiElastic->GetXaxis()->SetTitle("Neutron Momentum (MeV/c)");
  h_ND_NeutronMomentum_QuasiElastic->GetYaxis()->SetTitle("Neutron Count");
  h_ND_NeutronMomentum_QuasiElastic->GetXaxis()->SetNdivisions(9);
  h_ND_NeutronMomentum_QuasiElastic->GetXaxis()->SetTitleSize(0.05);
  h_ND_NeutronMomentum_QuasiElastic->GetYaxis()->SetTitleSize(0.05);
  h_ND_NeutronMomentum_QuasiElastic->DrawCopy();
  neutronInformationQuasiElastic->cd(3);
  h_ND_InitialNeutronEnergy_QuasiElastic->GetXaxis()->SetTitle("Initial Neutron Energy (MeV)");
  h_ND_InitialNeutronEnergy_QuasiElastic->GetYaxis()->SetTitle("Neutron Count");
  h_ND_InitialNeutronEnergy_QuasiElastic->GetXaxis()->SetNdivisions(9);
  h_ND_InitialNeutronEnergy_QuasiElastic->GetXaxis()->SetTitleSize(0.05);
  h_ND_InitialNeutronEnergy_QuasiElastic->GetYaxis()->SetTitleSize(0.05);
  h_ND_InitialNeutronEnergy_QuasiElastic->DrawCopy();
  neutronInformationQuasiElastic->cd(4);
  h_ND_NeutronEnergy_QuasiElastic->GetXaxis()->SetTitle("Neutron Energy (MeV)");
  h_ND_NeutronEnergy_QuasiElastic->GetYaxis()->SetTitle("Neutron Count");
  h_ND_NeutronEnergy_QuasiElastic->GetXaxis()->SetNdivisions(9);
  h_ND_NeutronEnergy_QuasiElastic->GetXaxis()->SetTitleSize(0.05);
  h_ND_NeutronEnergy_QuasiElastic->GetYaxis()->SetTitleSize(0.05);
  h_ND_NeutronEnergy_QuasiElastic->DrawCopy();
  neutronInformationQuasiElastic->cd(5);
  h_ND_NeutronThetaAngle_QuasiElastic->GetXaxis()->SetTitle("Initial Neutron Angle (degrees)");
  h_ND_NeutronThetaAngle_QuasiElastic->GetYaxis()->SetTitle("Neutron Count");
  h_ND_NeutronThetaAngle_QuasiElastic->GetXaxis()->SetNdivisions(9);
  h_ND_NeutronThetaAngle_QuasiElastic->GetXaxis()->SetTitleSize(0.05);
  h_ND_NeutronThetaAngle_QuasiElastic->GetYaxis()->SetTitleSize(0.05);
  h_ND_NeutronThetaAngle_QuasiElastic->DrawCopy();

  TCanvas *NPeventElastic = new TCanvas("NPeventElastic","Plots for Real NP Elastic Events",1000,1200);
  NPeventElastic->Divide(3,2,0.0001, 0.00001,0);
  NPeventElastic->cd(1);
  h_NP_recoilAngle_Elastic->GetXaxis()->SetTitle("Proton Recoil Angle (deg for Elastic Events)");
  h_NP_recoilAngle_Elastic->GetYaxis()->SetTitle("Proton Count");
  h_NP_recoilAngle_Elastic->GetXaxis()->SetNdivisions(9);
  h_NP_recoilAngle_Elastic->SetTitleSize(0.05);
  h_NP_recoilAngle_Elastic->SetTitleSize(0.05);
  h_NP_recoilAngle_Elastic->DrawCopy();
  NPeventElastic->cd(2);
  h_NP_recoilEnergy_Elastic->GetXaxis()->SetTitle("Proton Recoil Energy (MeV)");
  h_NP_recoilEnergy_Elastic->GetYaxis()->SetTitle("Proton Count");
  h_NP_recoilEnergy_Elastic->GetXaxis()->SetNdivisions(9);
  h_NP_recoilEnergy_Elastic->SetTitleSize(0.05);
  h_NP_recoilEnergy_Elastic->SetTitleSize(0.05);
  h_NP_recoilEnergy_Elastic->DrawCopy();
  NPeventElastic->cd(3);
  h_NP_asymmetry_Elastic->GetXaxis()->SetTitle("Proton Recoil Asymmetry");
  h_NP_asymmetry_Elastic->GetYaxis()->SetTitle("Proton Count");
  h_NP_asymmetry_Elastic->GetXaxis()->SetNdivisions(4);
  h_NP_asymmetry_Elastic->SetTitleSize(0.05);
  h_NP_asymmetry_Elastic->SetTitleSize(0.05);
  h_NP_asymmetry_Elastic->DrawCopy();
  std::cout << "Real Up/Down asymmetry Elastic Events = " << (h_NP_asymmetry_Elastic->GetBinContent(4)/h_NP_asymmetry_Elastic->GetBinContent(2)) << std::endl;
  NPeventElastic->cd(4);
  h_NP_dEvsE_Elastic->GetXaxis()->SetTitle("E-Array Energy Deposited (MeV)");
  h_NP_dEvsE_Elastic->GetXaxis()->SetTitleSize(0.05);
  h_NP_dEvsE_Elastic->GetYaxis()->SetTitleSize(0.05);
  h_NP_dEvsE_Elastic->GetYaxis()->SetTitle("dE-Array Energy Deposited (MeV)");
  h_NP_dEvsE_Elastic->Rebin2D(2);
  h_NP_dEvsE_Elastic->DrawCopy("cont4");
  NPeventElastic->cd(5);
  h_NP_dEvsE2_Elastic->GetXaxis()->SetTitle("E-Array Energy Deposited (MeV)");
  h_NP_dEvsE2_Elastic->GetXaxis()->SetTitleSize(0.05);
  h_NP_dEvsE2_Elastic->GetYaxis()->SetTitleSize(0.05);
  h_NP_dEvsE2_Elastic->GetYaxis()->SetTitle("dE-Array Energy Deposited (MeV)");
  h_NP_dEvsE2_Elastic->Rebin2D(2);
  h_NP_dEvsE2_Elastic->DrawCopy("cont4");
  TCanvas *NPeventInElastic = new TCanvas("NPeventInElastic","Plots for Real NP InElastic Events",1000,1200);
  NPeventInElastic->Divide(3,2,0.0001, 0.00001,0);
  NPeventInElastic->cd(1);
  h_NP_recoilAngle_InElastic->GetXaxis()->SetTitle("Proton Recoil Angle (deg for InElastic Events)");
  h_NP_recoilAngle_InElastic->GetYaxis()->SetTitle("Proton Count");
  h_NP_recoilAngle_InElastic->GetXaxis()->SetNdivisions(9);
  h_NP_recoilAngle_InElastic->SetTitleSize(0.05);
  h_NP_recoilAngle_InElastic->SetTitleSize(0.05);
  h_NP_recoilAngle_InElastic->DrawCopy();
  NPeventInElastic->cd(2);
  h_NP_recoilEnergy_InElastic->GetXaxis()->SetTitle("Proton Recoil Energy (MeV)");
  h_NP_recoilEnergy_InElastic->GetYaxis()->SetTitle("Proton Count");
  h_NP_recoilEnergy_InElastic->GetXaxis()->SetNdivisions(9);
  h_NP_recoilEnergy_InElastic->SetTitleSize(0.05);
  h_NP_recoilEnergy_InElastic->SetTitleSize(0.05);
  h_NP_recoilEnergy_InElastic->DrawCopy();
  NPeventInElastic->cd(3);
  h_NP_asymmetry_InElastic->GetXaxis()->SetTitle("Proton Recoil Asymmetry");
  h_NP_asymmetry_InElastic->GetYaxis()->SetTitle("Proton Count");
  h_NP_asymmetry_InElastic->GetXaxis()->SetNdivisions(4);
  h_NP_asymmetry_InElastic->SetTitleSize(0.05);
  h_NP_asymmetry_InElastic->SetTitleSize(0.05);
  h_NP_asymmetry_InElastic->DrawCopy();
  std::cout << "Real Up/Down asymmetry InElastic Events = " << (h_NP_asymmetry_InElastic->GetBinContent(4)/h_NP_asymmetry_InElastic->GetBinContent(2)) << std::endl;
  NPeventInElastic->cd(4);
  h_NP_dEvsE_InElastic->GetXaxis()->SetTitle("E-Array Energy Deposited (MeV)");
  h_NP_dEvsE_InElastic->GetXaxis()->SetTitleSize(0.05);
  h_NP_dEvsE_InElastic->GetYaxis()->SetTitleSize(0.05);
  h_NP_dEvsE_InElastic->GetYaxis()->SetTitle("dE-Array Energy Deposited (MeV)");
  h_NP_dEvsE_InElastic->Rebin2D(2);
  h_NP_dEvsE_InElastic->DrawCopy("cont4");
  NPeventInElastic->cd(5);
  h_NP_dEvsE2_InElastic->GetXaxis()->SetTitle("E-Array Energy Deposited (MeV)");
  h_NP_dEvsE2_InElastic->GetXaxis()->SetTitleSize(0.05);
  h_NP_dEvsE2_InElastic->GetYaxis()->SetTitleSize(0.05);
  h_NP_dEvsE2_InElastic->GetYaxis()->SetTitle("dE-Array Energy Deposited (MeV)");
  h_NP_dEvsE2_InElastic->Rebin2D(2);
  h_NP_dEvsE2_InElastic->DrawCopy("cont4");
  TCanvas *NPeventQuasiElastic = new TCanvas("NPeventQuasiElastic","Plots for Real NP QuasiElastic Events",1000,1200);
  NPeventQuasiElastic->Divide(3,2,0.0001, 0.00001,0);
  NPeventQuasiElastic->cd(1);
  h_NP_recoilAngle_QuasiElastic->GetXaxis()->SetTitle("Proton Recoil Angle (deg for QuasiElastic Events)");
  h_NP_recoilAngle_QuasiElastic->GetYaxis()->SetTitle("Proton Count");
  h_NP_recoilAngle_QuasiElastic->GetXaxis()->SetNdivisions(9);
  h_NP_recoilAngle_QuasiElastic->SetTitleSize(0.05);
  h_NP_recoilAngle_QuasiElastic->SetTitleSize(0.05);
  h_NP_recoilAngle_QuasiElastic->DrawCopy();
  NPeventQuasiElastic->cd(2);
  h_NP_recoilEnergy_QuasiElastic->GetXaxis()->SetTitle("Proton Recoil Energy (MeV)");
  h_NP_recoilEnergy_QuasiElastic->GetYaxis()->SetTitle("Proton Count");
  h_NP_recoilEnergy_QuasiElastic->GetXaxis()->SetNdivisions(9);
  h_NP_recoilEnergy_QuasiElastic->SetTitleSize(0.05);
  h_NP_recoilEnergy_QuasiElastic->SetTitleSize(0.05);
  h_NP_recoilEnergy_QuasiElastic->DrawCopy();
  NPeventQuasiElastic->cd(3);
  h_NP_asymmetry_QuasiElastic->GetXaxis()->SetTitle("Proton Recoil Asymmetry");
  h_NP_asymmetry_QuasiElastic->GetYaxis()->SetTitle("Proton Count");
  h_NP_asymmetry_QuasiElastic->GetXaxis()->SetNdivisions(4);
  h_NP_asymmetry_QuasiElastic->SetTitleSize(0.05);
  h_NP_asymmetry_QuasiElastic->SetTitleSize(0.05);
  h_NP_asymmetry_QuasiElastic->DrawCopy();
  std::cout << "Real Up/Down asymmetry QuasiElastic Events = " << (h_NP_asymmetry_QuasiElastic->GetBinContent(4)/h_NP_asymmetry_QuasiElastic->GetBinContent(2)) << std::endl;
  NPeventQuasiElastic->cd(4);
  h_NP_dEvsE_QuasiElastic->GetXaxis()->SetTitle("E-Array Energy Deposited (MeV)");
  h_NP_dEvsE_QuasiElastic->GetXaxis()->SetTitleSize(0.05);
  h_NP_dEvsE_QuasiElastic->GetYaxis()->SetTitleSize(0.05);
  h_NP_dEvsE_QuasiElastic->GetYaxis()->SetTitle("dE-Array Energy Deposited (MeV)");
  h_NP_dEvsE_QuasiElastic->Rebin2D(2);
  h_NP_dEvsE_QuasiElastic->DrawCopy("cont4");
  NPeventQuasiElastic->cd(5);
  h_NP_dEvsE2_QuasiElastic->GetXaxis()->SetTitle("E-Array Energy Deposited (MeV)");
  h_NP_dEvsE2_QuasiElastic->GetXaxis()->SetTitleSize(0.05);
  h_NP_dEvsE2_QuasiElastic->GetYaxis()->SetTitleSize(0.05);
  h_NP_dEvsE2_QuasiElastic->GetYaxis()->SetTitle("dE-Array Energy Deposited (MeV)");
  h_NP_dEvsE2_QuasiElastic->Rebin2D(2);
  h_NP_dEvsE2_QuasiElastic->DrawCopy("cont4");

  TCanvas *angularPlotsElastic = new TCanvas("angularPlotsElastic","Elastic Events Proton Recoil Angle",800, 600);
  angularPlotsElastic->Divide(2,2,0.0001, 0.00001,0);
  angularPlotsElastic->cd(1);
  h_RA_recoilAngleRaw_Elastic->GetXaxis()->SetTitle("Recoil Proton Angle (degrees)");
  h_RA_recoilAngleRaw_Elastic->GetYaxis()->SetTitle("Recoil Proton Count");
  h_RA_recoilAngleRaw_Elastic->GetXaxis()->SetNdivisions(9);
  h_RA_recoilAngleRaw_Elastic->GetXaxis()->SetTitleSize(0.05);
  h_RA_recoilAngleRaw_Elastic->GetYaxis()->SetTitleSize(0.05);
  h_RA_recoilAngleRaw_Elastic->DrawCopy();
  angularPlotsElastic->cd(2);
  h_RA_recoilAngle_Elastic->GetXaxis()->SetTitle("Recoil Proton Angle (degrees)");
  h_RA_recoilAngle_Elastic->GetYaxis()->SetTitle("Recoil Proton Count");
  h_RA_recoilAngle_Elastic->GetXaxis()->SetNdivisions(9);
  h_RA_recoilAngle_Elastic->GetXaxis()->SetTitleSize(0.05);
  h_RA_recoilAngle_Elastic->GetYaxis()->SetTitleSize(0.05);
  h_RA_recoilAngle_Elastic->DrawCopy();
  angularPlotsElastic->cd(3);
  h_RA_totalEnergy_Elastic->GetXaxis()->SetTitle("Energy Deposited (MeV)");
  h_RA_totalEnergy_Elastic->GetYaxis()->SetTitle("Recoil Proton Count");
  h_RA_totalEnergy_Elastic->GetXaxis()->SetNdivisions(9);
  h_RA_totalEnergy_Elastic->GetXaxis()->SetTitleSize(0.05);
  h_RA_totalEnergy_Elastic->GetYaxis()->SetTitleSize(0.05);
  h_RA_totalEnergy_Elastic->DrawCopy();
  TCanvas *angularPlotsInElastic = new TCanvas("angularPlotsInElastic","InElastic Events Proton Recoil Angle",800, 600);
  angularPlotsInElastic->Divide(2,2,0.0001, 0.00001,0);
  angularPlotsInElastic->cd(1);
  h_RA_recoilAngleRaw_InElastic->GetXaxis()->SetTitle("Recoil Proton Angle (degrees)");
  h_RA_recoilAngleRaw_InElastic->GetYaxis()->SetTitle("Recoil Proton Count");
  h_RA_recoilAngleRaw_InElastic->GetXaxis()->SetNdivisions(9);
  h_RA_recoilAngleRaw_InElastic->GetXaxis()->SetTitleSize(0.05);
  h_RA_recoilAngleRaw_InElastic->GetYaxis()->SetTitleSize(0.05);
  h_RA_recoilAngleRaw_InElastic->DrawCopy();
  angularPlotsInElastic->cd(2);
  h_RA_recoilAngle_InElastic->GetXaxis()->SetTitle("Recoil Proton Angle (degrees)");
  h_RA_recoilAngle_InElastic->GetYaxis()->SetTitle("Recoil Proton Count");
  h_RA_recoilAngle_InElastic->GetXaxis()->SetNdivisions(9);
  h_RA_recoilAngle_InElastic->GetXaxis()->SetTitleSize(0.05);
  h_RA_recoilAngle_InElastic->GetYaxis()->SetTitleSize(0.05);
  h_RA_recoilAngle_InElastic->DrawCopy();
  angularPlotsInElastic->cd(3);
  h_RA_totalEnergy_InElastic->GetXaxis()->SetTitle("Energy Deposited (MeV)");
  h_RA_totalEnergy_InElastic->GetYaxis()->SetTitle("Recoil Proton Count");
  h_RA_totalEnergy_InElastic->GetXaxis()->SetNdivisions(9);
  h_RA_totalEnergy_InElastic->GetXaxis()->SetTitleSize(0.05);
  h_RA_totalEnergy_InElastic->GetYaxis()->SetTitleSize(0.05);
  h_RA_totalEnergy_InElastic->DrawCopy();
  TCanvas *angularPlotsQuasiElastic = new TCanvas("angularPlotsQuasiElastic","QuasiElastic Events Proton Recoil Angle",800, 600);
  angularPlotsQuasiElastic->Divide(2,2,0.0001, 0.00001,0);
  angularPlotsQuasiElastic->cd(1);
  h_RA_recoilAngleRaw_QuasiElastic->GetXaxis()->SetTitle("Recoil Proton Angle (degrees)");
  h_RA_recoilAngleRaw_QuasiElastic->GetYaxis()->SetTitle("Recoil Proton Count");
  h_RA_recoilAngleRaw_QuasiElastic->GetXaxis()->SetNdivisions(9);
  h_RA_recoilAngleRaw_QuasiElastic->GetXaxis()->SetTitleSize(0.05);
  h_RA_recoilAngleRaw_QuasiElastic->GetYaxis()->SetTitleSize(0.05);
  h_RA_recoilAngleRaw_QuasiElastic->DrawCopy();
  angularPlotsQuasiElastic->cd(2);
  h_RA_recoilAngle_QuasiElastic->GetXaxis()->SetTitle("Recoil Proton Angle (degrees)");
  h_RA_recoilAngle_QuasiElastic->GetYaxis()->SetTitle("Recoil Proton Count");
  h_RA_recoilAngle_QuasiElastic->GetXaxis()->SetNdivisions(9);
  h_RA_recoilAngle_QuasiElastic->GetXaxis()->SetTitleSize(0.05);
  h_RA_recoilAngle_QuasiElastic->GetYaxis()->SetTitleSize(0.05);
  h_RA_recoilAngle_QuasiElastic->DrawCopy();
  angularPlotsQuasiElastic->cd(3);
  h_RA_totalEnergy_QuasiElastic->GetXaxis()->SetTitle("Energy Deposited (MeV)");
  h_RA_totalEnergy_QuasiElastic->GetYaxis()->SetTitle("Recoil Proton Count");
  h_RA_totalEnergy_QuasiElastic->GetXaxis()->SetNdivisions(9);
  h_RA_totalEnergy_QuasiElastic->GetXaxis()->SetTitleSize(0.05);
  h_RA_totalEnergy_QuasiElastic->GetYaxis()->SetTitleSize(0.05);
  h_RA_totalEnergy_QuasiElastic->DrawCopy();
  
  TCanvas *dEvsENPeventElastic = new TCanvas("dEvsENPeventElastic","dE vs. E for Real NP Event (Elastic Events)",1000,1200);
  dEvsENPeventElastic->Divide(1,1,0.0001, 0.00001,0);
  dEvsENPeventElastic->cd(1);
  h_NP_dEvsE_Elastic->GetXaxis()->SetTitle("E-Array Energy Deposited (MeV)");
  h_NP_dEvsE_Elastic->GetXaxis()->SetTitleSize(0.03);
  h_NP_dEvsE_Elastic->GetYaxis()->SetTitleSize(0.03);
  h_NP_dEvsE_Elastic->GetYaxis()->SetTitle("dE-Array Energy Deposited (MeV)");
  //h_NP_dEvsE_InElastic->Rebin2D(2);
  h_NP_dEvsE_Elastic->DrawCopy("cont4");
  TCanvas *dEvsENPeventInElastic = new TCanvas("dEvsENPeventInElastic","dE vs. E for Real NP Event (InElastic Events)",1000,1200);
  dEvsENPeventInElastic->Divide(1,1,0.0001, 0.00001,0);
  dEvsENPeventInElastic->cd(1);
  h_NP_dEvsE_InElastic->GetXaxis()->SetTitle("E-Array Energy Deposited (MeV)");
  h_NP_dEvsE_InElastic->GetXaxis()->SetTitleSize(0.03);
  h_NP_dEvsE_InElastic->GetYaxis()->SetTitleSize(0.03);
  h_NP_dEvsE_InElastic->GetYaxis()->SetTitle("dE-Array Energy Deposited (MeV)");
  //h_NP_dEvsE_InElastic->Rebin2D(2);
  h_NP_dEvsE_InElastic->DrawCopy("cont4");
  TCanvas *dEvsENPeventQuasiElastic = new TCanvas("dEvsENPeventQuasiElastic","dE vs. E for Real NP Event (QuasiElastic Events)",1000,1200);
  dEvsENPeventQuasiElastic->Divide(1,1,0.0001, 0.00001,0);
  dEvsENPeventQuasiElastic->cd(1);
  h_NP_dEvsE_QuasiElastic->GetXaxis()->SetTitle("E-Array Energy Deposited (MeV)");
  h_NP_dEvsE_QuasiElastic->GetXaxis()->SetTitleSize(0.03);
  h_NP_dEvsE_QuasiElastic->GetYaxis()->SetTitleSize(0.03);
  h_NP_dEvsE_QuasiElastic->GetYaxis()->SetTitle("dE-Array Energy Deposited (MeV)");
  //h_NP_dEvsE_InElastic->Rebin2D(2);
  h_NP_dEvsE_QuasiElastic->DrawCopy("cont4");

  TCanvas *dEoverE_Elastic = new TCanvas("dEoverE_Elastic","dE Over E - Elastic Events",1000, 800);
  dEoverE_Elastic->Divide(2,2,0.0001,0.00001,0);
  dEoverE_Elastic->cd(1);
  h_RA_dEoverEtop_Elastic->GetXaxis()->SetTitle("E-Array Energy Deposited (MeV)");
  h_RA_dEoverEtop_Elastic->GetXaxis()->SetTitleSize(0.05);
  h_RA_dEoverEtop_Elastic->GetYaxis()->SetTitleSize(0.05);
  h_RA_dEoverEtop_Elastic->GetYaxis()->SetTitle("dE-Array Energy Deposited (MeV)");
  //h_RA_dEoverEtop_Elastic->Rebin2D(2);
  h_RA_dEoverEtop_Elastic->DrawCopy("cont4");
  dEoverE_Elastic->cd(2);
  h_RA_dEoverEbot_Elastic->GetXaxis()->SetTitle("E-Array Energy Deposited (MeV)");
  h_RA_dEoverEbot_Elastic->GetXaxis()->SetTitleSize(0.05);
  h_RA_dEoverEbot_Elastic->GetYaxis()->SetTitleSize(0.05);
  h_RA_dEoverEbot_Elastic->GetYaxis()->SetTitle("dE-Array Energy Deposited (MeV)");
  //h_RA_dEoverEbot_Elastic->Rebin2D(2);
  h_RA_dEoverEbot_Elastic->DrawCopy("cont4");
  dEoverE_Elastic->cd(3);
  h_RA_dEoverEBoth_Elastic->GetXaxis()->SetTitle("E-Array Energy Deposited (MeV)");
  h_RA_dEoverEBoth_Elastic->GetXaxis()->SetTitleSize(0.05);
  h_RA_dEoverEBoth_Elastic->GetYaxis()->SetTitleSize(0.05);
  h_RA_dEoverEBoth_Elastic->GetYaxis()->SetTitle("dE-Array Energy Deposited (MeV)");
  h_RA_dEoverEBoth_Elastic->Rebin2D(2);
  h_RA_dEoverEBoth_Elastic->DrawCopy("cont4");
  TCanvas *dEoverE_InElastic = new TCanvas("dEoverE_InElastic","dE Over E - InElastic Events",1000, 800);
  dEoverE_InElastic->Divide(2,2,0.0001,0.00001,0);
  dEoverE_InElastic->cd(1);
  h_RA_dEoverEtop_InElastic->GetXaxis()->SetTitle("E-Array Energy Deposited (MeV)");
  h_RA_dEoverEtop_InElastic->GetXaxis()->SetTitleSize(0.05);
  h_RA_dEoverEtop_InElastic->GetYaxis()->SetTitleSize(0.05);
  h_RA_dEoverEtop_InElastic->GetYaxis()->SetTitle("dE-Array Energy Deposited (MeV)");
  //h_RA_dEoverEtop_InElastic->Rebin2D(2);
  h_RA_dEoverEtop_InElastic->DrawCopy("cont4");
  dEoverE_InElastic->cd(2);
  h_RA_dEoverEbot_InElastic->GetXaxis()->SetTitle("E-Array Energy Deposited (MeV)");
  h_RA_dEoverEbot_InElastic->GetXaxis()->SetTitleSize(0.05);
  h_RA_dEoverEbot_InElastic->GetYaxis()->SetTitleSize(0.05);
  h_RA_dEoverEbot_InElastic->GetYaxis()->SetTitle("dE-Array Energy Deposited (MeV)");
  //h_RA_dEoverEbot_InElastic->Rebin2D(2);
  h_RA_dEoverEbot_InElastic->DrawCopy("cont4");
  dEoverE_InElastic->cd(3);
  h_RA_dEoverEBoth_InElastic->GetXaxis()->SetTitle("E-Array Energy Deposited (MeV)");
  h_RA_dEoverEBoth_InElastic->GetXaxis()->SetTitleSize(0.05);
  h_RA_dEoverEBoth_InElastic->GetYaxis()->SetTitleSize(0.05);
  h_RA_dEoverEBoth_InElastic->GetYaxis()->SetTitle("dE-Array Energy Deposited (MeV)");
  h_RA_dEoverEBoth_InElastic->Rebin2D(2);
  h_RA_dEoverEBoth_InElastic->DrawCopy("cont4");  
  TCanvas *dEoverE_QuasiElastic = new TCanvas("dEoverE_QuasiElastic","dE Over E - QuasiElastic Events",1000, 800);
  dEoverE_QuasiElastic->Divide(2,2,0.0001,0.00001,0);
  dEoverE_QuasiElastic->cd(1);
  h_RA_dEoverEtop_QuasiElastic->GetXaxis()->SetTitle("E-Array Energy Deposited (MeV)");
  h_RA_dEoverEtop_QuasiElastic->GetXaxis()->SetTitleSize(0.05);
  h_RA_dEoverEtop_QuasiElastic->GetYaxis()->SetTitleSize(0.05);
  h_RA_dEoverEtop_QuasiElastic->GetYaxis()->SetTitle("dE-Array Energy Deposited (MeV)");
  //h_RA_dEoverEtop_QuasiElastic->Rebin2D(2);
  h_RA_dEoverEtop_QuasiElastic->DrawCopy("cont4");
  dEoverE_QuasiElastic->cd(2);
  h_RA_dEoverEbot_QuasiElastic->GetXaxis()->SetTitle("E-Array Energy Deposited (MeV)");
  h_RA_dEoverEbot_QuasiElastic->GetXaxis()->SetTitleSize(0.05);
  h_RA_dEoverEbot_QuasiElastic->GetYaxis()->SetTitleSize(0.05);
  h_RA_dEoverEbot_QuasiElastic->GetYaxis()->SetTitle("dE-Array Energy Deposited (MeV)");
  //h_RA_dEoverEbot_QuasiElastic->Rebin2D(2);
  h_RA_dEoverEbot_QuasiElastic->DrawCopy("cont4");
  dEoverE_QuasiElastic->cd(3);
  h_RA_dEoverEBoth_QuasiElastic->GetXaxis()->SetTitle("E-Array Energy Deposited (MeV)");
  h_RA_dEoverEBoth_QuasiElastic->GetXaxis()->SetTitleSize(0.05);
  h_RA_dEoverEBoth_QuasiElastic->GetYaxis()->SetTitleSize(0.05);
  h_RA_dEoverEBoth_QuasiElastic->GetYaxis()->SetTitle("dE-Array Energy Deposited (MeV)");
  h_RA_dEoverEBoth_QuasiElastic->Rebin2D(2);
  h_RA_dEoverEBoth_QuasiElastic->DrawCopy("cont4");

  TCanvas *dToF = new TCanvas("dToF","Delta Time-of-Flight from Analyzer to dE-Array",1000, 800);
  dToF->Divide(2,2,0.0001,0.00001,0);
  dToF->cd(1);
  h_RA_dTOF_Elastic->GetXaxis()->SetTitle("Time (ns)");
  h_RA_dTOF_Elastic->GetXaxis()->SetTitleSize(0.05);
  h_RA_dTOF_Elastic->GetYaxis()->SetTitleSize(0.05);
  h_RA_dTOF_Elastic->GetYaxis()->SetTitle("Number of Events");
  h_RA_dTOF_Elastic->SetFillColor(kRed);
  h_RA_dTOF_Elastic->DrawCopy();
  dToF->cd(2);
  h_RA_dTOF_InElastic->GetXaxis()->SetTitle("Time (ns)");
  h_RA_dTOF_InElastic->GetXaxis()->SetTitleSize(0.05);
  h_RA_dTOF_InElastic->GetYaxis()->SetTitleSize(0.05);
  h_RA_dTOF_InElastic->GetYaxis()->SetTitle("Number of Events");
  h_RA_dTOF_InElastic->SetFillColor(kYellow);
  h_RA_dTOF_InElastic->DrawCopy();
  dToF->cd(3);
  h_RA_dTOF_QuasiElastic->GetXaxis()->SetTitle("Time (ns)");
  h_RA_dTOF_QuasiElastic->GetXaxis()->SetTitleSize(0.05);
  h_RA_dTOF_QuasiElastic->GetYaxis()->SetTitleSize(0.05);
  h_RA_dTOF_QuasiElastic->GetYaxis()->SetTitle("Number of Events");
  h_RA_dTOF_QuasiElastic->SetFillColor(kBlue);
  h_RA_dTOF_QuasiElastic->DrawCopy();
  dToF->cd(4);
  h_RA_dTOF_InElastic->GetXaxis()->SetTitle("Time (ns)");
  h_RA_dTOF_InElastic->GetXaxis()->SetTitleSize(0.05);
  h_RA_dTOF_InElastic->GetYaxis()->SetTitleSize(0.05);
  h_RA_dTOF_InElastic->GetYaxis()->SetTitle("Number of Events");
  h_RA_dTOF_InElastic->SetTitle("Delta Time-of-Flight (All Events)");
  h_RA_dTOF_InElastic->SetFillColor(kYellow);
  h_RA_dTOF_InElastic->DrawCopy();
  h_RA_dTOF_Elastic->SetFillColor(kRed);
  //h_RA_dTOF_Elastic->SetLineColor(kRed);
  h_RA_dTOF_Elastic->DrawCopy("same");
  h_RA_dTOF_QuasiElastic->SetFillColor(kBlue);
  h_RA_dTOF_QuasiElastic->DrawCopy("same");
  
  TCanvas *recoilMomentum = new TCanvas("RecoilMomentumAnalysis","Recoil Momentum (P_leading - P_elastic)", 1000, 1200);
  recoilMomentum->Divide(2,2,0.0001, 0.00001,0);
  recoilMomentum->cd(1);
  h_NP_RecoilMomentum_Elastic->GetXaxis()->SetTitle("(P_leading - P_elastic) (MeV/c)");
  h_NP_RecoilMomentum_Elastic->GetYaxis()->SetTitle("Counts");
  h_NP_RecoilMomentum_Elastic->GetXaxis()->SetTitleSize(0.05);
  h_NP_RecoilMomentum_Elastic->GetYaxis()->SetTitleSize(0.05);
  h_NP_RecoilMomentum_Elastic->SetTitle("Momentum Difference (Elastic Events)");
  h_NP_RecoilMomentum_Elastic->SetFillColor(kRed);
  h_NP_RecoilMomentum_Elastic->SetLineColor(kRed);
  h_NP_RecoilMomentum_Elastic->DrawCopy();
  recoilMomentum->cd(2);
  h_NP_RecoilMomentum_InElastic->GetXaxis()->SetTitle("(P_leading - P_elastic) (MeV/c)");
  h_NP_RecoilMomentum_InElastic->GetYaxis()->SetTitle("Counts");
  h_NP_RecoilMomentum_InElastic->GetXaxis()->SetTitleSize(0.05);
  h_NP_RecoilMomentum_InElastic->GetYaxis()->SetTitleSize(0.05);
  h_NP_RecoilMomentum_InElastic->SetTitle("Momentum Difference (InElastic Events)");
  h_NP_RecoilMomentum_InElastic->SetFillColor(kYellow);
  h_NP_RecoilMomentum_InElastic->DrawCopy();
  recoilMomentum->cd(3);
  h_NP_RecoilMomentum_QuasiElastic->GetXaxis()->SetTitle("(P_leading - P_elastic) (MeV/c)");
  h_NP_RecoilMomentum_QuasiElastic->GetYaxis()->SetTitle("Counts");
  h_NP_RecoilMomentum_QuasiElastic->GetXaxis()->SetTitleSize(0.05);
  h_NP_RecoilMomentum_QuasiElastic->GetYaxis()->SetTitleSize(0.05);
  h_NP_RecoilMomentum_QuasiElastic->SetTitle("Momentum Difference (QuasiElastic Events)");
  h_NP_RecoilMomentum_QuasiElastic->SetFillColor(kBlue);
  h_NP_RecoilMomentum_QuasiElastic->DrawCopy();
  recoilMomentum->cd(4);
  h_NP_RecoilMomentum_InElastic->GetXaxis()->SetTitle("(P_leading - P_elastic) (MeV/c)");
  h_NP_RecoilMomentum_InElastic->GetYaxis()->SetTitle("Counts");
  h_NP_RecoilMomentum_InElastic->GetXaxis()->SetTitleSize(0.05);
  h_NP_RecoilMomentum_InElastic->GetYaxis()->SetTitleSize(0.05);
  h_NP_RecoilMomentum_InElastic->SetTitle("Momentum Difference (All Events)");
  h_NP_RecoilMomentum_InElastic->SetFillColor(kYellow);
  int yVal = h_NP_RecoilMomentum_Elastic->GetBinContent(h_NP_RecoilMomentum_Elastic->GetMaximumBin());
  yVal = vertScaleVal(yVal);
  //yVal = (yVal/1000 + 1)*1000;
  h_NP_RecoilMomentum_InElastic->GetYaxis()->SetRangeUser(0,yVal);
  h_NP_RecoilMomentum_InElastic->DrawCopy();
  h_NP_RecoilMomentum_QuasiElastic->SetFillColor(kBlue);
  h_NP_RecoilMomentum_QuasiElastic->DrawCopy("same");
  h_NP_RecoilMomentum_Elastic->SetFillColor(kRed);
  h_NP_RecoilMomentum_Elastic->SetLineColor(kRed);
  h_NP_RecoilMomentum_Elastic->DrawCopy("same");
  

  std::cout << std::endl << "Elastic Efficiencies" << std::endl;
  TCanvas *sectionEff_ElasticEvents = new TCanvas("sectionEff_ElasticEvents","Section Efficiency - Elastic Events",800, 800);
  // Section Efficiencies
  sectionEff_ElasticEvents->Divide(2,2,0.002,0.01,0);
  sectionEff_ElasticEvents->cd(1);
  h_RA_section1Efficiency_Elastic->SetOption("bar");
  h_RA_section1Efficiency_Elastic->GetXaxis()->SetTitle("Section of Polarimeter");
  h_RA_section1Efficiency_Elastic->GetXaxis()->SetRangeUser(0,5);
  h_RA_section1Efficiency_Elastic->GetXaxis()->SetNdivisions(9);
  h_RA_section1Efficiency_Elastic->GetXaxis()->SetTitleSize(0.05);
  h_RA_section1Efficiency_Elastic->GetYaxis()->SetTitleSize(0.05);
  h_RA_section1Efficiency_Elastic->GetYaxis()->SetTitle("Percent Efficiency");
  h_RA_section1Efficiency_Elastic->SetFillColor(kBlue);
  h_RA_section1Efficiency_Elastic->Scale(100/eventInteraction);
  h_RA_section1Efficiency_Elastic->DrawCopy();
  std::cout << "The Total " << h_RA_section1Efficiency_Elastic->GetTitle() << " is " << h_RA_section1Efficiency_Elastic->Integral() << "." << std::endl;
  sectionEff_ElasticEvents->cd(2);
  h_RA_section2Efficiency_Elastic->SetOption("bar");
  h_RA_section2Efficiency_Elastic->GetXaxis()->SetTitle("Section of Polarimeter");
  h_RA_section2Efficiency_Elastic->GetXaxis()->SetRangeUser(0,5);
  h_RA_section2Efficiency_Elastic->GetXaxis()->SetNdivisions(9);
  h_RA_section2Efficiency_Elastic->GetXaxis()->SetTitleSize(0.05);
  h_RA_section2Efficiency_Elastic->GetYaxis()->SetTitleSize(0.05);
  h_RA_section2Efficiency_Elastic->GetYaxis()->SetTitle("Percent Efficiency");
  h_RA_section2Efficiency_Elastic->SetFillColor(kBlue);
  h_RA_section2Efficiency_Elastic->Scale(100/eventInteraction);
  h_RA_section2Efficiency_Elastic->DrawCopy();
  std::cout << "The Total " << h_RA_section2Efficiency_Elastic->GetTitle() << " is " << h_RA_section2Efficiency_Elastic->Integral() << "." <<  std::endl;
  sectionEff_ElasticEvents->cd(3);
  h_RA_section3Efficiency_Elastic->SetOption("bar");
  h_RA_section3Efficiency_Elastic->GetXaxis()->SetTitle("Section of Polarimeter");
  h_RA_section3Efficiency_Elastic->GetXaxis()->SetRangeUser(0,5);
  h_RA_section3Efficiency_Elastic->GetXaxis()->SetNdivisions(9);
  h_RA_section3Efficiency_Elastic->GetXaxis()->SetTitleSize(0.05);
  h_RA_section3Efficiency_Elastic->GetYaxis()->SetTitleSize(0.05);
  h_RA_section3Efficiency_Elastic->GetYaxis()->SetTitle("Percent Efficiency");
  h_RA_section3Efficiency_Elastic->SetFillColor(kBlue);
  h_RA_section3Efficiency_Elastic->Scale(100/eventInteraction);
  h_RA_section3Efficiency_Elastic->DrawCopy();
  std::cout << "The Total " << h_RA_section3Efficiency_Elastic->GetTitle() << " is " << h_RA_section3Efficiency_Elastic->Integral() << "." << std::endl;
  sectionEff_ElasticEvents->cd(4);
  h_RA_section4Efficiency_Elastic->SetOption("bar");
  h_RA_section4Efficiency_Elastic->GetXaxis()->SetTitle("Section of Polarimeter");
  h_RA_section4Efficiency_Elastic->GetXaxis()->SetRangeUser(0,5);
  h_RA_section4Efficiency_Elastic->GetXaxis()->SetNdivisions(9);
  h_RA_section4Efficiency_Elastic->GetXaxis()->SetTitleSize(0.05);
  h_RA_section4Efficiency_Elastic->GetYaxis()->SetTitleSize(0.05);
  h_RA_section4Efficiency_Elastic->GetYaxis()->SetTitle("Percent Efficiency");
  h_RA_section4Efficiency_Elastic->SetFillColor(kBlue);
  h_RA_section4Efficiency_Elastic->Scale(100/eventInteraction);
  h_RA_section4Efficiency_Elastic->DrawCopy();
  std::cout << "The Total " << h_RA_section4Efficiency_Elastic->GetTitle() << " is " << h_RA_section4Efficiency_Elastic->Integral() << "." << std::endl;
  std::cout << std::endl << "InElastic Efficiencies" << std::endl;
  TCanvas *sectionEff_InElasticEvents = new TCanvas("sectionEff_InElasticEvents","Section Efficiency - InElastic Events",800, 800);
  // Section Efficiencies
  sectionEff_InElasticEvents->Divide(2,2,0.002,0.01,0);
  sectionEff_InElasticEvents->cd(1);
  h_RA_section1Efficiency_InElastic->SetOption("bar");
  h_RA_section1Efficiency_InElastic->GetXaxis()->SetTitle("Section of Polarimeter");
  h_RA_section1Efficiency_InElastic->GetXaxis()->SetRangeUser(0,5);
  h_RA_section1Efficiency_InElastic->GetXaxis()->SetNdivisions(9);
  h_RA_section1Efficiency_InElastic->GetXaxis()->SetTitleSize(0.05);
  h_RA_section1Efficiency_InElastic->GetYaxis()->SetTitleSize(0.05);
  h_RA_section1Efficiency_InElastic->GetYaxis()->SetTitle("Percent Efficiency");
  h_RA_section1Efficiency_InElastic->SetFillColor(kBlue);
  h_RA_section1Efficiency_InElastic->Scale(100/eventInteraction);
  h_RA_section1Efficiency_InElastic->DrawCopy();
  std::cout << "The Total " << h_RA_section1Efficiency_InElastic->GetTitle() << " is " << h_RA_section1Efficiency_InElastic->Integral() << "." << std::endl;
  sectionEff_InElasticEvents->cd(2);
  h_RA_section2Efficiency_InElastic->SetOption("bar");
  h_RA_section2Efficiency_InElastic->GetXaxis()->SetTitle("Section of Polarimeter");
  h_RA_section2Efficiency_InElastic->GetXaxis()->SetRangeUser(0,5);
  h_RA_section2Efficiency_InElastic->GetXaxis()->SetNdivisions(9);
  h_RA_section2Efficiency_InElastic->GetXaxis()->SetTitleSize(0.05);
  h_RA_section2Efficiency_InElastic->GetYaxis()->SetTitleSize(0.05);
  h_RA_section2Efficiency_InElastic->GetYaxis()->SetTitle("Percent Efficiency");
  h_RA_section2Efficiency_InElastic->SetFillColor(kBlue);
  h_RA_section2Efficiency_InElastic->Scale(100/eventInteraction);
  h_RA_section2Efficiency_InElastic->DrawCopy();
  std::cout << "The Total " << h_RA_section2Efficiency_InElastic->GetTitle() << " is " << h_RA_section2Efficiency_InElastic->Integral() << "." <<  std::endl;
  sectionEff_InElasticEvents->cd(3);
  h_RA_section3Efficiency_InElastic->SetOption("bar");
  h_RA_section3Efficiency_InElastic->GetXaxis()->SetTitle("Section of Polarimeter");
  h_RA_section3Efficiency_InElastic->GetXaxis()->SetRangeUser(0,5);
  h_RA_section3Efficiency_InElastic->GetXaxis()->SetNdivisions(9);
  h_RA_section3Efficiency_InElastic->GetXaxis()->SetTitleSize(0.05);
  h_RA_section3Efficiency_InElastic->GetYaxis()->SetTitleSize(0.05);
  h_RA_section3Efficiency_InElastic->GetYaxis()->SetTitle("Percent Efficiency");
  h_RA_section3Efficiency_InElastic->SetFillColor(kBlue);
  h_RA_section3Efficiency_InElastic->Scale(100/eventInteraction);
  h_RA_section3Efficiency_InElastic->DrawCopy();
  std::cout << "The Total " << h_RA_section3Efficiency_InElastic->GetTitle() << " is " << h_RA_section3Efficiency_InElastic->Integral() << "." << std::endl;
  sectionEff_InElasticEvents->cd(4);
  h_RA_section4Efficiency_InElastic->SetOption("bar");
  h_RA_section4Efficiency_InElastic->GetXaxis()->SetTitle("Section of Polarimeter");
  h_RA_section4Efficiency_InElastic->GetXaxis()->SetRangeUser(0,5);
  h_RA_section4Efficiency_InElastic->GetXaxis()->SetNdivisions(9);
  h_RA_section4Efficiency_InElastic->GetXaxis()->SetTitleSize(0.05);
  h_RA_section4Efficiency_InElastic->GetYaxis()->SetTitleSize(0.05);
  h_RA_section4Efficiency_InElastic->GetYaxis()->SetTitle("Percent Efficiency");
  h_RA_section4Efficiency_InElastic->SetFillColor(kBlue);
  h_RA_section4Efficiency_InElastic->Scale(100/eventInteraction);
  h_RA_section4Efficiency_InElastic->DrawCopy();
  std::cout << "The Total " << h_RA_section4Efficiency_InElastic->GetTitle() << " is " << h_RA_section4Efficiency_InElastic->Integral() << "." << std::endl;
  std::cout << std::endl << "QuasiElastic Efficiencies" << std::endl;
  TCanvas *sectionEff_QuasiElasticEvents = new TCanvas("sectionEff_QuasiElasticEvents","Section Efficiency - QuasiElastic Events",800, 800);
  // Section Efficiencies
  sectionEff_QuasiElasticEvents->Divide(2,2,0.002,0.01,0);
  sectionEff_QuasiElasticEvents->cd(1);
  h_RA_section1Efficiency_QuasiElastic->SetOption("bar");
  h_RA_section1Efficiency_QuasiElastic->GetXaxis()->SetTitle("Section of Polarimeter");
  h_RA_section1Efficiency_QuasiElastic->GetXaxis()->SetRangeUser(0,5);
  h_RA_section1Efficiency_QuasiElastic->GetXaxis()->SetNdivisions(9);
  h_RA_section1Efficiency_QuasiElastic->GetXaxis()->SetTitleSize(0.05);
  h_RA_section1Efficiency_QuasiElastic->GetYaxis()->SetTitleSize(0.05);
  h_RA_section1Efficiency_QuasiElastic->GetYaxis()->SetTitle("Percent Efficiency");
  h_RA_section1Efficiency_QuasiElastic->SetFillColor(kBlue);
  h_RA_section1Efficiency_QuasiElastic->Scale(100/eventInteraction);
  h_RA_section1Efficiency_QuasiElastic->DrawCopy();
  std::cout << "The Total " << h_RA_section1Efficiency_QuasiElastic->GetTitle() << " is " << h_RA_section1Efficiency_QuasiElastic->Integral() << "." << std::endl;
  sectionEff_QuasiElasticEvents->cd(2);
  h_RA_section2Efficiency_QuasiElastic->SetOption("bar");
  h_RA_section2Efficiency_QuasiElastic->GetXaxis()->SetTitle("Section of Polarimeter");
  h_RA_section2Efficiency_QuasiElastic->GetXaxis()->SetRangeUser(0,5);
  h_RA_section2Efficiency_QuasiElastic->GetXaxis()->SetNdivisions(9);
  h_RA_section2Efficiency_QuasiElastic->GetXaxis()->SetTitleSize(0.05);
  h_RA_section2Efficiency_QuasiElastic->GetYaxis()->SetTitleSize(0.05);
  h_RA_section2Efficiency_QuasiElastic->GetYaxis()->SetTitle("Percent Efficiency");
  h_RA_section2Efficiency_QuasiElastic->SetFillColor(kBlue);
  h_RA_section2Efficiency_QuasiElastic->Scale(100/eventInteraction);
  h_RA_section2Efficiency_QuasiElastic->DrawCopy();
  std::cout << "The Total " << h_RA_section2Efficiency_QuasiElastic->GetTitle() << " is " << h_RA_section2Efficiency_QuasiElastic->Integral() << "." <<  std::endl;
  sectionEff_QuasiElasticEvents->cd(3);
  h_RA_section3Efficiency_QuasiElastic->SetOption("bar");
  h_RA_section3Efficiency_QuasiElastic->GetXaxis()->SetTitle("Section of Polarimeter");
  h_RA_section3Efficiency_QuasiElastic->GetXaxis()->SetRangeUser(0,5);
  h_RA_section3Efficiency_QuasiElastic->GetXaxis()->SetNdivisions(9);
  h_RA_section3Efficiency_QuasiElastic->GetXaxis()->SetTitleSize(0.05);
  h_RA_section3Efficiency_QuasiElastic->GetYaxis()->SetTitleSize(0.05);
  h_RA_section3Efficiency_QuasiElastic->GetYaxis()->SetTitle("Percent Efficiency");
  h_RA_section3Efficiency_QuasiElastic->SetFillColor(kBlue);
  h_RA_section3Efficiency_QuasiElastic->Scale(100/eventInteraction);
  h_RA_section3Efficiency_QuasiElastic->DrawCopy();
  std::cout << "The Total " << h_RA_section3Efficiency_QuasiElastic->GetTitle() << " is " << h_RA_section3Efficiency_QuasiElastic->Integral() << "." << std::endl;
  sectionEff_QuasiElasticEvents->cd(4);
  h_RA_section4Efficiency_QuasiElastic->SetOption("bar");
  h_RA_section4Efficiency_QuasiElastic->GetXaxis()->SetTitle("Section of Polarimeter");
  h_RA_section4Efficiency_QuasiElastic->GetXaxis()->SetRangeUser(0,5);
  h_RA_section4Efficiency_QuasiElastic->GetXaxis()->SetNdivisions(9);
  h_RA_section4Efficiency_QuasiElastic->GetXaxis()->SetTitleSize(0.05);
  h_RA_section4Efficiency_QuasiElastic->GetYaxis()->SetTitleSize(0.05);
  h_RA_section4Efficiency_QuasiElastic->GetYaxis()->SetTitle("Percent Efficiency");
  h_RA_section4Efficiency_QuasiElastic->SetFillColor(kBlue);
  h_RA_section4Efficiency_QuasiElastic->Scale(100/eventInteraction);
  h_RA_section4Efficiency_QuasiElastic->DrawCopy();
  std::cout << "The Total " << h_RA_section4Efficiency_QuasiElastic->GetTitle() << " is " << h_RA_section4Efficiency_QuasiElastic->Integral() << "." << std::endl;
  
  // Write out canvases to file
  neutronInformationElastic->Write();
  neutronInformationInElastic->Write();
  neutronInformationQuasiElastic->Write();
  NPeventElastic->Write();
  NPeventInElastic->Write();
  NPeventQuasiElastic->Write();
  angularPlotsElastic->Write();
  angularPlotsInElastic->Write();
  angularPlotsQuasiElastic->Write();
  dEvsENPeventElastic->Write();
  dEvsENPeventInElastic->Write();
  dEvsENPeventQuasiElastic->Write();
  dEoverE_Elastic->Write();
  dEoverE_InElastic->Write();
  dEoverE_QuasiElastic->Write();
  dToF->Write();
  recoilMomentum->Write();
  sectionEff_ElasticEvents->Write();
  sectionEff_InElasticEvents->Write();
  sectionEff_QuasiElasticEvents->Write();
  // Close files
  outFile->Close();
  sourceNeutron->Close();
  
}

TString FormInputFile(TString InputDir){
  
  TString fileName = InputDir + "/" + BaseName + "_NpolEff.root";
  
  return fileName;
}

TString FormOutputFile(TString OutputDir){
  
  TString fileName =  OutputDir + "/" + BaseName + "_efficiencyHistograms.root";
  
  return fileName;
}

void RetrieveENVvariables() {

  if(getenv("CHARGE_TYPE")){
	CType = getenv("CHARGE_TYPE");
  }else{
	CType = "All";
  }

  if(getenv("JOBNUMBER")){
	JobNum = getenv("JOBNUMBER");
  }else{
    JobNum = "99999"; // default job number is 99999
  }

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
  
  if(getenv("PlotsOutputDir")){
	OutputDir = getenv("PlotsOutputDir");
  }else{
	std::cout << "Plots Output Directory environmental varilable not set" << std::endl;
	return;
  }

  if(getenv("HistoInputDir")){
	InputDir = getenv("HistoInputDir");
  }else{
	std::cout << "Input Directory environmental varilable not set" << std::endl;
	return;
  }
}

int vertScaleVal(int yVal){
  int n = 0;  int p = 0; int newVal = 0;
  int nTest = pow(10,n);
  while((yVal % nTest ) != yVal){
	n++;
	nTest = pow(10,n);
  }
  if(n == 0) return newVal = 1;
  
  int firstDig = 0;
  int secondDig = 0;
  int thirdDig = 0;
  if(n >= 1) firstDig= yVal/pow(10,(n-1));
  if(n >= 2) secondDig = yVal/pow(10,(n-2)) - 10*firstDig;
  if(n >= 3) thirdDig = yVal/pow(10,(n-3)) - (100*firstDig + 10*secondDig);

  if(n == 1) newVal = yVal + 1;
  if(n == 2) newVal = firstDig*pow(10,(n-1)) + 1;
  if(n == 3 || n == 4) newVal = firstDig*pow(10,(n-1)) + (secondDig*pow(10,(n-2)) + 1);
  if(n >= 5) {
	if(thirdDig % 5 == thirdDig) thirdDig = 5;
	if(thirdDig % 5 <= 5) { thirdDig = 5; secondDig += 1;}
	newVal = firstDig*pow(10,(n-1)) + secondDig*pow(10,(n-2)) + thirdDig*pow(10,(n-3));
  }
  return newVal;
}
