
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

void MergeRootObjects(TFile *TargetFile, TFile *InFile);
void WriteMergeObjects( TFile *TargetFile );

bool isRootFile(char *filename) {
  int len = strlen(filename);
  if(len <= 5) return false;
  return strcmp(&(filename[len-5]),".root") == 0;
}

void NpolCombineHistos() {
  
  // THis first line of variables needs to be set in order to combine the correct files together.
  std::string Lead = "15"; std::string Energy = "4.4"; std::string Bfield = "4"; 
  
  std::string OutputDir = "/home/tireman/simulation/jlab/npol/nmu-npol/root/Plots";
  std::string InputDir = "/home/tireman/simulation/jlab/npol/nmu-npol/root/Output";
  std::string OutputFile;

  //std::string dirName = InputDir; 
  std::string dirName = InputDir;
  OutputFile = OutputDir + "/Semenov" + Energy + "GeV_Lead" + Lead + "cm_" + Bfield + "Bdl_Histos.root";

  TargetFile = TFile::Open( OutputFile.c_str(), "RECREATE" );

  DIR *d = NULL;
  struct dirent *dir = NULL;
 
  //d = opendir(dirName.c_str());
  d = opendir(InputDir.c_str());
  if(d == NULL) {
    std::cerr << "Cannot open directory " << dirName << std::endl;
    return;
  }
    
  while((dir = readdir(d)) != NULL) {
	std::string InFile = InputDir + "/" + dir->d_name;

	if(isRootFile(dir->d_name)) {
	  InputFile = TFile::Open( InFile.c_str(), "READ" );	
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
  for(it2 = vectors.begin(); it2 != vectors.end(); it2++) {
	it2->second->Write();
  }
  for(it = histograms.begin(); it != histograms.end(); it++) {
    it->second->Write();
  } 
  TargetFile->SaveSelf(kTRUE);
}


void MergeRootObjects( TFile *TargetFile, TFile *InFile ){

  //std::cout << "Target path: " << TargetFile->GetPath() << std::endl;
  TString path( (char*)strstr( TargetFile->GetPath(), ":" ) );
  path.Remove( 0, 1 );
  
  InFile->cd(path);
  TDirectory *current_sourcedir = gDirectory;
  
  // loop over all keys in this directory
  //TChain *globChain = 0;
  TIter nextkey( current_sourcedir->GetListOfKeys() );
  TKey *key, *oldkey=0;
  while ( (key = (TKey*)nextkey())) {

	//keep only the highest cycle number for each key
	if (oldkey && !strcmp(oldkey->GetName(),key->GetName())) continue;
	
	InFile->cd( path );
	TObject *obj = key->ReadObj();

	if ( obj->IsA()->InheritsFrom( TH1::Class() ) ) {
	  TargetFile->cd(path);
	  // descendant of TH1 -> merge it
	  TObject *obj1 = obj->Clone();
	  TH1 *h1 = (TH1*)obj1->Clone();
	  h1->SetDirectory(0);
	  
	  if(histograms.find(h1->GetName()) == histograms.end()){
		(histograms[h1->GetName()]) = h1;
	  } else {
		TH1 *h2 = (TH1*)(histograms[h1->GetName()]);
		h2->Add( h1 );
		(histograms[h1->GetName()]) = h2;	
	  }
	  
	} else if ( obj->IsA()->InheritsFrom( TVectorD::Class() ) ) {

	  TargetFile->cd(path);
	  
	  TVectorD *v1 = (TVectorD*)obj->Clone();
	  
	  if(vectors.find(v1->GetName()) == vectors.end()){
		(vectors[v1->GetName()]) = v1;
	  } else {
		TVectorD *v2 = (TVectorD*)(vectors[v1->GetName()]);
		for(Int_t i=0; i<v1->GetNoElements(); i++){
		  ((*v2))[i] += ((*v1))[i];
		}
		(vectors[v1->GetName()]) = v2;
	  }
	} else {

	  // object is of no type that we know or can handle
	  cout << "Unknown object type, name: "
		   << obj->GetName() << " title: " << obj->GetTitle() << endl;
	}
	
	/*if ( obj ) {
	  TargetFile->cd( path );

	  //!!if the object is a tree, it is stored in globChain...
	  if(obj->IsA()->InheritsFrom( TTree::Class() )){
	  	globChain->Merge(TargetFile->GetFile(),0,"keep");
	  } else {
		obj->Write( key->GetName() );
	  }
	  }*/
    delete obj;
	obj = NULL;
  }
  
}





