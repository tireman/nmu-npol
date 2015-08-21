#define _BSD_SOURCE 1
#define BUFSZ 1024

#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TChain.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include "TString.h"
#include <stdio>
#include <map>
#include <errno.h>

// Particle ID numbers
/*#define PROTON_ID 1
#define NEUTRON_ID 2
#define ELECTRON_ID 3
#define POSITRON_ID 4
#define GAMMA_ID 5
#define PIPOS 6
#define PINEG 7
#define PINEUTRAL 8*/

Int_t NpolTest(){
  std::map<const char *, int> volIDsMap;
  std::map<const char *, int>::iterator it;

  Chain_Ntuple_Process(volIDsMap);
  
  return 0;
};


void Chain_Ntuple_Process(std::map<const char *, int>& volIDsMap){
 
  // Define the variables for loading in the branches of the TTree and others
  Float_t Wxpos, Wypos, Wzpos, xmom, ymom, zmom, Venergy, Kenergy;
  Float_t Vxpos, Vypos, Vzpos;
  Int_t VID,PartID,PID,EID,TID,SID;
  const int numVols = 218;
  Int_t VOL[numVols];
  Int_t iFlag = 0;
  string S0 = "detIDs_"; string S2 = ".txt"; 
  string S1 = "test2";
  char * Pick[numVols];

  // Load the ntuple in the chain and point to the TTree you want
  TChain *in_chain = new TChain ("ntuples/NpolData");

  // Add the Ntuples.  Wild cards work which is great for physicsts. 
  //in_chain->Add("npol_*.root");
  in_chain->Add("npol_test2.root");
  /*in_chain->Add("npol_1.root"); in_chain->Add("npol_2.root");
  in_chain->Add("npol_3.root");  in_chain->Add("npol_4.root");
  in_chain->Add("npol_5.root");  in_chain->Add("npol_6.root");
  in_chain->Add("npol_7.root");  in_chain->Add("npol_8.root");
  in_chain->Add("npol_9.root");  in_chain->Add("npol_10.root");
  in_chain->Add("npol_11.root");  in_chain->Add("npol_12.root");
  in_chain->Add("npol_13.root");  in_chain->Add("npol_14.root");
  in_chain->Add("npol_15.root");  in_chain->Add("npol_16.root");
  in_chain->Add("npol_17.root");  in_chain->Add("npol_18.root");
  in_chain->Add("npol_19.root");  in_chain->Add("npol_20.root");
  in_chain->Add("npol_21.root");  in_chain->Add("npol_22.root");
  in_chain->Add("npol_23.root");  in_chain->Add("npol_24.root");
  in_chain->Add("npol_25.root");  in_chain->Add("npol_26.root");
  in_chain->Add("npol_27.root");  in_chain->Add("npol_28.root");
  in_chain->Add("npol_29.root");  in_chain->Add("npol_30.root");
  */
  // Put histogram definitions here
  // General Histograms
  TH1F *henergy = new TH1F("henergy","Vertex Energy",500,0.,100.);
  TH2F *hYZpos = new TH2F("hYZpos"," Yposition vs. Zposition", 200, 0., 7200., 200, -200., 200.);
  TH1F *hevtID = new TH1F("hevtID","Frequency of steps in an Event", 2000, 0., 2000);
  TH2F *hXYmom = new TH2F("hXYmom","XY Momentum", 500, -70., 70., 500, -70., 70.);

  // Particle specific Histograms
  TH2F *electronYZ = new TH2F("electronXY","Electron Front Tagger YZ position",3500, 0., 7200., 200, -200., 200.);
  TH2F *protonYZ = new TH2F("protonXY","Proton Front Tagger YZ position",3500,0., 7200., 200, -200., 200.);
  TH2F *neutronYZ = new TH2F("neutronXY","Neutron Front Tagger YZ position",3500,0., 7200., 200, -200., 200.);
  TH2F *pionYZ = new TH2F("pionXY","Pion Front Tagger YZ position",3500, 0., 7200., 200, -200., 200.);
  TH2F *gammaYZ = new TH2F("gammaXY","Gamma Front Tagger YZ position",3500, -0., 7200., 200, -200., 200.);

  // Setup the array to reference the volumes of interest; this is the crap due to the way
  // we deal with the volume IDs in the simulation
  Pick[0] = "av_11_impr_1_FrontTagLV_pv_0";  Pick[1] = "av_11_impr_1_FrontTagLV_pv_1";
  Pick[2] = "av_11_impr_1_FrontTagLV_pv_2";  Pick[3] = "av_11_impr_1_FrontTagLV_pv_3";
  Pick[4] = "av_11_impr_1_FrontTagLV_pv_4";  Pick[5] = "av_11_impr_1_FrontTagLV_pv_5";
  Pick[6] = "av_11_impr_2_FrontTagLV_pv_0";  Pick[7] = "av_11_impr_2_FrontTagLV_pv_1";
  Pick[8] = "av_11_impr_2_FrontTagLV_pv_2";  Pick[9] = "av_11_impr_2_FrontTagLV_pv_3";
  Pick[10] = "av_11_impr_2_FrontTagLV_pv_4";  Pick[11] = "av_11_impr_2_FrontTagLV_pv_5";
  Pick[12] = "av_12_impr_1_FrontTagLV_pv_0";  Pick[13] = "av_12_impr_1_FrontTagLV_pv_1"; 
  Pick[14] = "av_12_impr_1_FrontTagLV_pv_2";  Pick[15] = "av_12_impr_1_FrontTagLV_pv_3";
  Pick[16] = "av_12_impr_1_FrontTagLV_pv_4";  Pick[17] = "av_12_impr_1_FrontTagLV_pv_5";
  Pick[18] = "av_12_impr_1_FrontTagLV_pv_6";  Pick[19] = "av_12_impr_1_FrontTagLV_pv_7";
  Pick[20] = "av_12_impr_2_FrontTagLV_pv_0";  Pick[21] = "av_12_impr_2_FrontTagLV_pv_1"; 
  Pick[22] = "av_12_impr_2_FrontTagLv_pv_2";  Pick[23] = "av_12_impr_2_FrontTagLV_pv_3"; 
  Pick[24] = "av_12_impr_2_FrontTagLV_pv_4";  Pick[25] = "av_12_impr_2_FrontTagLV_pv_5"; 
  Pick[26] = "av_12_impr_2_FrontTagLV_pv_6";  Pick[27] = "av_12_impr_2_FrontTagLV_pv_7"; 
  Pick[28] = "av_9_impr_1_FrontDetLV_pv_0";  Pick[29] = "av_9_impr_1_FrontDetLV_pv_1";
  Pick[30] = "av_9_impr_1_FrontDetLV_pv_2";  Pick[31] = "av_9_impr_1_FrontDetLV_pv_3";
  Pick[32] = "av_9_impr_1_FrontDetLV_pv_4";  Pick[33] = "av_9_impr_1_FrontDetLV_pv_5";
  Pick[34] = "av_9_impr_2_FrontDetLV_pv_0";  Pick[35] = "av_9_impr_2_FrontDetLV_pv_1";
  Pick[36] = "av_9_impr_2_FrontDetLV_pv_2";  Pick[37] = "av_9_impr_2_FrontDetLV_pv_3";
  Pick[38] = "av_9_impr_2_FrontDetLV_pv_4";  Pick[39] = "av_9_impr_2_FrontDetLV_pv_5";
  Pick[40] = "av_10_impr_1_FrontDetLV_pv_0";  Pick[41] = "av_10_impr_1_FrontDetLV_pv_1"; 
  Pick[42] = "av_10_impr_1_FrontDetLV_pv_2";  Pick[43] = "av_10_impr_1_FrontDetLV_pv_3";
  Pick[44] = "av_10_impr_1_FrontDetLV_pv_4";  Pick[45] = "av_10_impr_1_FrontDetLV_pv_5";
  Pick[46] = "av_10_impr_1_FrontDetLV_pv_6";  Pick[47] = "av_10_impr_1_FrontDetLV_pv_7";
  Pick[48] = "av_10_impr_2_FrontDetLV_pv_0";  Pick[49] = "av_10_impr_2_FrontDetLV_pv_1"; 
  Pick[50] = "av_10_impr_2_FrontDetLv_pv_2";  Pick[51] = "av_10_impr_2_FrontDetLV_pv_3"; 
  Pick[52] = "av_10_impr_2_FrontDetLV_pv_4";  Pick[53] = "av_10_impr_2_FrontDetLV_pv_5"; 
  Pick[54] = "av_10_impr_2_FrontDetLV_pv_6";  Pick[55] = "av_10_impr_2_FrontDetLV_pv_7"; 
  Pick[56] = "av_1_impr_1_TopDetLV_pv_0";  Pick[57] = "av_1_impr_1_TopDetLV_pv_1"; 
  Pick[58] = "av_1_impr_1_TopDetLV_pv_2";  Pick[59] = "av_1_impr_1_TopDetLV_pv_3"; 
  Pick[60] = "av_1_impr_1_TopDetLV_pv_4";  Pick[61] = "av_1_impr_1_TopDetLV_pv_5"; 
  Pick[62] = "av_1_impr_1_TopDetLV_pv_6";  Pick[63] = "av_1_impr_1_TopDetLV_pv_7"; 
  Pick[64] = "av_1_impr_1_TopDetLV_pv_8";  Pick[65] = "av_1_impr_1_TopDetLV_pv_9"; 
  Pick[66] = "av_1_impr_1_TopDetLV_pv_10";  Pick[67] = "av_1_impr_1_TopDetLV_pv_11";
  Pick[68] = "av_1_impr_1_TopDetLV_pv_12";  Pick[69] = "av_1_impr_2_TopDetLV_pv_0";  
  Pick[70] = "av_1_impr_2_TopDetLV_pv_1";  Pick[71] = "av_1_impr_2_TopDetLV_pv_2";  
  Pick[72] = "av_1_impr_2_TopDetLV_pv_3";  Pick[73] = "av_1_impr_2_TopDetLV_pv_4";  
  Pick[74] = "av_1_impr_2_TopDetLV_pv_5";  Pick[75] = "av_1_impr_2_TopDetLV_pv_6";  
  Pick[76] = "av_1_impr_2_TopDetLV_pv_7";  Pick[77] = "av_1_impr_2_TopDetLV_pv_8";  
  Pick[78] = "av_1_impr_2_TopDetLV_pv_9";  Pick[79] = "av_1_impr_2_TopDetLV_pv_10";  
  Pick[80] = "av_1_impr_2_TopDetLV_pv_11";  Pick[81] = "av_1_impr_2_TopDetLV_pv_12";
  Pick[82] = "av_2_impr_1_TopDetLV_pv_0";  Pick[83] = "av_2_impr_1_TopDetLV_pv_1"; 
  Pick[84] = "av_2_impr_1_TopDetLV_pv_2";  Pick[85] = "av_2_impr_1_TopDetLV_pv_3"; 
  Pick[86] = "av_2_impr_1_TopDetLV_pv_4";  Pick[87] = "av_2_impr_1_TopDetLV_pv_5"; 
  Pick[88] = "av_2_impr_1_TopDetLV_pv_6";  Pick[89] = "av_2_impr_1_TopDetLV_pv_7"; 
  Pick[90] = "av_2_impr_1_TopDetLV_pv_8";  Pick[91] = "av_2_impr_1_TopDetLV_pv_9"; 
  Pick[92] = "av_2_impr_1_TopDetLV_pv_10";  Pick[93] = "av_2_impr_1_TopDetLV_pv_11";
  Pick[94] = "av_2_impr_1_TopDetLV_pv_12";  Pick[95] = "av_2_impr_1_TopDetLV_pv_13";
  Pick[96] = "av_2_impr_2_TopDetLV_pv_0";  Pick[97] = "av_2_impr_2_TopDetLV_pv_1";  
  Pick[98] = "av_2_impr_2_TopDetLV_pv_2";  Pick[99] = "av_2_impr_2_TopDetLV_pv_3";  
  Pick[100] = "av_2_impr_2_TopDetLV_pv_4";  Pick[101] = "av_2_impr_2_TopDetLV_pv_5"; 
  Pick[102] = "av_2_impr_2_TopDetLV_pv_6";  Pick[103] = "av_2_impr_2_TopDetLV_pv_7";
  Pick[104] = "av_2_impr_2_TopDetLV_pv_8";  Pick[105] = "av_2_impr_2_TopDetLV_pv_9"; 
  Pick[106] = "av_2_impr_2_TopDetLV_pv_10";  Pick[107] = "av_2_impr_2_TopDetLV_pv_11"; 
  Pick[108] = "av_2_impr_2_TopDetLV_pv_12";  Pick[109] = "av_2_impr_2_TopDetLV_pv_13";
  Pick[110] = "av_3_impr_1_TopVetoLV_pv_0";  Pick[111] = "av_3_impr_1_TopVetoLV_pv_1"; 
  Pick[112] = "av_3_impr_1_TopVetoLV_pv_2";  Pick[113] = "av_3_impr_1_TopVetoLV_pv_3"; 
  Pick[114] = "av_3_impr_1_TopVetoLV_pv_4";  Pick[115] = "av_3_impr_1_TopVetoLV_pv_5"; 
  Pick[116] = "av_3_impr_1_TopVetoLV_pv_6";  Pick[117] = "av_3_impr_1_TopVetoLV_pv_7"; 
  Pick[118] = "av_3_impr_1_TopVetoLV_pv_8";  Pick[119] = "av_3_impr_1_TopVetoLV_pv_9"; 
  Pick[120] = "av_3_impr_1_TopVetoLV_pv_10";  Pick[121] = "av_3_impr_1_TopVetoLV_pv_11";
  Pick[122] = "av_3_impr_1_TopVetoLV_pv_12";  Pick[123] = "av_4_impr_1_TopVetoLV_pv_0";  
  Pick[124] = "av_4_impr_1_TopVetoLV_pv_1";  Pick[125] = "av_4_impr_1_TopVetoLV_pv_2"; 
  Pick[126] = "av_4_impr_1_TopVetoLV_pv_3";  Pick[127] = "av_4_impr_1_TopVetoLV_pv_4";  
  Pick[128] = "av_4_impr_1_TopVetoLV_pv_5";  Pick[129] = "av_4_impr_1_TopVetoLV_pv_6";  
  Pick[130] = "av_4_impr_1_TopVetoLV_pv_7";  Pick[131] = "av_4_impr_1_TopVetoLV_pv_8"; 
  Pick[132] = "av_4_impr_1_TopVetoLV_pv_9";  Pick[133] = "av_4_impr_1_TopVetoLV_pv_10"; 
  Pick[134] = "av_4_impr_1_TopVetoLV_pv_11";  Pick[135] = "av_4_impr_1_TopVetoLV_pv_12";  
  Pick[136] = "av_4_impr_1_TopVetoLV_pv_13";  Pick[137] = "av_5_impr_1_BottomDetLV_pv_0";  
  Pick[138] = "av_5_impr_1_BottomDetLV_pv_1";  Pick[139] = "av_5_impr_1_BottomDetLV_pv_2";  
  Pick[140] = "av_5_impr_1_BottomDetLV_pv_3";  Pick[141] = "av_5_impr_1_BottomDetLV_pv_4"; 
  Pick[142] = "av_5_impr_1_BottomDetLV_pv_5";  Pick[143] = "av_5_impr_1_BottomDetLV_pv_6";
  Pick[144] = "av_5_impr_1_BottomDetLV_pv_7";  Pick[145] = "av_5_impr_1_BottomDetLV_pv_8";
  Pick[146] = "av_5_impr_1_BottomDetLV_pv_9";  Pick[147] = "av_5_impr_1_BottomDetLV_pv_10";
  Pick[148] = "av_5_impr_1_BottomDetLV_pv_11";  Pick[149] = "av_5_impr_1_BottomDetLV_pv_12";
  Pick[150] = "av_5_impr_2_BottomDetLV_pv_0";  Pick[151] = "av_5_impr_2_BottomDetLV_pv_1"; 
  Pick[152] = "av_5_impr_2_BottomDetLV_pv_2";  Pick[153] = "av_5_impr_2_BottomDetLV_pv_3";
  Pick[154] = "av_5_impr_2_BottomDetLV_pv_4";  Pick[155] = "av_5_impr_2_BottomDetLV_pv_5"; 
  Pick[156] = "av_5_impr_2_BottomDetLV_pv_6";  Pick[157] = "av_5_impr_2_BottomDetLV_pv_7";
  Pick[158] = "av_5_impr_2_BottomDetLV_pv_8";  Pick[159] = "av_5_impr_2_BottomDetLV_pv_9"; 
  Pick[160] = "av_5_impr_2_BottomDetLV_pv_10";  Pick[161] = "av_5_impr_2_BottomDetLV_pv_11";
  Pick[162] = "av_5_impr_2_BottomDetLV_pv_12";  Pick[163] = "av_6_impr_1_BottomDetLV_pv_0";
  Pick[164] = "av_6_impr_1_BottomDetLV_pv_1";  Pick[165] = "av_6_impr_1_BottomDetLV_pv_2";
  Pick[166] = "av_6_impr_1_BottomDetLV_pv_3";  Pick[167] = "av_6_impr_1_BottomDetLV_pv_4";
  Pick[168] = "av_6_impr_1_BottomDetLV_pv_5";  Pick[169] = "av_6_impr_1_BottomDetLV_pv_6";
  Pick[170] = "av_6_impr_1_BottomDetLV_pv_7";  Pick[171] = "av_6_impr_1_BottomDetLV_pv_8";
  Pick[172] = "av_6_impr_1_BottomDetLV_pv_9";  Pick[173] = "av_6_impr_1_BottomDetLV_pv_10";
  Pick[174] = "av_6_impr_1_BottomDetLV_pv_11";  Pick[175] = "av_6_impr_1_BottomDetLV_pv_12";
  Pick[176] = "av_6_impr_1_BottomDetLV_pv_13";  Pick[177] = "av_6_impr_2_BottomDetLV_pv_0";
  Pick[178] = "av_6_impr_2_BottomDetLV_pv_1";  Pick[179] = "av_6_impr_2_BottomDetLV_pv_2"; 
  Pick[180] = "av_6_impr_2_BottomDetLV_pv_3";  Pick[181] = "av_6_impr_2_BottomDetLV_pv_4";
  Pick[182] = "av_6_impr_2_BottomDetLV_pv_5";  Pick[183] = "av_6_impr_2_BottomDetLV_pv_6"; 
  Pick[184] = "av_6_impr_2_BottomDetLV_pv_7";  Pick[185] = "av_6_impr_2_BottomDetLV_pv_8";
  Pick[186] = "av_6_impr_2_BottomDetLV_pv_9";  Pick[187] = "av_6_impr_2_BottomDetLV_pv_10";
  Pick[188] = "av_6_impr_2_BottomDetLV_pv_11";  Pick[189] = "av_6_impr_2_BottomDetLV_pv_12"; 
  Pick[190] = "av_6_impr_2_BottomDetLV_pv_13";  Pick[191] = "av_7_impr_1_BottomVetoLV_pv_0";
  Pick[192] = "av_7_impr_1_BottomVetoLV_pv_1";  Pick[193] = "av_7_impr_1_BottomVetoLV_pv_2"; 
  Pick[194] = "av_7_impr_1_BottomVetoLV_pv_3";  Pick[195] = "av_7_impr_1_BottomVetoLV_pv_4";
  Pick[196] = "av_7_impr_1_BottomVetoLV_pv_5";  Pick[197] = "av_7_impr_1_BottomVetoLV_pv_6"; 
  Pick[198] = "av_7_impr_1_BottomVetoLV_pv_7";  Pick[199] = "av_7_impr_1_BottomVetoLV_pv_8";
  Pick[200] = "av_7_impr_1_BottomVetoLV_pv_9";  Pick[201] = "av_7_impr_1_BottomVetoLV_pv_10";
  Pick[202] = "av_7_impr_1_BottomVetoLV_pv_11";  Pick[203] = "av_7_impr_1_BottomVetoLV_pv_12"; 
  Pick[204] = "av_8_impr_1_BottomVetoLV_pv_0";  Pick[205] = "av_8_impr_1_BottomVetoLV_pv_1"; 
  Pick[206] = "av_8_impr_1_BottomVetoLV_pv_2";  Pick[207] = "av_8_impr_1_BottomVetoLV_pv_3"; 
  Pick[208] = "av_8_impr_1_BottomVetoLV_pv_4";  Pick[209] = "av_8_impr_1_BottomVetoLV_pv_5"; 
  Pick[210] = "av_8_impr_1_BottomVetoLV_pv_6";  Pick[211] = "av_8_impr_1_BottomVetoLV_pv_7"; 
  Pick[212] = "av_8_impr_1_BottomVetoLV_pv_8";  Pick[213] = "av_8_impr_1_BottomVetoLV_pv_9"; 
  Pick[214] = "av_8_impr_1_BottomVetoLV_pv_10";  Pick[215] = "av_8_impr_1_BottomVetoLV_pv_11";
  Pick[216] = "av_8_impr_1_BottomVetoLV_pv_12";  Pick[217] = "av_8_impr_1_BottomVetoLV_pv_13";
  

  // Easy to make a histogram when you want simple cuts.
  // Doesn't work for volumeID cuts since we have this map thing
   
  // Set the Branch Address from the TTree to your variable you 
  // declared above.
  in_chain->SetBranchAddress("VolumeID",&VID);
  in_chain->SetBranchAddress("ParticleID",&PartID);
  in_chain->SetBranchAddress("ParentID",&PID);
  in_chain->SetBranchAddress("TrackID",&TID);
  in_chain->SetBranchAddress("StepID", &SID);
  in_chain->SetBranchAddress("EventID",&EID);
  in_chain->SetBranchAddress("VertexEnergy",&Venergy);
  in_chain->SetBranchAddress("KineticEnergy",&Kenergy);
  in_chain->SetBranchAddress("WorldXPosition",&Wxpos);
  in_chain->SetBranchAddress("WorldYPosition",&Wypos);
  in_chain->SetBranchAddress("WorldZPosition",&Wzpos);
  in_chain->SetBranchAddress("VolumeXPosition",&Vxpos);
  in_chain->SetBranchAddress("VolumeYPosition",&Vypos);
  in_chain->SetBranchAddress("VolumeZPosition",&Vzpos);
  in_chain->SetBranchAddress("XMomentum",&xmom);
  in_chain->SetBranchAddress("YMomentum",&ymom);
  in_chain->SetBranchAddress("ZMomentum",&zmom);
  
  Long64_t icount = 0;  // for counting events so we know it hasn't locked up
  
  // Loop for loading each row from each ntuple tree in the chain
  for (Long64_t irow=0;irow<in_chain->GetEntries();irow++){
    
    if (icount == 0) {
      icount = in_chain->GetEntries();
      cout << "Initial Count = " << icount << endl;
    }

    in_chain->GetEntry(irow);  // get the next entry
    
    // Just prints the event number to the screen so we know it is working
    if (icount%10000 == 0){ cout << "Count = " << icount << " Event ID = " << EID << endl;}
    icount--;
    
    // This section loads the detIDs_*.txt file into a map which is then scanned to fill the 
    // volume array with the volume ID from the file generated during the simulation.
    if ((EID == 0) && (iFlag == 0)){ 
      cout << "Event ID = " << EID << endl;
      string filename = S0;
      filename.append(S1);
      filename.append(S2);
      S1 = sum(S1,"1");
      printf("Filename = %s \n",filename.c_str());
      Fill_Map(volIDsMap,filename);
      iFlag = 1;
   
      for(it = volIDsMap.begin(); it != volIDsMap.end(); it++){
	for(int iTmp = 0; iTmp < numVols; iTmp++){
	  if(strcmp(it->first,Pick[iTmp]) == 0) VOL[iTmp] = it->second;
	}
      }
    }else if(EID == 1){
      iFlag = 0;
    }
/*#define PROTON_ID 1
#define NEUTRON_ID 2
#define ELECTRON_ID 3
#define POSITRON_ID 4
#define GAMMA_ID 5
#define PIPOS 6
#define PINEG 7
#define PINEUTRAL 8*/
    // Histogram processing here ...
    int i=0;
      do{
	if(VID == VOL[i]){
	  henergy->Fill(Venergy);
	  hYZpos->Fill(Wzpos,Wypos);
	  hevtID->Fill(EID);
	  hXYmom->Fill(xmom,ymom);

	  if(PartID == 3) electronYZ->Fill(Wzpos,Wypos);
	  if(PartID == 2) neutronYZ->Fill(Wzpos,Wypos);
	  if(PartID == 1) protonYZ->Fill(Wzpos,Wypos);
	  if(PartID == 5) gammaYZ->Fill(Wzpos,Wypos);
	  if((PartID >= 6) && (PartID <= 8)) pionXY->Fill(Wzpos,Wypos);
	  
	  i++;
	}else{
	  i++;
	}
      }while((VID != VOL[i-1]) && (i < numVols) && (TID < 4));

  }
   // Setup the Canvas(es)
  TCanvas *c1 = new TCanvas("c1","The Ntuple canvas",200,10,700,780);
  TPad *pad1 = new TPad("pad1","This is pad1",0.02,0.52,0.48,0.98,21);
  TPad *pad2 = new TPad("pad2","This is pad2",0.52,0.52,0.98,0.98,21);
  TPad *pad3 = new TPad("pad3","This is pad3",0.02,0.02,0.48,0.48,21);
  TPad *pad4 = new TPad("pad4","This is pad4",0.52,0.02,0.98,0.48,1);

  pad1->cd(1); pad1->SetFillColor(10); pad1->Draw();
  pad2->cd(2); pad2->SetFillColor(10); pad2->Draw();
  pad3->cd(3); pad3->SetFillColor(10); pad3->Draw();
  pad4->cd(4); pad4->SetFillColor(10); pad4->Draw(); 
  pad1->cd(); pad1->SetLogy(); henergy->Draw();
  pad2->cd(); hYZpos->Draw();
  pad3->cd(); hevtID->Draw();
  pad4->cd(); hXYmom->SetFillColor(32); hXYmom->Draw();

  TCanvas *c2 = new TCanvas("c2","The Particle canvas",200,10,700,780);
  TPad *pad5 = new TPad("pad5","This is pad5",0.02,0.52,0.48,0.98,21);
  TPad *pad6 = new TPad("pad6","This is pad6",0.52,0.52,0.98,0.98,21);
  TPad *pad7 = new TPad("pad7","This is pad7",0.02,0.02,0.48,0.48,21);
  TPad *pad8 = new TPad("pad8","This is pad8",0.52,0.02,0.98,0.48,1);

  pad5->cd(1); pad5->SetFillColor(10); pad5->Draw();
  pad6->cd(2); pad6->SetFillColor(10); pad6->Draw();
  pad7->cd(3); pad7->SetFillColor(10); pad7->Draw();
  pad8->cd(4); pad8->SetFillColor(10); pad8->Draw();
  pad5->cd(); neutronYZ->Draw();
  pad6->cd(); gammaYZ->Draw();
  pad7->cd(); electronYZ->Draw();
  pad8->cd(); protonYZ->Draw();

  TCanvas *c3 = new TCanvas("c3","The 2nd Particle canvas",200,10,700,780);
  TPad *pad9 = new TPad("pad9","This is pad9",0.02,0.52,0.48,0.98,21);
  TPad *pad10 = new TPad("pad10","This is pad10",0.52,0.52,0.98,0.98,21);
  TPad *pad11 = new TPad("pad11","This is pad11",0.02,0.02,0.48,0.48,21);
  TPad *pad12 = new TPad("pad12","This is pad12",0.52,0.02,0.98,0.48,1);
 
  pad9->cd(1); pad9->SetFillColor(10);  pad9->Draw();
  pad9->cd(); pionXY->Draw();
  //pad10->cd(2); pad10->SetFillColor(10); pad10->Draw();
  //pad11->cd(3); pad11->SetFillColor(10); pad11->Draw();
  //pad12->cd(4); pad12->SetFillColor(10); pad12->Draw(); 
  
}



