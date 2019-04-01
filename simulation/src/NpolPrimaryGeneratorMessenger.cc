#include "NpolPrimaryGeneratorMessenger.hh"

#include "G4UIcommand.hh"
#include "G4SystemOfUnits.hh"

NpolPrimaryGeneratorMessenger::NpolPrimaryGeneratorMessenger(
                                  NpolPrimaryGeneratorAction* gun)
:fnpolAction(gun){
  fgunDir = new G4UIdirectory("/npol/gun/");
  fgunDir->SetGuidance("Particle Gun control commands.");

  listCmd = new G4UIcmdWithoutParameter("/npol/gun/list",this);
  listCmd->SetGuidance("List available particles.");
  listCmd->SetGuidance(" Invoke G4ParticleTable.");
  
  //help option
  hlp = new G4UIcmdWithAString("/npol/gun/help", this);
  hlp->SetGuidance(" Lists possible actions to be taken");
  hlp->SetGuidance(" Choice: h ");
  hlp->SetParameterName("choice", true);
  hlp->SetDefaultValue("off");
  hlp->SetCandidates("h off");
  hlp->AvailableForStates(G4State_PreInit, G4State_Idle);
  
  //choice of DCS filter: filter for DCS computation
  //filter = new G4UIcmdWithAString("/npol/gun/filter", this);
  filter->SetGuidance(" Choice of DCS filter");
  filter->SetGuidance(" Choice: none (default), unpolarized, polarized ");
  filter->SetParameterName("choice", true);
  filter->SetDefaultValue("none");
  filter->SetCandidates("none unpolarized polarized");
  filter->AvailableForStates(G4State_PreInit, G4State_Idle);

  //number of channels to be processed
  /*numChan = new G4UIcmdWithAnInteger("/npol/gun/numOfChannels", this);
  numChan->SetGuidance(" Number of channels to be processed");
  numChan->SetGuidance(" Choice: 18 (default), [0,17] ");
  numChan->SetParameterName("choice", true);
  numChan->SetDefaultValue((G4int)18);
  numChan->AvailableForStates(G4State_PreInit, G4State_Idle);

  //Select if pi0 Channel will be used
  piDecay = new G4UIcmdWithABool("/npol/gun/selectPi0",this);
  piDecay->SetGuidance(" Decide if ROOT file will be created");
  piDecay->SetGuidance(" Choice: false (default), true ");
  piDecay->SetParameterName("choice", true);
  piDecay->SetDefaultValue((G4bool)true);
  piDecay->AvailableForStates(G4State_PreInit,G4State_Idle);

  //mean A' resonance
  massA = new G4UIcmdWithADouble("/npol/gun/massOfAprime", this);
  massA->SetGuidance(" Mass of A'");
  massA->SetGuidance(" Choice: 0.05 GeV (default) ");
  massA->SetParameterName("choice", true);
  massA->SetDefaultValue((G4double)0.05*1*GeV);
  massA->AvailableForStates(G4State_PreInit, G4State_Idle);*/

}

NpolPrimaryGeneratorMessenger::~NpolPrimaryGeneratorMessenger(){
  
  delete hlp;
  delete filter;
  //delete numChan;
  //delete massA;
  //delete piDecay;
  delete fgunDir;
}

void NpolPrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command, G4String newVal){

  
  //G4cout << " \n  Is this GENERATOR being called at all??????????? \n \n";
  if(command == hlp){
    //florAction->SetUseHelp(newVal);
    G4cout << "\nUsage: NpolGenerator [options] inputFile\n\n";
    G4cout << "\toptions:\n";
    G4cout << "\t/npol/gun/help\t\tThis information\n";
    G4cout << "\t/npol/gun/useGenerator\t To enable use of Generator in Simulation [Default--> Real branching ratios and all 18 channels]\n";
    G4cout << "\t/npol/gun/rootFile [filename]\tdirect ROOT output to <filename>\n";
    G4cout << "\t/npol/gun/BR [argument]\tOption for branch ratio, arguments:\n\t\t\treal -- real branch ratio; default\n\t\t\tsame -- same branch ratio\n";
    G4cout << "\t/run/beamOn M [#]\t\tProcess  only (#) number of events\n\n";
    G4cout << "\t/npol/gun/numOfChannels [#]\t\t[#] channels are processed for kaonPlus decay? Default: 18\n";
    G4cout << "\t/npol/gun/processChan [\"* * *\"]\tProcessed channel number [\"* * *\"] for kaonPlus decay. Must be followed after option 'N'. Default: \"0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17\"\n";
    G4cout << "\t for example: Running 3 Channels say \"2 4 5\" means three kaonPlus decay channels 2, 4 and 5 will be processed.\n\n";
    G4cout << "\t STEP 1) /trek/useGenerator\t\tThis is to enable TrekPrimaryGenerator to switch on TrekGenerator\n";
    G4cout << "\t STEP 2) /trek/BR same(real)\t\tSelect type either uniform or realistic BR [default is real]\n";
    G4cout << "\t STEP 3) /trek/numOfChannels 3\t\tSelect the number of channels to process (3 in this example)\n";
    G4cout << "\t STEP 4) /trek/processChan 2 4 5\tChannels 2 4 5 will be processed with uniform distribution\n";
    G4cout << "\t/run/beamOn 100\t\tIn this case 100 events will be processed for the above channels\n";
    G4cout << "\t/npol/gun/selectPi0[#]\t\t[#] channels are processed for pionZero decay; Default: 3\n";
    G4cout << "\t/npol/gun/pi0ChanProcessed [\"* * *\"]\t Processed channel number [\"* * *\"] for pionPlus decay; Must be followed after option P; Default: \"0 1 2\"\n";
    G4cout << "\t/npol/gun/or example: -P 2 -C \"1 2\" means two pionZero decay channels 1 and 2 will be processed.\n\n";
    G4cout << "\t/npol/gun/massOfAprime [mean-of-resonance]\t\tMean of resonance of A' (GeV/c2); Default: 0.05\n";
    G4cout << "\t/npol/gun/epsilon2 [epsilon] epsilon square is the ratio of dark-photon coupling and real photon coupling; Default: 1e-3\n\n";
  } else if (command == filter){
	//filter->SetFilterValue("polarized");
  }
	

}
