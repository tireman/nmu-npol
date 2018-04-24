
#include <vector>
#include <map>

#include <TTree.h>

#include "G4PhysicalConstants.hh"
#include <G4SystemOfUnits.hh>
#include <G4Track.hh>
#include <G4Step.hh>
#include <G4StepPoint.hh>
#include <G4VPhysicalVolume.hh>
#include <G4VProcess.hh>
#include <G4NavigationHistory.hh>
#include <G4FastTrack.hh>

#include "NpolAnalysisManager.hh"
#include "NpolFileManager.hh"
#include "NpolVertex.hh"
#include "NpolStep.hh"
#include "NpolTagger.hh"
#include "NpolStatistics.hh"

#define OUTFILE_VERSION 20180423 // Determined by YYYYMMDD

NpolAnalysisManager *NpolAnalysisManager::pInstance = NULL;

NpolAnalysisManager *NpolAnalysisManager::GetInstance() {
	if(pInstance == NULL)
		pInstance = new NpolAnalysisManager();

	return pInstance;
}

NpolAnalysisManager::NpolAnalysisManager() {
	std::cout << "Constructing NpolAnalysisManager singleton" << std::endl;
	
	buildDir = "";

	eventFlag = false;

	tracks = new std::vector<NpolVertex *>();
	steps = new std::vector<NpolStep *>();

	taggers["NPOLTagger"] = new std::vector<NpolTagger *>();
	taggers["SHMSTagger"] = new std::vector<NpolTagger *>();
	taggers["ParticleTagger"] = new std::vector<NpolTagger *>();

	statistics = new std::vector<NpolStatistics *>();
	statistics->push_back(new NpolStatistics());
	((*statistics)[0])->version = OUTFILE_VERSION;
	((*statistics)[0])->totalEvents = 0;
	((*statistics)[0])->eventsSaved = 0;

	RecreateTrees();
}

NpolAnalysisManager::~NpolAnalysisManager() {
	delete npolTree;
	delete statsTree;

	ClearVectors();
	delete tracks;
	delete steps;

	std::map<G4String,std::vector<NpolTagger *> *>::iterator it;
	for(it = taggers.begin(); it != taggers.end(); it++)
		delete it->second;
	taggers.clear();

	std::vector<NpolStatistics *>::iterator it2;
	for(it2 = statistics->begin(); it2 != statistics->end(); it2++)
		delete *it2;
	statistics->clear();
	delete statistics;
}

// Delete and recreate the TTrees so they belong to the most recently opened TFile.
void NpolAnalysisManager::RecreateTrees() {
	npolTree = NULL;
	npolTree = new TTree("T","Per-event information from Npol simulation");
	npolTree->Branch("tracks","std::vector<NpolVertex *>",&tracks,32000,2);
	npolTree->Branch("steps","std::vector<NpolStep *>",&steps,32000,2);
	std::map<G4String,std::vector<NpolTagger *> *>::iterator it;
	for(it = taggers.begin(); it != taggers.end(); it++)
		npolTree->Branch(it->first,"std::vector<NpolTagger *>",it->second,32000,2);

	statsTree = NULL;
	statsTree = new TTree("T2","Per-run information from Npol simulation");
	statsTree->Branch("stats","std::vector<NpolStatistics *>",&statistics,32000,2);
	((*statistics)[0])->version = OUTFILE_VERSION;
	((*statistics)[0])->totalEvents = 0;
	((*statistics)[0])->eventsSaved = 0;
}

// Prepare for the next event.
void NpolAnalysisManager::BeginEvent(const G4int evtID) {
	eventFlag = false;
	ClearVectors();
	
	NpolFileManager *fileMan = NpolFileManager::GetInstance();
	if(fileMan->CheckIfChangingFiles(evtID)) {
		statsTree->Fill();
		fileMan->ChangeFiles();
		RecreateTrees();
	}
}

// Clean up a the end of an event.
void NpolAnalysisManager::EndEvent(const G4int evtID) {
	if(eventFlag) {
		((*statistics)[0])->eventsSaved += 1;
		QSort(steps,1,steps->size()); // Sort the steps vector in order of global time
		npolTree->Fill();
	}
	((*statistics)[0])->totalEvents += 1;
	
}

// Prepare for the run.
void NpolAnalysisManager::BeginRun(const G4int runID) {}

