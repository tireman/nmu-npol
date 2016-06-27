//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************

#ifndef Npol_Analysis_Manager_h
#define Npol_Analysis_Manager_h

#include <vector>
#include <map>

class G4Track;
class G4Step;

class TTree;

class NpolVertex;
class NpolStep;
class NpolTagger;
class NpolStatistics;

class NpolAnalysisManager {

	public:
		static NpolAnalysisManager *GetInstance();

		void RecreateTrees();
		void BeginEvent(const G4int evtID);
		void EndEvent(const G4int evtID);
		void BeginRun(const G4int runID);
		void EndRun(const G4int runID);
		void RecordTrack(const G4Track *aTrack);
		void RecordStep(const G4Step *aStep);
		void TrackKilled(const G4int trackID);

	private:
		NpolAnalysisManager();
		~NpolAnalysisManager();

		void ClearVectors();

	private:
		static NpolAnalysisManager *pInstance;

		TTree *npolTree;
		TTree *statsTree;

		std::vector<NpolVertex *> *tracks;
		std::vector<NpolStep *> *steps;
		std::vector<NpolStatistics *> *statistics;

		int eventsPerFile;
		std::map<G4String,std::vector<NpolTagger *> *> taggers;
		bool eventFlag;
};

int partition(std::vector<NpolStep *> *aVector, int p, int q);
void QSort(std::vector<NpolStep *> *aVector, int p, int q);

#endif

