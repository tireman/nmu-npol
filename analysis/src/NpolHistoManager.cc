
#include "NpolHistoManager.hh"


NpolHistoManager *NpolHistoManager::HistoMan = NULL;

NpolHistoManager *NpolHistoManager::GetInstance() {
  if(HistoMan == NULL) HistoMan = new NpolHistoManager();
  return HistoMan;
}

NpolHistoManager::NpolHistoManager(){
  //NpolHistoManager *HistoMan = NpolHistoManager::GetInstance();
  //HistoMan->outFile = new TFile("test.root","RECREATE");
  //HistoMan->CreateHistograms();
}

NpolHistoManager::~NpolHistoManager(){
  //NpolHistoManager *HistoMan = NpolHistoManager::GetInstance();
  //HistoMan->WriteHistograms();
  //if(HistoMan->outFile != NULL) HistoMan->outFile->Write();
  //HistoMan->outFile->Close();
}

void NpolHistoManager::CreateHistograms(){
  //NpolHistoManager *HistoMan = NpolHistoManager::GetInstance();
  //if(HistoMan->histograms.find("h_recoilAngle_Real") == HistoMan->histograms.end())
  	//HistoMan->histograms["h_recoilAngle_Real"] = new TH1F("recoilAngle_Real","Real Proton Recoil Angle",200, 0.0, 180.0);
}

void NpolHistoManager::Fill1DHistograms(std::string hName, double value){

  //HistoMan->histograms[hName]->Fill(value);
  
}

void NpolHistoManager::WriteHistograms(){
  //NpolHistoManager *HistoMan = NpolHistoManager::GetInstance();
  //HistoMan->histograms["h_recoilAngle_Real"]->Write();
}

void NpolHistoManager::ClearHistograms(){}

void NpolHistoManager::DeleteHistograms(){}
