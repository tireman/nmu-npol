//******* NPOL Physics Variable Computation Class *******//

#ifndef Npol_Physics_Variables_h
#define Npol_Physics_Variables_h

#include <string>
#include <map>
#include "TMath.h"

class NpolEventProcessing;
class NpolEventPreProcessing;

class NpolPhysicsVariables {
  
public:

  NpolPhysicsVariables();
  ~NpolPhysicsVariables();
  static NpolPhysicsVariables *GetInstance();
  
  double getAzimuthAngle(const double p1x, const double p1y, const double p1z,const double p2x, const double p2y, const double p2z, const double p3x, const double p3y, const double p3z);
  void GetPoI(double *ret, double *time, const int section, const PolarimeterDetector type, const std::map<std::string,NpolDetectorEvent *> *detEvents); 
  void GetPoI2(double *ret, double *time, const int section, const PolarimeterDetector type, const std::map<std::string,NpolDetectorEvent *> *detEvents);
  double ReturnAngle(NpolVertex *incNeutronVert, std::map<std::string, NpolDetectorEvent *> *detEvents, const int section, const PolarimeterDetector EArrayOfInterest, double *dTOF );

  double getEDepCluster(const std::map<std::string,NpolDetectorEvent *> *detEvents, int SOI, PolarimeterDetector detectorOfInterest);
double getTotalEnergyDep(const std::map<std::string,NpolDetectorEvent *> *detEvents);
double highestEDepPV(const std::map<std::string,NpolDetectorEvent *> *detEvents, int sectionOfInterest,	 PolarimeterDetector detectorOfInterest);
  
private:
   static NpolPhysicsVariables *PhysVars;
  
};

#endif
