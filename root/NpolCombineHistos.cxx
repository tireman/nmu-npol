
#include <cstdlib>
#include <iostream>
#include <string>

#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TBranch.h>
#include <TVector.h>
#include <TH1.h>
#include <TH2.h>
#include <TSystem.h>
#include <TString.h>
#include "TKey.h"
#include "Riostream.h"

TList *FileList;
TFile *Target;

void MergeRootfile( TDirectory *target, TList *sourcelist );

void NpolCombineHistos() {
  
  // Prepare the files to me merged, throw back to the example I copied
   if(gSystem->AccessPathName("hsimple1.root")) {
     gSystem->CopyFile("hsimple.root", "hsimple1.root");
     gSystem->CopyFile("hsimple.root", "hsimple2.root");
   }

   TString Lead = "0"; TString Energy = "4.4"; TString Bfield = "4";
   TString OutputDir = "Output/";
   TString InputDir = "/work/hallc/cgen/tireman/MagFieldOn/MagField_" + Bfield + "Bdl/LeadOn" + Lead + "cm/";
   Int_t Ncut = 500;
   Int_t Nfiles = 2954;
   Int_t Nloops = 0;
   if (Nfiles % Ncut != 0){
     Nloops = Nfiles/Ncut+ 1;
   } else {
     Nloops = Nfiles/Ncut;
   }

   std::cout << "Starting up the processing ..." << std::endl;
   for(Int_t j=0; j < Nloops; j++){

     TString OutputFile;
     if(j == (Nloops - 1)){
       OutputFile = OutputDir + "JLAB" + Energy + "GeV_Lead" + Lead + "cm_" + Bfield + "Bdl_Histos.root";
     } else {
       char jtemp[2];
       sprintf(jtemp,"%i",j);
       OutputFile = OutputDir + "JLAB" + Energy + "GeV_Lead" + Lead + "cm_" + Bfield + "Bdl_Histos_All_" + jtemp + ".root";
     }

     TString InputFile;
     if (j!= 0 ){
       char jplustemp[2];
       sprintf(jplustemp,"%i",j-1);
       InputFile = OutputDir + "JLAB" + Energy + "GeV_Lead" + Lead + "cm_" + Bfield + "Bdl_Histos_All_" + jplustemp + ".root";
     }

     Int_t A = 0; 
     Int_t B = 0;
     if(j == (Nloops-1)){
       A = j*500 + 1;
       B = Nfiles + 1;
     } else {
       A = j*500 + 1;
       B = j*500 + 501;
     }
     if(B < A) continue;

     Target = TFile::Open( OutputFile, "RECREATE" );

     FileList = new TList();
     if (j != 0) FileList->Add( TFile::Open(InputFile) );

     std::cout << "Engaging files " << A << " through " << B-1 << std::endl;
     for (Int_t i = A; i < B; i++) {
       char fname[60];
       sprintf(fname,"JLAB" + Energy + "GeV_Lead" + Lead + "cm_" + Bfield + "Bdl_Histos_%i.root",i);
       FileList->Add( TFile::Open(InputDir + fname) );
     }
   

   MergeRootfile( Target, FileList );

   Target->Close();
   FileList->Clear();
   delete FileList;
 
   }

}

