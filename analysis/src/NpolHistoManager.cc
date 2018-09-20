
#include "NpolHistoManager.hh"

NpolHistoManager *NpolHistoManager::HistoMan = NULL;

NpolHistoManager *NpolHistoManager::GetInstance() {
  if(HistoMan == NULL) HistoMan = new NpolHistoManager();
  return HistoMan;
}

NpolHistoManager::NpolHistoManager(){}

NpolHistoManager::~NpolHistoManager(){}

void NpolHistoManager::CreateHistograms(){
  NpolHistoManager *HistoMan = NpolHistoManager::GetInstance();
  
  if(histoMap.find("h_recoilAngle_Real2") == HistoMan->histoMap.end())
	histoMap["h_recoilAngle_Real2"] = new TH1F("recoilAngle_Real2","Real Proton Recoil Angle",200, 0.0, 180.0);
}

void NpolHistoManager::Fill1DHistograms(std::string hName, double value){
  NpolHistoManager *HistoMan = NpolHistoManager::GetInstance();
  HistoMan->histoMap[hName]->Fill(value);
  
}

void NpolHistoManager::WriteHistograms(){
  NpolHistoManager *HistoMan = NpolHistoManager::GetInstance();
  outFile2 = new TFile("test.root","RECREATE");
  std::map<std::string,TH1F *>::iterator histoIt;
  for(histoIt = histoMap.begin(); histoIt != histoMap.end(); histoIt++) {
    histoIt->second->Write();
  }
  
  //HistoMan->histoMap["h_recoilAngle_Real2"]->Write();
  std::cout << "Got to Here!" << std::endl;
  //if(HistoMan->outFile2 != NULL) HistoMan->outFile2->Write();
  outFile2->Close();
}

void NpolHistoManager::ClearHistograms(){}

void NpolHistoManager::DeleteHistograms(){}
