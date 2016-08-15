
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cstring>

#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>

#include <TFile.h>
#include <TMap.h>
#include <TTree.h>
#include <TChain.h>
#include <TBranch.h>
#include <TVectorD.h>
#include <TROOT.h>
#include <TH1.h>
#include <TH2.h>
#include <TSystem.h>
#include <TString.h>
#include "TKey.h"

#include "Riostream.h"

TFile *TargetFile = NULL;
TFile *InputFile = NULL;

std::map<std::string, TH1 *> histograms;
std::map<std::string, TVectorD *> vectors;
std::map<std::string, TChain *> treechain;

void RetrieveENVvariables();
//TString FormInputFile(TString InputDir);
TString FormOutputFile(TString OutputDir);
void MergeRootObjects(TDirectory *TargetFile, TFile *InFile);
void WriteMergeObjects( TFile *TargetFile );

bool isRootFile(char *filename) {
  int len = strlen(filename);
  if(len <= 5) return false;
  return strcmp(&(filename[len-5]),".root") == 0;
}

TString BaseName = "";
TString JobNum = "";
TString Lead = ""; 
TString Energy = "";
TString Bfield = "";
TString OutputDir = "";
TString InputDir = "";

void NpolCombineHistos() {
  
  RetrieveENVvariables();
  TString OutputFile = FormOutputFile(OutputDir);

  TargetFile = TFile::Open( OutputFile, "RECREATE" );

  DIR *d = NULL;
  struct dirent *dir = NULL;
 
  d = opendir(InputDir);
  if(d == NULL) {
    std::cerr << "Cannot open directory " << InputDir << std::endl;
    return;
  }
  
  while((dir = readdir(d)) != NULL) {
	TString InFile = InputDir + "/" + dir->d_name;
	
	if(InFile == OutputFile) continue;
	if(isRootFile(dir->d_name)) {
	  InputFile = TFile::Open( InFile, "READ" );
	  if(InputFile->IsZombie()){
		std::cout << "File was found to be zombie so skipping. " << dir->d_name << std::endl;
		InputFile->Close();
		continue;
	  }
	  
	  std::cout << "Filename: " << dir->d_name << std::endl;
	  MergeRootObjects(TargetFile, InputFile);	
	  TargetFile->SaveSelf(kTRUE);
	  InputFile->Close();
	}
  }
  WriteMergeObjects( TargetFile );
  TargetFile->Close();
  
}

void WriteMergeObjects( TFile *TargetFile ) {
  cout << "Writing the merged data." << endl;
  
  TargetFile->cd();
  std::map<std::string,TH1 *>::iterator it;
  std::map<std::string,TVectorD *>::iterator it2;
  std::map<std::string,TChain *>::iterator it3;

  for(it3 = treechain.begin(); it3 != treechain.end(); it3++){
	it3->second->Merge(TargetFile->GetFile(),0,"keep");
  }
  for(it2 = vectors.begin(); it2 != vectors.end(); it2++) {
	it2->second->Write();
  }
  for(it = histograms.begin(); it != histograms.end(); it++) {
    it->second->Write();
  } 

}


