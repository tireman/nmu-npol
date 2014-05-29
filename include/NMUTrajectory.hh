//Daniel Wilbern, dwilbern@nmu.edu

#ifndef NMUTrajectory_h
#define NMUTrajectory_h

#include "G4VTrajectory.hh"
#include "G4ThreeVector.hh"
#include "G4TrajectoryPoint.hh"
#include "G4ParticleDefinition.hh" 
#include "G4Track.hh"

typedef std::vector<G4VTrajectoryPoint *> TrajectoryPointContainer;

class NMUTrajectory : public G4VTrajectory {

public:
	NMUTrajectory(const G4Track *aTrack);
	~NMUTrajectory();

public:
	inline G4int GetTrackID() const {return TrackID;}
	G4int GetParentID() const {return ParentID;}
	G4String GetParticleName() const {return ParticleName;}
	G4double GetCharge() const {return 0;} //not used
	G4int GetPDGEncoding() const {return 0;} //not used
	G4ThreeVector GetInitialMomentum() const {return initialMomentum;}
	G4double GetInitialKineticEnergy() const {return initialKineticEnergy;}
	G4double GetTotalEnergy() const {return TotalEnergy;}
	G4VPhysicalVolume *GetVolume() const {return Volume;}
	const G4LogicalVolume *GetLogicalVolumeAtVertex() const {return LogicalVolumeAtVertex;}
	const G4VProcess *GetCreatorProcess() const {return CreatorProcess;}
	int GetPointEntries() const {return 0;} //not used
	G4TrajectoryPoint *GetPoint(G4int) const {return 0;} //not used
	void AppendStep(const G4Step *aStep);
	void MergeTrajectory(G4VTrajectory *) {} //not used

private:
	TrajectoryPointContainer *positionRecord;
	G4int TrackID;
	G4int ParentID;
	G4String ParticleName;
	G4VPhysicalVolume *Volume;
	G4double initialKineticEnergy;
	G4double TotalEnergy;
	G4ThreeVector initialMomentum;
	const G4VProcess *CreatorProcess;
	const G4LogicalVolume *LogicalVolumeAtVertex;
	
	

};

#endif

