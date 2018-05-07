//***** File and Environmental Variable control *****//

#ifndef Npol_FileEnv_Manager_h
#define Npol_FileEnv_Manager_h

#include <iostream>
#include <fstream>
#include <string>
#include "TString.h"

class NpolFileEnvManager {
private:
  
public:
  NpolFileEnvManager();
  ~NpolFileEnvManager();
  static NpolFileEnvManager *GetInstance();
  
  void RetrieveENVvariables();
  TString FormInputFile(TString InputDir);
  TString FormOutputFile(TString OutputDir);
 
private:
  static NpolFileEnvManager *FileEnv;
  TString BaseName;
  TString JobNum;
  TString Lead; 
  TString Energy;
  TString Bfield;
  TString CType;

public:
  TString OutputDir;
  TString InputDir;  
  
};

#endif