// Clean up at the end of the run.
void NpolAnalysisManager::EndRun(const G4int runID) {
	statsTree->Fill();
	ClearVectors();
	statsTree=NULL;
	NpolFileManager::GetInstance()->CloseFile();
}

// Make a new NpolVertex and add it to the tracks vector at index determined by TrackID
void NpolAnalysisManager::RecordTrack(const G4Track *aTrack) {
	NpolVertex *anNpolVertex = new NpolVertex();
	G4int trackID = aTrack->GetTrackID();
	G4int parentID = aTrack->GetParentID();

	anNpolVertex->trackId = trackID;
	anNpolVertex->parentId = parentID;
	anNpolVertex->posX = (aTrack->GetPosition()).x()/cm;
	anNpolVertex->posY = (aTrack->GetPosition()).y()/cm;
	anNpolVertex->posZ = (aTrack->GetPosition()).z()/cm;
	/*anNpolVertex->gPosX = (aTrack->GetPosition()).x()/cm;
	anNpolVertex->gPosY = (aTrack->GetPosition()).y()/cm;
	anNpolVertex->gPosZ = (aTrack->GetPosition()).z()/cm;
	anNpolVertex->lPosX = (aTrack->GetPosition()).x()/cm;
	anNpolVertex->lPosY = (aTrack->GetPosition()).y()/cm;
	anNpolVertex->lPosZ = (aTrack->GetPosition()).z()/cm;*/
	anNpolVertex->momX = (aTrack->GetMomentum()).x()/MeV;
	anNpolVertex->momY = (aTrack->GetMomentum()).y()/MeV;
	anNpolVertex->momZ = (aTrack->GetMomentum()).z()/MeV;
	anNpolVertex->time = (aTrack->GetGlobalTime())/ns;
	anNpolVertex->energy = (aTrack->GetKineticEnergy())/MeV;
	anNpolVertex->eMiss = false;
	anNpolVertex->particle = (aTrack->GetDefinition()->GetParticleName()).data();
	anNpolVertex->particleId = aTrack->GetDefinition()->GetPDGEncoding();
	anNpolVertex->volume = (aTrack->GetVolume()->GetName()).data();
	if(aTrack->GetCreatorProcess() != NULL)
		anNpolVertex->process = (aTrack->GetCreatorProcess()->GetProcessName()).data();
	else
		anNpolVertex->process = "";
	
	// Add anNpolVertex to the tracks vector with its trackId as the index.
	// This may require the vector be resized first.
	if(tracks->size() <= (unsigned int)trackID)
		tracks->resize(trackID+1);
	(*tracks)[trackID] = anNpolVertex;

	// Add this track to its mother's list of daughters
	if(parentID != 0) {
		NpolVertex *parent = tracks->at(parentID);
		(parent->daughterIds).push_back(trackID);
	} 
}

