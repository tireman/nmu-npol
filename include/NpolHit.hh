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

#ifndef Npol_Hit_h
#define Npol_Hit_h

#include "G4Allocator.hh"
#include "G4THitsCollection.hh"
#include "G4VHit.hh"
#include "G4Types.hh"
#include "G4Step.hh"
#include "G4Point3D.hh"

class NpolHit : public G4VHit {

private:	
  G4double totalEnergyDeposit;
  G4Point3D prepos, postpos;
  //From pre-step point:
  G4String volumeName;
  G4String processName;
  //From track:
  G4int TrackID;
  G4int ParentID;
  G4int stepNumber;
  G4String particleName;
  G4ThreeVector ParticleMomentum;

public:
  NpolHit(G4Step *aStep);
  ~NpolHit();
  // RM 10/24 WT inline void *operator new(size_t);
  // RM 10/24 WT inline void operator delete(void *aHit);

  virtual void Draw();
  virtual void Print();
  void FilePrint();
  
  //Get functions
public:
  G4int GetTrackID() {return TrackID;}
  G4int GetParentID() {return ParentID;}
  G4int GetStepNumber() {return stepNumber;}
  G4double GetTotalEnergyDeposit() {return totalEnergyDeposit;}
  G4String GetVolumeName() {return volumeName;}
  G4String GetProcessName() {return processName;}
  G4String GetParticleName() {return particleName;}
  G4Point3D GetPreStepPos() {return prepos;}
  G4Point3D GetPostStepPos() {return postpos;}
  G4ThreeVector GetParticleMomentum() {return ParticleMomentum;}
};

/* RM 10/24 WT typedef G4THitsCollection<NpolHit> NpolHitsCollection;
extern G4ThreadLocal G4Allocator<NpolHit>* NpolHitAllocator;

inline void* NpolHit::operator new(size_t)
{
  if(!NpolHitAllocator) NpolHitAllocator = new G4Allocator<NpolHit>;
  return (void *) NpolHitAllocator->MallocSingle();
}

inline void NpolHit::operator delete(void *ahit)
{
  NpolHitAllocator->FreeSingle((NpolHit*) ahit);
  } */
 
#endif