// Read the file output by the Geant4 simulation that 
// maps a unique ID
// to each volume in the simulation and fill a STL map 
// with this information.
void Fill_Map(std::map<const char *, int>& volIDsMap, std::string filename) {

  const char *fname = filename.c_str();
  FILE *volIDsFile = fopen(fname, "r");
  if(volIDsFile == NULL) {
    perror("Error opening file containing volume IDs");
    exit(1);
  }

  char buf[BUFSZ];
  while(true) {
    if(!fgets(buf,BUFSZ,volIDsFile))
      break;

    int id = atoi(strtok(buf,","));
    char *volName = strtok(NULL,"\n");
    char *s = StrDup(volName);
    //   printf("what is this? %s \n", volName);
    volIDsMap.insert(std::pair<const char *, int>(s, id));
  }
  fclose(volIDsFile);

  // example of iterating through the map
  //for ( it = volIDsMap.begin(); it !=volIDsMap.end(); it++){
  //printf("Volume %d has ID %s.\n", it->second, it->first);
  //};

}
    
string sum(string a, string b){

  while(a.length() < b.length()) a = "0" + a;
  while(b.length() > a.length()) b = "0" + b;
  a = "0" + a;
  b = "0" + b;
  string sum = "";
  int carry = 0;
  for (int i=a.length()-1; i>=0; i--){
    int ac = a[i] - '0';
    int bc = b[i] - '0';
    int s = ac + bc +carry;
    carry = s/10;
    s = s %10; 
    sum =((char)(s + '0')) + sum;
  }
  return remleadzero (sum);
}

string remleadzero (string a){
  while(a.length() >= 2 && a[0] == '0') a = a.substr(1);
  return a;
}
