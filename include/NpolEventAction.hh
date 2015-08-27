//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************

#ifndef Npol_EventAction_h
#define Npol_EventAction_h

#include "G4UserEventAction.hh"

class G4Event;

class NpolEventAction : public G4UserEventAction
{
public:
  NpolEventAction();
  ~NpolEventAction();
  
public:
  void BeginOfEventAction(const G4Event*);
  void EndOfEventAction(const G4Event*);
};

#endif

