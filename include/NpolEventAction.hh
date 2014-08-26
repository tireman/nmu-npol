   //********************************************************************
   //* License and Disclaimer: From GEANT Collaboration                 *
   //*                                                                  *
   //* The  Geant4 software  is  copyright of the Copyright Holders  of *
   //* the Geant4 Collaboration.  It is provided  under  the terms  and *
   //* conditions of the Geant4 Software License,  included in the file *
   //* LICENSE and available at  http://cern.ch/geant4/license .  These *
   //* include a list of copyright holders.     		      	*
   //********************************************************************
   //* The Geant4 software is used by the Northern Michigan University  *
   //* in accordance to the Geant4 software license specified above.    *
   //* The NMU Collaboration does not hold any rights to this software  *
   //* and gives full permission for its use to others under the limits *
   //* imposed by the GEANT4 Collaboration.  The NMU Collaboration      *
   //* gives no express or implied warranty and use of our code is at   *
   //* the users discretion only.  		    			*
   //********************************************************************


#ifndef Npol_EventAction_h
#define Npol_EventAction_h

#include "G4HCofThisEvent.hh"
#include "G4UserEventAction.hh"

class G4Event;

class NpolEventAction : public G4UserEventAction
{
public:
  NpolEventAction();
  ~NpolEventAction();
  FILE *fp;
public:
  void BeginOfEventAction(const G4Event*);
  void EndOfEventAction(const G4Event*);

public:
  int ProcessAndPrint(G4HCofThisEvent *HCE, int CHCID);
};

#endif

