//******* NPOL Processing Class *******//

#ifndef Npol_Event_Processing_h
#define Npol_Event_Processing_h

#include <iostream>
#include <fstream>
#include <string>
#include "TString.h"
#include "TRandom3.h"
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
  
  void fillDetectorEventMap(std::map<std::string,NpolDetectorEvent *> &eventMap, const NpolStep *aStep);
  void fillVertexMap(std::map<int,NpolVertex *> &theVertexMap, const std::vector<NpolVertex *> *vertVector, int DesiredPID, std::string eventVolume, std::string eventProcess, double eventTime);
  void printVertexMap(std::map<int,NpolVertex *> &theVertexMap, int eventID);
  int sectionNumber(const std::string &volName);
  PolarimeterDetector detectorType(const std::string &volName);
  PolarimeterDetector getEArrayOfInterest(std::map<std::string,NpolDetectorEvent* > *detEvents, std::map<PolarimeterDetector, double> *eDepArrayTotal, int sectionOfInterest);
  int getSectionOfInterest(const std::map<std::string,NpolDetectorEvent *> *detEvents);
  void getEDepArrayTotal(std::map<std::string,NpolDetectorEvent* > *detEvents, std::map<PolarimeterDetector, double> *eDepArrayTotal, int SOI);
  bool checkEarrayHits(const std::map<std::string,NpolDetectorEvent *> *detEvents);
  bool checkdEarrayHits(const std::map<std::string,NpolDetectorEvent *> *detEvents);
  void fillFourVector(const NpolStep *aStep, std::pair<double,std::vector<double> > &a4Vector);
  void fillFourVector(const NpolVertex *aVertex, std::pair<double,std::vector<double> > &a4Vector);
  void fillFourVector(std::vector<double> &particleData, std::pair<double,std::vector<double> > &a4Vector);
  static int LAYER_NUM;
  static double LOW_THRESHOLD;
  
private:
  static NpolEventProcessing *Process;
  TRandom3 *randN = new TRandom3();
};


#endif
