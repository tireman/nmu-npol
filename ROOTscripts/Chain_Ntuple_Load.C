#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TChain.h"

// Time to Chain some Ntuples together

void Chain_Ntuple_Load(){
  
  // Load the ntuple in the chain and point to the TTree you want
  TChain *in_chain = new TChain ("ntuples/NpolData");

  // Add the Ntuples.  Wild cards work which is great for physicsts. 
  in_chain->Add("npol_*.root");
 
  // Define the variables for loading in the branches of the TTree and others
  Double_t xpos, ypos, zpos, xmom, ymom, zmom, energy;
  Int_t VID, PartID, PID, EID;
  
  // Easy to make a histogram when you want simple cuts.
  // Doesn't work for volumeID cuts since we have this map thing
  //in_chain->Draw("YPosition/10:XPosition/10","VolumeID == 0 || 
  //          VolumeID == 4");
 
  // Set the Branch Address from the TTree to your variable you 
  // declared above.
  in_chain->SetBranchAddress("VolumeID",&VID);
  in_chain->SetBranchAddress("ParticleID",&PartID);
  in_chain->SetBranchAddress("ParentID",&PID);
  in_chain->SetBranchAddress("EventID",&EID);
  in_chain->SetBranchAddress("XPosition",&xpos);
  in_chain->SetBranchAddress("YPosition",&ypos);
  in_chain->SetBranchAddress("ZPosition",&zpos);
  in_chain->SetBranchAddress("XMomentum",&xmom);
  in_chain->SetBranchAddress("YMomentum",&ymom);
  in_chain->SetBranchAddress("ZMomentum",&zmom);
  in_chain->SetBranchAddress("VertexEnergy",&energy);
  
  Int_t icount = 0;  // for counting events so we know it hasn't locked up

  // Loop for loading each row from each ntuple tree in the chain
  for (Int_t irow=0;irow<in_chain->GetEntries();irow++){
    in_chain->GetEntry(irow);
    
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
