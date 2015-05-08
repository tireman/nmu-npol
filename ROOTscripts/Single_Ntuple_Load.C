#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"

// Function for loading a single Ntuple and working with it

void Single_Ntuple_Load(){
 
  // Load your ntuple and point to the TTree you want
  TFile *f = new TFile("npol_1.root");
  TTree *t = (TTree*)f->Get("ntuples/NpolData");
 
  // Define the variables for loading in the branches of the TTree and others
  Double_t xpos, ypos, zpos, xmom, ymom, zmom, energy;
  Int_t VID, PartID, PID, EID;
  
  // Easy to make a histogram when you want simple cuts
  t->Draw("YPosition/10:XPosition/10","VolumeID == 0 || VolumeID == 4");
 
  //  Set the Branch Address from the TTree to your variable you 
  // declared above.
  t->SetBranchAddress("VolumeID",&VID);
  t->SetBranchAddress("ParticleID",&PartID);
  t->SetBranchAddress("ParentID",&PID);
  t->SetBranchAddress("EventID",&EID);
  t->SetBranchAddress("XPosition",&xpos);
  t->SetBranchAddress("YPosition",&ypos);
  t->SetBranchAddress("ZPosition",&zpos);
  t->SetBranchAddress("XMomentum",&xmom);
  t->SetBranchAddress("YMomentum",&ymom);
  t->SetBranchAddress("ZMomentum",&zmom);
  t->SetBranchAddress("VertexEnergy",&energy);

  Int_t icount = 0;  // for counting events so we know it hasn't locked up
  
  for (Int_t irow=0;irow<t->GetEntries();irow++){
     t->GetEntry(irow);

     // Just prints the event number to the screen so we know it is working
     if (icount%10000 == 0) cout << "Count = " << icount << endl;
    icount++;
    
    // This section is where you do something with the data you loaded.
    // Currently it just prints to the screen every 10k events.  This
    // is where you will put histograming, decision making, and fill new
    // ntuples. 
    if(icount%10000 == 0){
      cout << "Volume ID " << VID  << "\t Particle ID " << PartID << "\t Parent ID " << PID << "\t Event ID " << EID << endl;
      cout << "X Position " << xpos <<"\t Y Position " << ypos << "\t Z Position " << zpos << endl; 
      cout << "X Momentum " << xmom <<"\t Y Momentum " << ymom << "\t Z Momentum " << zmom << endl;
    }
  }  
}