void MergeRootObjects( TDirectory *TargetFile, TFile *InFile ){

  TString path( (char*)strstr( TargetFile->GetPath(), ":" ) );
  path.Remove( 0, 1 );
  
  InFile->cd(path);
  TDirectory *current_sourcedir = gDirectory;
  
  // loop over all keys in this directory
  TIter nextkey( current_sourcedir->GetListOfKeys() );
  TKey *key, *oldkey=0;
  while ( (key = (TKey*)nextkey())) {

	//keep only the highest cycle number for each key
	if (oldkey && !strcmp(oldkey->GetName(),key->GetName())) continue;
	
	
	TObject *obj = key->ReadObj();

	if ( obj->IsA()->InheritsFrom( TH1::Class() ) ) {
	  TargetFile->cd(path);
	  // descendant of TH1 -> merge it
	  TH1 *h1 = (TH1*)obj->Clone();
	  
	  if(histograms.find(h1->GetName()) == histograms.end()){
		(histograms[h1->GetName()]) = h1;
	  } else {
		TH1 *h2 = (TH1*)(histograms[h1->GetName()]);
		h1->Add( h2 );
		(histograms[h1->GetName()]) = h1;	
		delete h2;
	  }
	  InFile->cd( path );

	} else if ( obj->IsA()->InheritsFrom( TVectorD::Class() ) ) {

	  TargetFile->cd(path);
	  
	  TVectorD *v1 = (TVectorD*)obj->Clone();
	  
	  if(vectors.find(v1->GetName()) == vectors.end()){
		(vectors[v1->GetName()]) = v1;
	  } else {
		TVectorD *v2 = (TVectorD*)(vectors[v1->GetName()]);
		for(Int_t i=0; i<v1->GetNoElements(); i++){
		  ((*v1))[i] += ((*v2))[i];
		}
		(vectors[v1->GetName()]) = v1;
		delete v2;
	  }
	  InFile->cd( path );

	} else if ( obj->IsA()->InheritsFrom( TTree::Class() ) ) {
	  
	  InFile->cd(path);

	  std::string str = std::string(obj->GetName());
	  std::map<std::string ,TChain *>::iterator rit;
	  
	  TargetFile->cd(path);
	  
	  if(treechain.find(str) == treechain.end()){
		treechain[str] =  new TChain(obj->GetName());
	  }
	  rit = treechain.find(str);
	  rit->second->Add(InFile->GetName());
	  
	  InFile->cd( path );

	} else if ( obj->IsA()->InheritsFrom( TDirectory::Class() ) ) {
	  // it's a subdirectory!!!!!
	  
	  cout << "Found subdirectory " << obj->GetName() << endl;
	  
	  // create a new subdir of same name and title in the target file
	  TargetFile->cd();
	  TDirectory *newdir = TargetFile->mkdir( obj->GetName(), obj->GetTitle() );
	  
	  // newdir is now the starting point of another round of merging
	  // newdir still knows its depth within the TargetFile via
	  // GetPath(), so we can still figure out where we are in the recursion
	  MergeRootObjects( newdir, InFile );	 
		 
	} else {
	  
	  // object is of no type that we know or can handle
	  cout << "Unknown object type, name: "
		   << obj->GetName() << " title: " << obj->GetTitle() << endl;
	  InFile->cd( path );

	}
	
	if ( obj )
	  TargetFile->cd( path );

    delete obj;
	obj = NULL;
  }
  
}

TString FormInputFile(TString InputDir){
  
  TString fileName = InputDir + "/" + BaseName + "_" + "Lead" + Lead + "cm_" + Energy + "GeV_" + Bfield + "Bdl_" + JobNum + ".root";
  
  return fileName;
}

TString FormOutputFile(TString OutputDir){
  
  TString fileName =  InputDir + "/" + BaseName + "_" + Energy + "GeV_Lead" + Lead + "cm_" + Bfield + "Bdl_Histos.root";
  
  return fileName;
}

void RetrieveENVvariables() {

 if(getenv("JOBNUMBER")){
    JobNum = getenv("JOBNUMBER");
  }else{
    JobNum = "99999"; // default job number is 99999
  }

  std::cout << "Processing job number: " << JobNum << std::endl;

  if(getenv("NPOLBASENAME")){
	BaseName = getenv("NPOLBASENAME");
  }else{
	std::cout << "Npol Base Name environmental variable not set" << std::endl; 
	return; // Return error if not found
  }

  if(getenv("Lead")){
    Lead = getenv("Lead");
  }else{
     std::cout << "Lead environmental variable not set" << std::endl;
     return; // Return error if not found
  }

  if(getenv("Energy")){
    Energy = getenv("Energy");
  }else{
    std::cout << "Energy environmental variable not set" << std::endl;
     return; // Return error if not found
  }
  
  if(getenv("Bfield")){
    Bfield = getenv("Bfield");
  }else{
    std::cout << "Bfield environmental variable not set" << std::endl;
     return; // Return error if not found
  }
  
  if(getenv("WorkOutputDir")){
	OutputDir = getenv("WorkOutputDir");
  }else{
	std::cout << "Output Directory environmental varilable not set" << std::endl;
	return;
  }

  if(getenv("WorkInputDir")){
	InputDir = getenv("WorkInputDir");
  }else{
	std::cout << "Input Directory environmental varilable not set" << std::endl;
	return;
  }
}



