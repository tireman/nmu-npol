// %% NpolActionInitization.cc  %% 
// It exists to exist to start this stuff

#include "NpolActionInitialization.hh"
#include "NpolPrimaryGeneratorAction.hh"
#include "NpolPrimaryGeneratorActionPS.hh"
#include "NpolPrimaryGeneratorActionFS.hh"
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

  // Three sources to choose from. First is just a beam down the beamline
  // Second (*ActionPS) is an attempt at a points source with spherical 
  //distribution of momentum vectors
  // Third (*ActionFS) is a flat source that generates a random point 
  //in the xz plane and rotates it and then fires the particle at a 
  //vector relative to z-axis (NpolAng) all parallel.

  SetUserAction(new NpolPrimaryGeneratorAction);
  //SetUserAction(new NpolPrimaryGeneratorActionPS);
  //SetUserAction(new NpolPrimaryGeneratorActionFS);
  SetUserAction(new NpolRunAction);
  SetUserAction(new NpolEventAction);
  SetUserAction(new NpolSteppingAction);
}