void MergeRootfile( TDirectory *target, TList *sourcelist ) {
 
   //  cout << "Target path: " << target->GetPath() << endl;
   TString path( (char*)strstr( target->GetPath(), ":" ) );
   path.Remove( 0, 2 );

   TFile *first_source = (TFile*)sourcelist->First();
   first_source->cd( path );
   TDirectory *current_sourcedir = gDirectory;

   //gain time, do not add the objects in the list in memory
   Bool_t status = TH1::AddDirectoryStatus();
   TH1::AddDirectory(kFALSE);

   // loop over all keys in this directory
   TChain *globChain = 0;
   TIter nextkey( current_sourcedir->GetListOfKeys() );
   TKey *key, *oldkey=0;
   while ( (key = (TKey*)nextkey())) {

       //keep only the highest cycle number for each key
      if (oldkey && !strcmp(oldkey->GetName(),key->GetName())) continue;

      // read object from first source file
      first_source->cd( path );
      TObject *obj = key->ReadObj();

      if ( obj->IsA()->InheritsFrom( TH1::Class() ) ) {
         // descendant of TH1 -> merge it

         //      cout << "Merging histogram " << obj->GetName() << endl;
         TH1 *h1 = (TH1*)obj;

         // loop over all source files and add the content of the
         // correspondant histogram to the one pointed to by "h1"
         TFile *nextsource = (TFile*)sourcelist->After( first_source );
         while ( nextsource ) {

            // make sure we are at the correct directory level by cd'ing to path
            nextsource->cd( path );
            TKey *key2 = (TKey*)gDirectory->GetListOfKeys()->FindObject(h1->GetName());
            if (key2) {
               TH1 *h2 = (TH1*)key2->ReadObj();
               h1->Add( h2 );
               delete h2;
            }

            nextsource = (TFile*)sourcelist->After( nextsource );
         }
      }
      else if ( obj->IsA()->InheritsFrom( TTree::Class() ) ) {

         // loop over all source files create a chain of Trees "globChain"
         const char* obj_name= obj->GetName();

         globChain = new TChain(obj_name);
         globChain->Add(first_source->GetName());
         TFile *nextsource = (TFile*)sourcelist->After( first_source );
         //      const char* file_name = nextsource->GetName();
         // cout << "file name  " << file_name << endl;
         while ( nextsource ) {

            globChain->Add(nextsource->GetName());
            nextsource = (TFile*)sourcelist->After( nextsource );
         }

      } else if ( obj->IsA()->InheritsFrom( TDirectory::Class() ) ) {
         // it's a subdirectory

         cout << "Found subdirectory " << obj->GetName() << endl;

         // create a new subdir of same name and title in the target file
         target->cd();
         TDirectory *newdir = target->mkdir( obj->GetName(), obj->GetTitle() );

         // newdir is now the starting point of another round of merging
         // newdir still knows its depth within the target file via
         // GetPath(), so we can still figure out where we are in the recursion
         MergeRootfile( newdir, sourcelist );

      } else if ( obj->IsA()->InheritsFrom( TVectorD::Class() ) ) {
	Long_t num1 = 0, num2 = 0;
	TVectorD *v1 = (TVectorD*)obj;
	
	TFile *nextsource = (TFile*)sourcelist->After( first_source );
         while ( nextsource ) {

            // make sure we are at the correct directory level by cd'ing to path
            nextsource->cd( path );
            TKey *key2 = (TKey*)gDirectory->GetListOfKeys()->FindObject(v1->GetName());
            if (key2) {
               TVectorD *v2 = (TVectorD*)key2->ReadObj();
	       for(Int_t i=0; i<v1->GetNoElements(); i++){
		 ((*v1))[i] += ((*v2))[i];
	       }
	       delete v2;
	    }

            nextsource = (TFile*)sourcelist->After( nextsource );
         }
      } else {

         // object is of no type that we know or can handle
         cout << "Unknown object type, name: "
         << obj->GetName() << " title: " << obj->GetTitle() << endl;
      }

      // now write the merged histogram (which is "in" obj) to the target file
      // note that this will just store obj in the current directory level,
      // which is not persistent until the complete directory itself is stored
      // by "target->Write()" below
      if ( obj ) {
         target->cd();

         //!!if the object is a tree, it is stored in globChain...
         if(obj->IsA()->InheritsFrom( TTree::Class() ))
            globChain->Merge(target->GetFile(),0,"keep");
         else
            obj->Write( key->GetName() );
      }

   } // while ( ( TKey *key = (TKey*)nextkey() ) )

   // save modifications to target file
   target->SaveSelf(kTRUE);
   TH1::AddDirectory(status);
}

