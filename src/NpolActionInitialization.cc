// %% NpolActionInitization.cc  %% 
// It exists to exist to start this stuff

#include "NpolActionInitialization.hh"
#include "NpolPrimaryGeneratorAction.hh"
#include "NpolRunAction.hh"
#include "NpolEventAction.hh"
#include "NpolSteppingAction.hh"

NpolActionInitialization::NpolActionInitialization()
{}

NpolActionInitialization::~NpolActionInitialization()
{}

void NpolActionInitialization::BuildForMaster() const {
	SetUserAction(new NpolRunAction);
}

void NpolActionInitialization::Build() const {

	SetUserAction(new NpolPrimaryGeneratorAction);
	SetUserAction(new NpolRunAction);
	SetUserAction(new NpolEventAction);
	SetUserAction(new NpolSteppingAction);
}

