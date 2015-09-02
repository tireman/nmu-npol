#include "NpolAnalysisMessenger.hh"

#include <sstream>

#include "NpolAnalysisManager.hh"
#include "G4UIdirectory.hh"
#include "G4UIcommand.hh"
#include "G4UIparameter.hh"
#include "G4UIcmdWithAString.hh"

NpolAnalysisMessenger::NpolAnalysisMessenger(NpolAnalysisManager* )
//: G4UImessenger(), fFileCmd(0)
{
  npolDir = new G4UIdirectory("/npol/");
  npolDir->SetGuidance("Npol Directory");
  
  analysisDir = new G4UIdirectory("/npol/analysis/");
  analysisDir->SetGuidance("Analysis Directory for Npol");
  
  fFileCmd = new G4UIcmdWithAString("/npol/analysis/setFileName",this);
  fFileCmd->SetGuidance("set name for the ROOT file");
  fFileCmd->SetGuidance("default file name is -> npol_test.root");
  fFileCmd->SetParameterName("rootName",true);
  fFileCmd->SetDefaultValue("npol_test");
}

NpolAnalysisMessenger::~NpolAnalysisMessenger()
{
  delete fFileCmd;
  delete analysisDir;
}

void NpolAnalysisMessenger::SetNewValue(G4UIcommand* command, G4String newValues)
{ 
  if (command == fFileCmd) { analysisMan->setFileName(newValues); }
}
