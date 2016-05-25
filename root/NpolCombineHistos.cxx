
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

void MergeRootObjects(TDirectory *TargetFile, TFile *InFile);
void WriteMergeObjects( TFile *TargetFile );

bool isRootFile(char *filename) {
  int len = strlen(filename);
  if(len <= 5) return false;
  return strcmp(&(filename[len-5]),".root") == 0;
}

void NpolCombineHistos() {
  
  // THis first line of variables needs to be set in order to combine the correct files together.
  std::string Lead = "15"; std::string Energy = "4.4"; std::string Bfield = "4"; 
  
  std::string OutputDir = "/data2/cgen/JlabSimData/4.4GeV/4Bdl/Lead15cm/histos";
  std::string InputDir = "/data2/cgen/JlabSimData/4.4GeV/4Bdl/Lead15cm/histos";
  std::string OutputFile;

  OutputFile = OutputDir +"/" + "npol_" + Energy + "GeV_Lead" + Lead + "cm_" + Bfield + "Bdl_Histos.root";

  TargetFile = TFile::Open( OutputFile.c_str(), "RECREATE" );

  DIR *d = NULL;
  struct dirent *dir = NULL;
 
  d = opendir(InputDir.c_str());
  if(d == NULL) {
    std::cerr << "Cannot open directory " << InputDir << std::endl;
    return;
  }
    
  while((dir = readdir(d)) != NULL) {
	std::string InFile = InputDir + "/" + dir->d_name;

	if(isRootFile(dir->d_name)) {
	  InputFile = TFile::Open( InFile.c_str(), "READ" );
	  if(InputFile->IsZombie()){
		std::cout << "File was found to be zombie so skipping. " << dir->d_name << std::endl;
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





