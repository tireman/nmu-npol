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

//Daniel Wilbern, dwilbern@nmu.edu

#ifndef Npol_SensitiveDetector_h
#define Npol_SensitiveDetector_h

#include "G4VSensitiveDetector.hh"
#include "G4THitsCollection.hh"
#include "G4Step.hh"
#include "G4TouchableHistory.hh"

#include "NpolHit.hh"

typedef G4THitsCollection<NpolHit> NpolHitsCollection;

class NpolSensitiveDetector : public G4VSensitiveDetector {
  
private:
  NpolHitsCollection *hitsCollection;
  G4int collectionID;
  
public:
  NpolSensitiveDetector(G4String name);
  virtual ~NpolSensitiveDetector();
  virtual void Initialize(G4HCofThisEvent*HCE);
  virtual void EndOfEvent(G4HCofThisEvent*HCE);
  virtual G4bool ProcessHits(G4Step*aStep, G4TouchableHistory*ROHist);
};

#endif

