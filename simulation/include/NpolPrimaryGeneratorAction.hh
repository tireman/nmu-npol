//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************

#ifndef Npol_PrimaryGeneratorAction_h
#define Npol_PrimaryGeneratorAction_h

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4Types.hh"
#include "G4ParticleGun.hh"
#include "TLorentzVector.h"

class G4GeneralParticleSource;
class G4Event;

class NpolPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  NpolPrimaryGeneratorAction();
  virtual ~NpolPrimaryGeneratorAction();
  
  virtual void GeneratePrimaries(G4Event*);
  TLorentzVector GenerateNeutronEvent();
  double genCalc(double q2);
  double gmnCalc(double q2);

  static G4double NpolAng;
private:
  //G4GeneralParticleSource* fParticleGun;
  G4ParticleGun* fParticleGun;
};

#endif

