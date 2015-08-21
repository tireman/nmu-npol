//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	     *
//********************************************************************

// Orignal: Daniel Wilbern, dwilbern@nmu.edu
// Updated: William Tireman, wtireman@nmu.edu

#ifndef Npol_Hit_h
#define Npol_Hit_h

#include "G4Allocator.hh"
#include "G4THitsCollection.hh"
#include "G4VHit.hh"
#include "G4Types.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4Point3D.hh"

class NpolHit : public G4VHit 
{  

public:
  NpolHit(G4Step *aStep);
  ~NpolHit();
  NpolHit(const NpolHit &right);
  
  inline void *operator new(size_t);
  inline void operator delete(void *aHit);
  void *operator new(size_t, void *p){return p;}
#ifndef G4NOT_ISO_DELETES
  void operator delete(void *, void*){};
#endif

  virtual void Draw();
  virtual void Print();
  void FilePrint();

private:	
  G4double totalEnergyDeposit;
  G4Point3D prepos, postpos;
  //From pre-step point:
  G4String volumeName;
  G4String processName;
  //From track:
  G4int TrackID;
  G4int ParentID;
  G4int particleID;
  G4int stepNumber;
  G4double vertexEnergy, kineticEnergy;
  G4String particleName;
  G4ThreeVector ParticleMomentum;
  G4VPhysicalVolume *volumePointer;

  //Get functions
public:
  G4int GetTrackID() {return TrackID;}
  G4int GetParentID() {return ParentID;}
  G4int GetParticleID() {return particleID;}
  G4int GetStepNumber() {return stepNumber;}
  G4double GetVertexEnergy() {return vertexEnergy;}
  G4double GetKineticEnergy() {return kineticEnergy;}
  G4double GetTotalEnergyDeposit() {return totalEnergyDeposit;}
  G4String GetVolumeName() {return volumeName;}
  G4String GetProcessName() {return processName;}
  G4String GetParticleName() {return particleName;}
  G4Point3D GetPreStepPos() {return prepos;}
  G4Point3D GetPostStepPos() {return postpos;}
  G4ThreeVector GetParticleMomentum() {return ParticleMomentum;}
  G4VPhysicalVolume* GetVolumePointer() {return volumePointer;}
};

typedef G4THitsCollection<NpolHit> NpolHitsCollection;

extern G4Allocator<NpolHit> NpolHitAllocator;

inline void* NpolHit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) NpolHitAllocator.MallocSingle();
  return aHit;
}

inline void NpolHit::operator delete(void *aHit)
{
  NpolHitAllocator.FreeSingle((NpolHit*) aHit);
}

#endif

