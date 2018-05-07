
#include <string>
#include "NpolFileEnvManager.hh"

NpolFileEnvManager *NpolFileEnvManager::FileEnv = NULL;

NpolFileEnvManager *NpolFileEnvManager::GetInstance(){
  if(FileEnv == NULL) FileEnv = new NpolFileEnvManager();
  return FileEnv;
}

NpolFileEnvManager::NpolFileEnvManager() {
  BaseName = "";
  JobNum = "";
  Lead = ""; 
  Energy = "";
  Bfield = "";
  OutputDir = "";
  InputDir = "";
  CType = "";
}

NpolFileEnvManager::~NpolFileEnvManager() {}

TString NpolFileEnvManager::FormInputFile(TString InputDir){
  
  TString fileName = InputDir + "/" + BaseName + "_" + JobNum + ".root";
  
  return fileName;
}

TString NpolFileEnvManager::FormOutputFile(TString OutputDir){
  
  TString fileName =  OutputDir + "/" + BaseName + "_NpolEff" + "_" + JobNum + ".root";
  
  return fileName;
}

void NpolFileEnvManager::RetrieveENVvariables() {


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

  std::cout << "Processing job number: " << JobNum << std::endl;

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
  
  if(getenv("HistoOutputDir")){
	OutputDir = getenv("HistoOutputDir");
  }else{
	std::cout << "Output Directory environmental varilable not set" << std::endl;
	return;
  }

  if(getenv("RawDataDir")){
	InputDir = getenv("RawDataDir");
  }else{
	std::cout << "Input Directory environmental varilable not set" << std::endl;
	return;
  }
}
