//******* NPOL Processing Class *******//

#ifndef Npol_Event_Processing_h
#define Npol_Event_Processing_h

#include <iostream>
#include <fstream>
#include <string>
#include "TString.h"
#include <vector>
#include <map>
#include "TMath.h"
#include "NpolVertex.hh"
#include "NpolTagger.hh"
#include "NpolStep.hh"
#include "NpolStatistics.hh"
#include "NpolDetectorEvent.hh"

enum PolarimeterDetector {
  analyzer = 0,
  tagger,
  topEArray,
  topdEArray,
  botEArray,
  botdEArray,
  backPlane,
  unknown
  };

class NpolEventProcessing {
  
public:
  NpolEventProcessing();
  ~NpolEventProcessing();
  
  static NpolEventProcessing *GetInstance();
  
  int sectionNumber(const std::string &volName);
  PolarimeterDetector detectorType(const std::string &volName);
  PolarimeterDetector getEArrayOfInterest(std::map<std::string,NpolDetectorEvent* > *detEvents, std::map<PolarimeterDetector, double> *eDepArrayTotal, int sectionOfInterest);
  int getSectionOfInterest(const std::map<std::string,NpolDetectorEvent *> *detEvents);
  void getEDepArrayTotal(std::map<std::string,NpolDetectorEvent* > *detEvents, std::map<PolarimeterDetector, double> *eDepArrayTotal, int SOI);
  bool checkEarrayHits(const std::map<std::string,NpolDetectorEvent *> *detEvents);
  bool checkdEarrayHits(const std::map<std::string,NpolDetectorEvent *> *detEvents);
  
  static int LAYER_NUM;
  static double LOW_THRESHOLD;
  
private:
  static NpolEventProcessing *Process;
  
};


#endif
