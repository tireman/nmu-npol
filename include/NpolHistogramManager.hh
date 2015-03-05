//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************

// Daniel Wilbern, dwilbern@nmu.edu

#ifndef NpolHistogramManager_h
#define NpolHistogramManager_h 

#include <map>

#include "G4SystemOfUnits.hh"

struct EDepHistoData {

	G4int histoID;
	G4String name;
	G4String title;
	G4int nbins;
	G4double xmin;
	G4double xmax;
};

class G4VPhysicalVolume;

class NpolHistogramManager {

	public:
		static NpolHistogramManager *GetInstance();
		void RegisterActiveDetector(G4VPhysicalVolume *PV, G4String nname, G4String ttitle, G4int nnbins, G4double xxmin, G4double xxmax);
		void CreateHistograms();
		void FillHistogram(G4VPhysicalVolume *PV, G4double value);
		bool CheckHistogramAssociation(G4VPhysicalVolume *PV);

	private:
		NpolHistogramManager();
		~NpolHistogramManager();

		std::map<G4VPhysicalVolume *, struct EDepHistoData> histoData;
};

#endif