// Make a new NpolStep and add it to the steps vector.  Also check if this step is in a tagger
// volume and add it to the appropriate tagger vector if that is the case.
void NpolAnalysisManager::RecordStep(const G4Step *aStep) {
	G4Track *aTrack = aStep->GetTrack();
	G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
	std::string volName = preStepPoint->GetPhysicalVolume()->GetName();
	G4ThreeVector worldPosition = preStepPoint->GetPosition();
	G4ThreeVector localPosition = preStepPoint->GetTouchableHandle()->GetHistory()
	  ->GetTopTransform().TransformPoint(worldPosition);

	if(volName.substr(0,3) == "av_") {
    	NpolStep *npolStep = new NpolStep();
		
		npolStep->trackId = aTrack->GetTrackID();
		npolStep->parentId = aTrack->GetParentID();
		npolStep->gPosX = worldPosition.x()/cm;
		npolStep->gPosY = worldPosition.y()/cm;
		npolStep->gPosZ = worldPosition.z()/cm;
		npolStep->lPosX = localPosition.x()/cm;
		npolStep->lPosY = localPosition.y()/cm;
		npolStep->lPosZ = localPosition.z()/cm;
		npolStep->momX = (aTrack->GetMomentum()).x()/MeV;
		npolStep->momY = (aTrack->GetMomentum()).y()/MeV;
		npolStep->momZ = (aTrack->GetMomentum()).z()/MeV;
		npolStep->time = aTrack->GetGlobalTime()/ns;
		npolStep->eDep = (aStep->GetTotalEnergyDeposit())/MeV;
		npolStep->particleId = aTrack->GetDefinition()->GetPDGEncoding();
		npolStep->volume = volName;
		steps->push_back(npolStep);
	}

	// Flag this event to be saved if at least one step takes place inside
	// this tagger volume.
	if((taggers.find(volName) != taggers.end()) || (volName.substr(0,3) == "av_"))
		eventFlag = true;

	if(taggers.find(volName) != taggers.end()) {
		NpolTagger *taggedParticle = new NpolTagger();

		taggedParticle->trackId = aTrack->GetTrackID();
		taggedParticle->parentId = aTrack->GetParentID();
		taggedParticle->gPosX = worldPosition.x()/cm;
		taggedParticle->gPosY = worldPosition.y()/cm;
		taggedParticle->gPosZ = worldPosition.z()/cm;
		taggedParticle->lPosX = localPosition.x()/cm;
		taggedParticle->lPosY = localPosition.y()/cm;
		taggedParticle->lPosZ = localPosition.z()/cm;
		taggedParticle->momX = (aTrack->GetMomentum()).x()/MeV;
		taggedParticle->momY = (aTrack->GetMomentum()).y()/MeV;
		taggedParticle->momZ = (aTrack->GetMomentum()).z()/MeV;
		taggedParticle->time = aTrack->GetGlobalTime()/ns;
		taggedParticle->energy = aTrack->GetKineticEnergy()/MeV;
		taggedParticle->particle = (aTrack->GetDefinition()->GetParticleName()).data();
		taggedParticle->particleId = aTrack->GetDefinition()->GetPDGEncoding();

		(taggers[volName])->push_back(taggedParticle);
	}
}

// If we manually kill a track (e.g. hits the hall shell or exits the world), we should set the
// eMiss boolean for that track in the vector.
void NpolAnalysisManager::TrackKilled(const G4int trackID) {
	(*tracks)[trackID]->eMiss = true;
}

// clear out the per-event information vectors
void NpolAnalysisManager::ClearVectors() {
	std::vector<NpolVertex *>::iterator it;
	std::vector<NpolStep *>::iterator it2;
	std::vector<NpolTagger *>::iterator it3;
	std::map<G4String, std::vector<NpolTagger *> *>::iterator it4;

	for(it = tracks->begin(); it != tracks->end(); it++)
		delete *it;
	tracks->clear();

	for(it2 = steps->begin(); it2 != steps->end(); it2++)
		delete *it2;
	steps->clear();

	for(it4 = taggers.begin(); it4 != taggers.end(); it4++) {
		std::vector<NpolTagger *> *v = it4->second;
		for(it3 = v->begin(); it3 != v->end(); it3++) {
			NpolTagger *t = *it3;
			delete t;
		}
		v->clear();
	}
}

// Set buildDir to the path of the  build directory containing the gdml and
// macros directory.  This is done by popping characters off from argv[0] until we
// find a '/'.
void NpolAnalysisManager::SetBuildDir(const char *argv0) {
	buildDir = argv0;
	while(buildDir.length() != 0 && buildDir.back() != '/')
		buildDir.pop_back();
}

G4String NpolAnalysisManager::GetBuildDir() {
	if(buildDir == "") {
		G4cout << "Warning: buildDir is an empty string.  Did you set it or is Npolapp in your PATH?" << G4endl;
		return "./";
	} else
	return buildDir;
}

// Quicksort routine for sorting the steps vector in time order
// before filling the tree
int partition(std::vector<NpolStep *> *aVector, int p, int q) {
	double x = (*aVector)[p]->time;
	int i = p;

	for(int j = p+1; j < q; j++) {
		if((*aVector)[j]->time <= x) {
			i++;
			std::swap((*aVector)[i],(*aVector)[j]);
		}
	}

	std::swap((*aVector)[i],(*aVector)[p]);
	return i;
}

void QSort(std::vector<NpolStep *> *aVector, int p, int q) {
	if(aVector == NULL)
		return;

	if(p<q) {
		int r = partition(aVector,p,q);
		QSort(aVector,p,r);
		QSort(aVector,r+1,q);
	}
}

