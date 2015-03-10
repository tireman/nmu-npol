//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************

// Daniel Wilbern, dwilbern@nmu.edu February 2015

#ifndef Npol_Data_Structure_h
#define Npol_Data_Structure_h

#include <map>

#include "G4SystemOfUnits.hh"
#include "G4VPhysicalVolume.hh"

#define PROTON_ID 1
#define NEUTRON_ID 2
#define ELECTRON_ID 3
#define POSITRON_ID 4
#define GAMMA_ID 5

struct NtupleColumns {
	G4int volumeNameColID;
	G4int particleIDColID;
	G4int parentIDColID;
	G4int vertexEnergyColID;
	G4int xPosColID;
	G4int yPosColID;
	G4int zPosColID;
};

struct HistoData {
	G4int histoID;
	const char *name;
	const char *title;
	G4int nbins;
	G4double xmin;
	G4double xmax;
};

struct ActiveDetectorData {
	G4double EDep;
	struct HistoData *histoData;
};

class NpolDataStructure {

	public:
		static NpolDataStructure *GetInstance();
		void RegisterActiveDetectorEDepHistogram(G4VPhysicalVolume *PV, G4String nname, G4String ttitle, G4int nnbins, G4double xxmin, G4double xxmax);
		void CreateHistograms();
		void CreateNtuple();
		void PrepareNewEvent();
		void AddEDep(G4VPhysicalVolume *PV, G4double dep);
		void FillHistograms();
		void FillNtuple(G4String volName, G4int particleID, G4int parentID, G4double vertexEnergy,
				G4double xPos, G4double yPos, G4double zPos);

	private:
		void FillAHistogram(struct HistoData *histoData, G4double dep);
		bool isVolumeActive(G4VPhysicalVolume *PV);
		bool volumeHasEDepHistogram(G4VPhysicalVolume *PV);

	private:
		NpolDataStructure();
		~NpolDataStructure();

		std::map<G4VPhysicalVolume *, struct ActiveDetectorData> detData;
		struct NtupleColumns cols;
};

#endif

