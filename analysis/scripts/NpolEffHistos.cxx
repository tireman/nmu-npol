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

// The Histograms we are going to make
 TH2F *h_dEoverE_AllEvents_Top;
 TH2F *h_dEoverE_AllEvents_Bot;
 TH2F *h_dEvsE_Real;
 TH2F *h_dEvsE_Real2;
 TH1F *h_dTOF_AllEvents;
 TH1F *h_sectionEff_AllEvents[4];
 TH1F *h_recoil_angle;
 TH1F *h_recoil_angle_raw;
 TH1F *h_Neutron_Theta;
 TH1F *h_totalEnergy;
 TH1F *h_Neutron_Momentum;
 TH1F *h_Neutron_Momentum_Initial;
 TH1F *h_Neutron_Energy_Initial;
 TH1F *h_Neutron_Energy;
 TH1F *h_Proton_Recoil_Real;
 TH1F *h_Proton_Energy_Real;
 TH1F *h_asymmetry_Real;
 TH1F *h_selectedRecoilMomentum;
 TH1F *h_qsquaredTest;
 
// Assign Histos from the input file to a pointer
 
// dE/E
 h_dEoverE_AllEvents_Top = (TH2F*)sourceNeutron->Get("dEoverEtop;1");
 h_dEoverE_AllEvents_Bot = (TH2F*)sourceNeutron->Get("dEoverEbot;1");
 h_dEvsE_Real = (TH2F*)sourceNeutron->Get("dEvsEReal;1");
 h_dEvsE_Real2 = (TH2F*)sourceNeutron->Get("dEvsEReal2;1");
 TH2F *h_dEoverE_Total = new TH2F("dEoverEtotal", "dE over E for both Top and Bottom", 400,0,120,400,0,20);
 h_dEoverE_Total->Add(h_dEoverE_AllEvents_Top,h_dEoverE_AllEvents_Bot);
 
 // dTof
 h_dTOF_AllEvents = (TH1F*)sourceNeutron->Get("dTOF;1");

 // Angle Plots
 h_recoil_angle = (TH1F*)sourceNeutron->Get("recoilAngle;1");
 h_recoil_angle_raw = (TH1F*)sourceNeutron->Get("recoilAngleRaw;1");
 h_Neutron_Theta = (TH1F*)sourceNeutron->Get("NeutronThetaAngle;1");

 // Test plots
 h_totalEnergy = (TH1F*)sourceNeutron->Get("totalEnergy;1");
 h_Neutron_Momentum = (TH1F*)sourceNeutron->Get("NeutronMomentum;1");
 h_Neutron_Momentum_Initial = (TH1F*)sourceNeutron->Get("NeutronMomentumInitial;1");
 h_Neutron_Energy_Initial = (TH1F*)sourceNeutron->Get("NeutronEnergyInitial;1");
 h_Neutron_Energy = (TH1F*)sourceNeutron->Get("NeutronEnergy;1");
 h_selectedRecoilMomentum = (TH1F*)sourceNeutron->Get("selectedRecoilMomentum;1");
 h_qsquaredTest = (TH1F*)sourceNeutron->Get("Qsquared;1");
 
 // Real NP scattering Plots
 h_Proton_Recoil_Real = (TH1F*)sourceNeutron->Get("recoilAngleReal;1");
 h_Proton_Energy_Real = (TH1F*)sourceNeutron->Get("recoilEnergyReal;1");
 h_asymmetry_Real = (TH1F*)sourceNeutron->Get("asymmetryReal;1");
 
 // Section Efficiency histograms are assigned based on cuts
 // All Events
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
 
 
 // CREATE TCANVAS
 TCanvas *dEoverE_dToF = new TCanvas("dEoverE_Two","dE Over E - All Events",1000, 800);
 TCanvas *sectionEff_AllEvents = new TCanvas("sectionEff_AllEvents","Section Efficiency - All Events",800, 800);
 TCanvas *angularPlots = new TCanvas("angularPlots","Proton Recoil Angle",800, 600);
 TCanvas *neutronInformation = new TCanvas("neutronInformation","Neutron Diagnostics",800,900);
 TCanvas *realNPevent = new TCanvas("realNPevent","Plots for Real NP Event",1000,1200);
 TCanvas *dEvsENPevent = new TCanvas("dEvsENPevent","dE vs. E for Real NP Event",1000,1200);
 TCanvas *recoilMomentum = new TCanvas("eventDiagnostics","Event Diagnostics", 1000, 1200);
 
 // Histogram Stats Removed**
 //gStyle->SetOptStat(0);
 gStyle->SetOptLogz();

 // dEoverE_Two (also the dToF plot)
 dEoverE_dToF->Divide(2,2,0.0001,0.00001,0);
 dEoverE_dToF->cd(1);
 h_dEoverE_AllEvents_Top->GetXaxis()->SetTitle("E-Array Energy Deposited (MeV)");
 h_dEoverE_AllEvents_Top->GetXaxis()->SetTitleSize(0.05);
 h_dEoverE_AllEvents_Top->GetYaxis()->SetTitleSize(0.05);
 h_dEoverE_AllEvents_Top->GetYaxis()->SetTitle("dE-Array Energy Deposited (MeV)");
 h_dEoverE_AllEvents_Top->Rebin2D(2);
 h_dEoverE_AllEvents_Top->DrawCopy("cont4");
 dEoverE_dToF->cd(2);
 h_dEoverE_AllEvents_Bot->GetXaxis()->SetTitle("E-Array Energy Deposited (MeV)");
 h_dEoverE_AllEvents_Bot->GetXaxis()->SetTitleSize(0.05);
 h_dEoverE_AllEvents_Bot->GetYaxis()->SetTitleSize(0.05);
 h_dEoverE_AllEvents_Bot->GetYaxis()->SetTitle("dE-Array Energy Deposited (MeV)");
 h_dEoverE_AllEvents_Bot->Rebin2D(2);
 h_dEoverE_AllEvents_Bot->DrawCopy("cont4");
 dEoverE_dToF->cd(3);
 h_dEoverE_Total->GetXaxis()->SetTitle("E-Array Energy Deposited (MeV)");
 h_dEoverE_Total->GetXaxis()->SetTitleSize(0.05);
 h_dEoverE_Total->GetYaxis()->SetTitleSize(0.05);
 h_dEoverE_Total->GetYaxis()->SetTitle("dE-Array Energy Deposited (MeV)");
 h_dEoverE_Total->Rebin2D(2);
 h_dEoverE_Total->DrawCopy("cont4");
 dEoverE_dToF->cd(4);
 h_dTOF_AllEvents->GetXaxis()->SetTitle("Time (ns)");
 h_dTOF_AllEvents->GetXaxis()->SetTitleSize(0.05);
 h_dTOF_AllEvents->GetYaxis()->SetTitleSize(0.05);
 h_dTOF_AllEvents->GetYaxis()->SetTitle("Number of Events");
 h_dTOF_AllEvents->SetFillColor(kBlue);
 h_dTOF_AllEvents->DrawCopy();
 
