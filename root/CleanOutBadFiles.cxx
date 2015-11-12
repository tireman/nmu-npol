
#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>

#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>

#include "TFile.h"

bool isRootFile(char *filename) {
  int len = strlen(filename);
  if(len <= 5) return false;
  return strcmp(&(filename[len-5]),".root") == 0;
}

void CleanOutBadFiles() {
  
  std::string dirName = "/data2/cgen/JlabSimData/MagField_2Bdl/Lead10cm";
  
  DIR *d = NULL;
  struct dirent *dir = NULL;
  
  d = opendir(dirName.c_str());
  if(d == NULL) {
    std::cerr << "Cannot open directory " << dirName << std::endl;
    return;
  }
  TFile.Recover 0;
  // For each file
  while((dir = readdir(d)) != NULL) {
    std::string filename = dirName + "/" + dir->d_name;
    
    if(isRootFile(dir->d_name)) {
      TFile *f = new TFile(filename.c_str(),"READ");
      if(f->IsZombie()){
	remove(filename.c_str());
	cout << "Removed File --> " << filename.c_str() << endl;
      }
      if (f->TestBit(TFile::kRecovered)){
	cout << "Recovered File --> " << filename.c_str() << endl;
      }
	delete f;
    }
  }
  
}

