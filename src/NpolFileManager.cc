//********************************************************************
//* License and Disclaimer: From GEANT Collaboration                 *
//*                                                                  *
//* The  Geant4 software  is  copyright of the Copyright Holders  of *
//* the Geant4 Collaboration.  It is provided  under  the terms  and *
//* conditions of the Geant4 Software License,  included in the file *
//* LICENSE and available at  http://cern.ch/geant4/license .  These *
//* include a list of copyright holders.     		      	*
//********************************************************************

#include <cstdio>
#include <cstdlib>
#include <string>

#include <G4ios.hh>
#include <G4String.hh>

#include <TFile.h>

#include "NpolFileManager.hh"
#include "NpolAnalysisManager.hh"

NpolFileManager *NpolFileManager::pInstance = NULL;

NpolFileManager *NpolFileManager::GetInstance() {
	if(pInstance == NULL)
		pInstance = new NpolFileManager();

	return pInstance;
}

NpolFileManager::NpolFileManager() {
	std::cout << "Constructing NpolFileManager singleton" << std::endl;

	const char *eventsPerFileString = getenv("NPOLEVENTSPERFILE");
	if(eventsPerFileString == NULL)
		eventsPerFileString = "100000";
	eventsPerFile = atoi(eventsPerFileString);

	outFile = NULL;
	fileNumber = 0;
	ConstructFilenamePrefix();
}

NpolFileManager::~NpolFileManager() {
	delete outFile;
}

// Construct the prefix for output filenames from enviornment variables, or
// use defaults if enviornment variables are not defined.
void NpolFileManager::ConstructFilenamePrefix() {
	const char *dirName = getenv("NPOLDIR");
	const char *baseName = getenv("NPOLBASENAME");
	const char *jobNumber = getenv("JOBNUMBER");

	filenamePrefix = "";
	filenamePrefix = filenamePrefix + 
		(dirName != NULL ? (G4String)dirName + "/root" : "output") + "/"
		+ (baseName != NULL ? baseName : "npol") + "_"
		+ (jobNumber != NULL ? jobNumber : "99999") + "_";
}

// Close the current file, open a new one, and increment the file number.
void NpolFileManager::OpenNextFile() {
	char fileNumberString[11];
	sprintf(fileNumberString,"%05d",fileNumber++);

	G4String filename = filenamePrefix + fileNumberString + ".root";
	
	delete outFile;
	outFile = new TFile(filename,"RECREATE");
	G4cout << filename << " was opened." << G4endl;
}

void NpolFileManager::CloseFile() {
	if(outFile != NULL)
		outFile->Write();
	delete outFile;
	outFile = NULL;
}

bool NpolFileManager::CheckIfChangingFiles(const G4int eventID) {
	if(eventsPerFile == 0) return false;
	return !(eventID % eventsPerFile);
}

void NpolFileManager::ChangeFiles() {
	if(outFile != NULL)
		outFile->Write();
	OpenNextFile();
}