// Section Efficiencies
// All events
 sectionEff_AllEvents->Divide(2,2,0.002,0.01,0);
 for(int i = 0; i <= 3; i++){
   sectionEff_AllEvents->cd(i+1);
   h_sectionEff_AllEvents[i]->SetOption("bar");
   h_sectionEff_AllEvents[i]->GetXaxis()->SetTitle("Section of Polarimeter");
   h_sectionEff_AllEvents[i]->GetXaxis()->SetRangeUser(0,5);
   h_sectionEff_AllEvents[i]->GetXaxis()->SetNdivisions(9);
   h_sectionEff_AllEvents[i]->GetXaxis()->SetTitleSize(0.05);
   h_sectionEff_AllEvents[i]->GetYaxis()->SetTitleSize(0.05);
   h_sectionEff_AllEvents[i]->GetYaxis()->SetTitle("Percent Efficiency");
   h_sectionEff_AllEvents[i]->SetFillColor(kBlue);
   h_sectionEff_AllEvents[i]->DrawCopy();
   std::cout << "The Total " << h_sectionEff_AllEvents[i]->GetTitle() << " is " << h_sectionEff_AllEvents[i]->Integral() << "." << std::endl;
 }

 angularPlots->Divide(2,2,0.0001, 0.00001,0);
 angularPlots->cd(1);
 h_recoil_angle_raw->GetXaxis()->SetTitle("Recoil Proton Angle (degrees)");
 h_recoil_angle_raw->GetYaxis()->SetTitle("Recoil Proton Count");
 h_recoil_angle_raw->GetXaxis()->SetNdivisions(9);
 h_recoil_angle_raw->GetXaxis()->SetTitleSize(0.05);
 h_recoil_angle_raw->GetYaxis()->SetTitleSize(0.05);
 h_recoil_angle_raw->DrawCopy();
 angularPlots->cd(2);
 h_recoil_angle->GetXaxis()->SetTitle("Recoil Proton Angle (degrees)");
 h_recoil_angle->GetYaxis()->SetTitle("Recoil Proton Count");
 h_recoil_angle->GetXaxis()->SetNdivisions(9);
 h_recoil_angle->GetXaxis()->SetTitleSize(0.05);
 h_recoil_angle->GetYaxis()->SetTitleSize(0.05);
 h_recoil_angle->DrawCopy();
 angularPlots->cd(3);
 h_totalEnergy->GetXaxis()->SetTitle("Energy Deposited (MeV)");
 h_totalEnergy->GetYaxis()->SetTitle("Recoil Proton Count");
 h_totalEnergy->GetXaxis()->SetNdivisions(9);
 h_totalEnergy->GetXaxis()->SetTitleSize(0.05);
 h_totalEnergy->GetYaxis()->SetTitleSize(0.05);
 h_totalEnergy->DrawCopy();

 neutronInformation->Divide(2,3,0.0001, 0.00001,0);
 neutronInformation->cd(1);
 h_Neutron_Momentum_Initial->GetXaxis()->SetTitle("Initial Neutron Momentum (MeV/c)");
 h_Neutron_Momentum_Initial->GetYaxis()->SetTitle("Neutron Count");
 h_Neutron_Momentum_Initial->GetXaxis()->SetNdivisions(9);
 h_Neutron_Momentum_Initial->GetXaxis()->SetTitleSize(0.05);
 h_Neutron_Momentum_Initial->GetYaxis()->SetTitleSize(0.05);
 h_Neutron_Momentum_Initial->DrawCopy();
 neutronInformation->cd(2);
 h_Neutron_Momentum->GetXaxis()->SetTitle("Neutron Momentum (MeV/c)");
 h_Neutron_Momentum->GetYaxis()->SetTitle("Neutron Count");
 h_Neutron_Momentum->GetXaxis()->SetNdivisions(9);
 h_Neutron_Momentum->GetXaxis()->SetTitleSize(0.05);
 h_Neutron_Momentum->GetYaxis()->SetTitleSize(0.05);
 h_Neutron_Momentum->DrawCopy();
 neutronInformation->cd(3);
 h_Neutron_Energy_Initial->GetXaxis()->SetTitle("Initial Neutron Energy (MeV)");
 h_Neutron_Energy_Initial->GetYaxis()->SetTitle("Neutron Count");
 h_Neutron_Energy_Initial->GetXaxis()->SetNdivisions(9);
 h_Neutron_Energy_Initial->GetXaxis()->SetTitleSize(0.05);
 h_Neutron_Energy_Initial->GetYaxis()->SetTitleSize(0.05);
 h_Neutron_Energy_Initial->DrawCopy();
 neutronInformation->cd(4);
 h_Neutron_Energy->GetXaxis()->SetTitle("Neutron Energy (MeV)");
 h_Neutron_Energy->GetYaxis()->SetTitle("Neutron Count");
 h_Neutron_Energy->GetXaxis()->SetNdivisions(9);
 h_Neutron_Energy->GetXaxis()->SetTitleSize(0.05);
 h_Neutron_Energy->GetYaxis()->SetTitleSize(0.05);
 h_Neutron_Energy->DrawCopy();
 neutronInformation->cd(5);
 h_Neutron_Theta->GetXaxis()->SetTitle("Initial Neutron Angle (degrees)");
 h_Neutron_Theta->GetYaxis()->SetTitle("Neutron Count");
 h_Neutron_Theta->GetXaxis()->SetNdivisions(9);
 h_Neutron_Theta->GetXaxis()->SetTitleSize(0.05);
 h_Neutron_Theta->GetYaxis()->SetTitleSize(0.05);
 h_Neutron_Theta->DrawCopy();

 realNPevent->Divide(3,2,0.0001, 0.00001,0);
 realNPevent->cd(1);
 h_Proton_Recoil_Real->GetXaxis()->SetTitle("Proton Recoil Angle (deg)");
 h_Proton_Recoil_Real->GetYaxis()->SetTitle("Proton Count");
 h_Proton_Recoil_Real->GetXaxis()->SetNdivisions(9);
 h_Proton_Recoil_Real->SetTitleSize(0.05);
 h_Proton_Recoil_Real->SetTitleSize(0.05);
 h_Proton_Recoil_Real->DrawCopy();
 realNPevent->cd(2);
 h_Proton_Energy_Real->GetXaxis()->SetTitle("Proton Recoil Energy (MeV)");
 h_Proton_Energy_Real->GetYaxis()->SetTitle("Proton Count");
 h_Proton_Energy_Real->GetXaxis()->SetNdivisions(9);
 h_Proton_Energy_Real->SetTitleSize(0.05);
 h_Proton_Energy_Real->SetTitleSize(0.05);
 h_Proton_Energy_Real->DrawCopy();
 realNPevent->cd(3);
 h_asymmetry_Real->GetXaxis()->SetTitle("Proton Recoil Asymmetry");
 h_asymmetry_Real->GetYaxis()->SetTitle("Proton Count");
 h_asymmetry_Real->GetXaxis()->SetNdivisions(4);
 h_asymmetry_Real->SetTitleSize(0.05);
 h_asymmetry_Real->SetTitleSize(0.05);
 h_asymmetry_Real->DrawCopy();
 std::cout << "Real Up/Down asymmetry = " << (h_asymmetry_Real->GetBinContent(4)/h_asymmetry_Real->GetBinContent(2)) << std::endl;
 realNPevent->cd(4);
 h_dEvsE_Real->GetXaxis()->SetTitle("E-Array Energy Deposited (MeV)");
 h_dEvsE_Real->GetXaxis()->SetTitleSize(0.05);
 h_dEvsE_Real->GetYaxis()->SetTitleSize(0.05);
 h_dEvsE_Real->GetYaxis()->SetTitle("dE-Array Energy Deposited (MeV)");
 h_dEvsE_Real->Rebin2D(2);
 h_dEvsE_Real->DrawCopy("cont4");
 realNPevent->cd(5);
 h_dEvsE_Real2->GetXaxis()->SetTitle("E-Array Energy Deposited (MeV)");
 h_dEvsE_Real2->GetXaxis()->SetTitleSize(0.05);
 h_dEvsE_Real2->GetYaxis()->SetTitleSize(0.05);
 h_dEvsE_Real2->GetYaxis()->SetTitle("dE-Array Energy Deposited (MeV)");
 h_dEvsE_Real2->Rebin2D(2);
 h_dEvsE_Real2->DrawCopy("cont4");

 dEvsENPevent->Divide(1,1,0.0001, 0.00001,0);
 dEvsENPevent->cd(1);
 h_dEvsE_Real->GetXaxis()->SetTitle("E-Array Energy Deposited (MeV)");
 h_dEvsE_Real->GetXaxis()->SetTitleSize(0.03);
 h_dEvsE_Real->GetYaxis()->SetTitleSize(0.03);
 h_dEvsE_Real->GetYaxis()->SetTitle("dE-Array Energy Deposited (MeV)");
 //h_dEvsE_Real->Rebin2D(2);
 h_dEvsE_Real->DrawCopy("cont4");

 recoilMomentum->Divide(2,2,0.0001, 0.00001,0);
 recoilMomentum->cd(1);
 h_selectedRecoilMomentum->GetXaxis()->SetTitle("(P_leading - P_elastic) (MeV/c)");
 h_selectedRecoilMomentum->GetYaxis()->SetTitle("Counts");
 h_selectedRecoilMomentum->GetXaxis()->SetTitleSize(0.05);
 h_selectedRecoilMomentum->GetYaxis()->SetTitleSize(0.05);
 h_selectedRecoilMomentum->SetTitle("Momentum Difference");
 h_selectedRecoilMomentum->DrawCopy();
 recoilMomentum->cd(2);
 h_qsquaredTest->SetTitle("Recoil Proton Q-Squared Value (MeV/c)^2");
 h_qsquaredTest->GetXaxis()->SetTitleSize(0.03);
 h_qsquaredTest->GetYaxis()->SetTitleSize(0.03);
 h_qsquaredTest->DrawCopy();
 
 // Write out canvases to file 
 dEoverE_dToF->Write();
 sectionEff_AllEvents->Write();
 angularPlots->Write();
 neutronInformation->Write();
 realNPevent->Write();
 dEvsENPevent->Write();
 recoilMomentum->Write();
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
