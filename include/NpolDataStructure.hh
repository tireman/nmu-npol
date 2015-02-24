// %% NpolDataStructure.cc %%

// Daniel Wilbern, dwilbern@nmu.edu February 2015

#ifndef Npol_Data_Structure_h
#define Npol_Data_Structure_h

#include <map>

#include "G4SystemOfUnits.hh"

class G4VPhysicalVolume;

class NpolDataStructure {

	public:
		static NpolDataStructure *GetInstance();
		void PrepareNewEvent();
		void AddEDep(G4VPhysicalVolume *PV, G4double dep);
		void FillHistograms();

	private:
		NpolDataStructure();
		~NpolDataStructure();

		std::map<G4VPhysicalVolume *, G4double> EDep;

};

#endif

