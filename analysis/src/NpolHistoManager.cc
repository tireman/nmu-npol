
#include "NpolHistoManager.hh"

NpolHistoManager *NpolHistoManager::HistoMan = NULL;

NpolHistoManager *NpolHistoManager::GetInstance() {
  if(HistoMan == NULL) HistoMan = new NpolHistoManager();
  return HistoMan;
}

NpolHistoManager::NpolHistoManager(){}

NpolHistoManager::~NpolHistoManager(){
  DeleteHistograms();
  ClearHistograms();
  delete outFile2;
}

void NpolHistoManager::CreateHistograms(std::string hID, std::string hTitle,int xBins, double xMin, double xMax, int yBins, double yMin, double yMax){
  if(xBins == 0){
	std::cout << "X Bins value must be a non-zero integer" << std::endl;
	return;
  }

  std::string histogramRef = "h_" + hID;

  if(yBins == 0){
    histoMap[histogramRef] = new TH1F(hID.c_str(),hTitle.c_str(),xBins,xMin,xMax);
  } else if(yBins != 0){
	histoMap2D[histogramRef] = new TH2F(hID.c_str(),hTitle.c_str(),xBins,xMin,xMax,yBins,yMin,yMax);
  }
}
void NpolHistoManager::FillHistograms(std::string hID, double value){
  std::string histogramRef = "h_"+hID;
  (histoMap[histogramRef])->Fill(value);
}

void NpolHistoManager::FillHistograms(std::string hID, double xValue, double yValue){
  std::string histogramRef = "h_"+hID;
  (histoMap2D[histogramRef])->Fill(xValue,yValue);
}

void NpolHistoManager::WriteHistograms(){
  std::cout << "Writing Histogram to File!" << std::endl;
  std::map<std::string,TH1F *>::iterator histoIt2;
  for(histoIt2 = histoMap.begin(); histoIt2 != histoMap.end(); histoIt2++) {
	if(histoIt2->second == NULL) continue;
	histoIt2->second->Write();
  }

  std::map<std::string,TH2F *>::iterator histoIt3;
  for(histoIt3 = histoMap2D.begin(); histoIt3 != histoMap2D.end(); histoIt3++) {
	if(histoIt3->second == NULL) continue;
	histoIt3->second->Write();
  }

}

void NpolHistoManager::ClearHistograms(){
  histoMap.clear();
  histoMap2D.clear();
}

void NpolHistoManager::DeleteHistograms(){
  std::map<std::string,TH1F *>::iterator histoIt;
  for(histoIt = histoMap.begin(); histoIt != histoMap.end(); histoIt++) {
	delete histoIt->second;
  }
std::map<std::string,TH2F *>::iterator histoIt2;
  for(histoIt2 = histoMap2D.begin(); histoIt2 != histoMap2D.end(); histoIt2++) {
	delete histoIt2->second;
  }
  
}

void NpolHistoManager::OpenFile(TString OutputFile){
  delete outFile2;
  outFile2 = new TFile(OutputFile,"RECREATE");
}

void NpolHistoManager::CloseFile() {
	outFile2->Close();
	delete outFile2;
	outFile2 = NULL;
}
