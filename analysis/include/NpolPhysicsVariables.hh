//******* NPOL Physics Variable Computation Class *******//

#ifndef Npol_Physics_Variables_h
#define Npol_Physics_Variables_h

#include <string>
#include <map>
#include "TMath.h"
#include "NpolStep.hh"
#include "TRandom3.h"
#include "TVector3.h"

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
  double computeBetheBloch(double KE, double Mass, int z, double rho, double A, int Z, double I);
  double computeEnergyLoss(double protonEnergy, double thetaP, double scintThick);
  double computeMomentum(double xMom, double yMom, double zMom);
  double computeScatTheta(double xMom, double yMom);
  double computeScatPhi(double zMom, double totalMom);
  double computeElasticMomentum(const std::pair<double,std::vector<double>> &projNeutron4Vec, double thetaP);
  bool checkQuasiElasticScattering(const std::map<int,NpolVertex *> &theVertexMap, const std::pair<double,std::vector<double> > &projNeutron4Vec);
  double compute4VecSquared(const std::pair<double,std::vector<double>> &aParticle4Vec);
  double computeLeadingParticleMomentum(const std::map<int,NpolVertex *> &theVertexMap,int selectedTID);
  double computeRecoilParticleAngle(const std::pair<double,std::vector<double>> &projNeutron4Vec, const std::map<int,NpolVertex *> &theVertexMap, int selectedTID);
  double returnParticleMomentum(const std::pair<double,std::vector<double>> &aParticle4Vec);
  double returnParticleEnergy(const std::pair<double,std::vector<double> > &aParticle4Vec);
  double returnParticleEnergy(const std::map<int,NpolVertex *> &theVertexMap, int TID);
  int findLeadingParticle(const std::map<int,NpolVertex *> &theVertexMap, std::string eventType);
  int findBestProtonTrackID(const std::map<int,NpolVertex *> &theVertexMap, const std::vector<NpolStep *> *steps, int npSOI);
  
private:
  static NpolPhysicsVariables *PhysVars;
  TRandom3 *randN = new TRandom3();
};

#endif
