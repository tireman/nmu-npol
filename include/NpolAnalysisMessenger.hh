#ifndef NpolAnalysisMessenger_h
#define NpolAnalysisMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class NpolAnalysisManager;
class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithAString;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class NpolAnalysisMessenger: public G4UImessenger
{
public:
  NpolAnalysisMessenger(NpolAnalysisManager*);
  virtual ~NpolAnalysisMessenger();

  virtual void SetNewValue(G4UIcommand*, G4String);

private:

  NpolAnalysisManager* analysisMan;
  
  G4UIdirectory* analysisDir;
  G4UIdirectory* npolDir;
  G4UIcmdWithAString* fFileCmd;
  
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
