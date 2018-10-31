
#include "NpolEventPreProcessing.hh"
#include "NpolEventProcessing.hh"
#include "NpolPhysicsVariables.hh"
#include "NpolStep.hh"
#include "NpolVertex.hh"
#include "TVector3.h"

#include <cmath>

NpolPhysicsVariables *NpolPhysicsVariables::PhysVars = NULL;

NpolPhysicsVariables *NpolPhysicsVariables::GetInstance() {
	if(PhysVars == NULL) PhysVars = new NpolPhysicsVariables();
	return PhysVars;
}

NpolPhysicsVariables::NpolPhysicsVariables(){}

NpolPhysicsVariables::~NpolPhysicsVariables(){}

// Return the azimuthal part of the scattering angle (in degrees) in the polarimeter given
// three points in global coordinates: (p1x,p1y,p1z) - a point along the line of the
// incoming neutron's trajectory (the vertex in the target will do)
// (p2x,p2y,p2z) - the point where the n,p elastic collision occurs in the analyzer detector
// (p3x,p3y,p3z) - a point along the trajectory of the scattered proton (in the E array)
// The calculation is done by forming a triangle with these points and using the law of cosines
double NpolPhysicsVariables::getAzimuthAngle(const double p1x, const double p1y, const double p1z, const double p2x, const double p2y, const double p2z, const double p3x, const double p3y, const double p3z) {

 
  double s1, s2, s3; // the lengths of the triangle's three sides
  s1 = TMath::Sqrt(TMath::Power(p2x-p1x,2) + TMath::Power(p2y-p1y,2) + TMath::Power(p2z-p1z,2));
  s2 = TMath::Sqrt(TMath::Power(p3x-p2x,2) + TMath::Power(p3y-p2y,2) + TMath::Power(p3z-p2z,2));
  s3 = TMath::Sqrt(TMath::Power(p3x-p1x,2) + TMath::Power(p3y-p1y,2) + TMath::Power(p3z-p1z,2));

  double theta = TMath::ACos((s1*s1 + s2*s2 - s3*s3)/(2.0*s1*s2));
  return 180.0-TMath::RadToDeg()*theta;
}

// Pass an array of 3 doubles to hold the x,y,z positions, the section number and detector type identifying the array, and the detEvents map with global position set when the detector first crosses the desired energy threshold
void NpolPhysicsVariables::GetPoI(double *ret, double *time, const int section, const PolarimeterDetector type, const std::map<std::string,NpolDetectorEvent *> *detEvents) {

  NpolEventProcessing *Process = NpolEventProcessing::GetInstance();
  std::map<std::string,NpolDetectorEvent *>::const_iterator it;

  ret[0] = 0.0;
  ret[1] = 0.0;
  ret[2] = 0.0;
  double totEdepSoFar = 0.0;
  for(it = detEvents->begin(); it != detEvents->end(); it++) {
    if(Process->sectionNumber(it->first) == section && Process->detectorType(it->first) == type) {
      if(it->second->thresholdExceeded) {
		ret[0] += (it->second->totEnergyDep)*(it->second->hPosX);
		ret[1] += (it->second->totEnergyDep)*(it->second->hPosY);
		ret[2] += (it->second->totEnergyDep)*(it->second->hPosZ);
		*time += (it->second->totEnergyDep)*(it->second->time);
		totEdepSoFar += it->second->totEnergyDep;
      }
    }
  }
  
  // Compute the weighted average
  ret[0] /= totEdepSoFar;
  ret[1] /= totEdepSoFar;
  ret[2] /= totEdepSoFar;
  *time /= totEdepSoFar;
}
/*		if((it->second->totEnergyDep) > totEdepSoFar){
		  totEdepSoFar = it->second->totEnergyDep;
		  ret[0] = (it->second->hPosX);
		  ret[1] = (it->second->hPosY);
		  ret[2] = (it->second->hPosZ);
		  *time  = (it->second->time);
		}
      }
    }
  }
  }*/

void NpolPhysicsVariables::GetPoI2(double *ret, double *time, const int SOI, const PolarimeterDetector type, const std::map<std::string,NpolDetectorEvent *> *detEvents) {
  NpolEventProcessing *Process = NpolEventProcessing::GetInstance();
  NpolEventPreProcessing *PProcess = NpolEventPreProcessing::GetInstance();
  std::map<std::string,NpolDetectorEvent *>::const_iterator it;
  ret[0] = 0.0;
  ret[1] = 0.0;
  ret[2] = 0.0;
  double totEdepSoFar = 0.0;
  for(it = detEvents->begin(); it != detEvents->end(); it++) {
	int section = Process->sectionNumber(it->first);
    if(section == SOI && Process->detectorType(it->first) == type) {
      if(it->second->thresholdExceeded) {
		ret[0] += (it->second->totEnergyDep)*(it->second->hPosX);
		ret[1] += (it->second->totEnergyDep)*(it->second->hPosY);
		ret[2] += (it->second->totEnergyDep)*(it->second->hPosZ);
		*time += (it->second->totEnergyDep)*(it->second->time);
		totEdepSoFar += it->second->totEnergyDep;
      }
    } else if((section == (SOI + 1)) && Process->detectorType(it->first) == type){
	  int AVNum = PProcess->GetAVNumber(it->first);
	  int PVNum = PProcess->GetPlacementNumber(it->first);
	  if((SOI == 0) && ((AVNum == 1) || (AVNum == 5)) && ((PVNum >= 3) && (PVNum <= 5))){
		ret[0] += (it->second->totEnergyDep)*(it->second->hPosX);
		ret[1] += (it->second->totEnergyDep)*(it->second->hPosY);
		ret[2] += (it->second->totEnergyDep)*(it->second->hPosZ);
		*time += (it->second->totEnergyDep)*(it->second->time);
		totEdepSoFar += it->second->totEnergyDep;
	  } else if((SOI == 1) && ((AVNum == 2) || (AVNum == 6)) && ((PVNum >= 11) && (PVNum <= 13))){
		ret[0] += (it->second->totEnergyDep)*(it->second->hPosX);
		ret[1] += (it->second->totEnergyDep)*(it->second->hPosY);
		ret[2] += (it->second->totEnergyDep)*(it->second->hPosZ);
		*time += (it->second->totEnergyDep)*(it->second->time);
		totEdepSoFar += it->second->totEnergyDep;
	  } else if((SOI == 2) && ((AVNum == 2) || (AVNum == 6)) && ((PVNum >= 4) && (PVNum <= 6))){
		ret[0] += (it->second->totEnergyDep)*(it->second->hPosX);
		ret[1] += (it->second->totEnergyDep)*(it->second->hPosY);
		ret[2] += (it->second->totEnergyDep)*(it->second->hPosZ);
		*time += (it->second->totEnergyDep)*(it->second->time);
		totEdepSoFar += it->second->totEnergyDep;
	  }
	}
  }
  
  // Compute the weighted average
  ret[0] /= totEdepSoFar;
  ret[1] /= totEdepSoFar;
  ret[2] /= totEdepSoFar;
  *time /= totEdepSoFar;
}

/*  //This code looks for the highest energy detector and uses it.
		if((it->second->totEnergyDep) > totEdepSoFar){
		  totEdepSoFar = it->second->totEnergyDep;
		  ret[0] = (it->second->hPosX);
		  ret[1] = (it->second->hPosY);
		  ret[2] = (it->second->hPosZ);
		  *time  = (it->second->time);
		}
      }
    }
  }
  }*/


// Return the Azimuth angle only.  Check is in main code.
// Optionally return delta time-of-flight between the analyzer and E-array
double NpolPhysicsVariables::ReturnAngle(NpolVertex *incNeutronVert, std::map<std::string,NpolDetectorEvent *> *detEvents, const int SOI, const PolarimeterDetector EArrayOfInterest, double *dTOF = NULL) {
  double targetPt[3];
  double analyzerPt[3];
  double EarrayPt[3];
  double analyzerTime;
  double EarrayTime;

  targetPt[0] = incNeutronVert->gPosX;
  targetPt[1] = incNeutronVert->gPosY;
  targetPt[2] = incNeutronVert->gPosZ;

  GetPoI(analyzerPt,&analyzerTime,SOI,analyzer,detEvents);
  GetPoI2(EarrayPt,&EarrayTime,SOI,EArrayOfInterest,detEvents);

  double azAngle = getAzimuthAngle(targetPt[0],targetPt[1], targetPt[2],
								   analyzerPt[0],analyzerPt[1],analyzerPt[2],
								   EarrayPt[0],EarrayPt[1],EarrayPt[2]);

  if(dTOF != NULL)
    *dTOF = EarrayTime - analyzerTime;
  return azAngle;
}

double NpolPhysicsVariables::getEDepCluster(const std::map<std::string,NpolDetectorEvent *> *detEvents, int SOI, PolarimeterDetector detectorOfInterest) {
  NpolEventProcessing *Process = NpolEventProcessing::GetInstance();
  NpolEventPreProcessing *PProcess = NpolEventPreProcessing::GetInstance();
  
  double totEnergyDeposit = 0.0;
  double highEnergyDeposit = 0.0;
  std::string highestDetector = "";
  
  std::map<std::string,NpolDetectorEvent *>::const_iterator it;
  for(it = detEvents->begin(); it != detEvents->end(); it++) {
	if((SOI ==  Process->sectionNumber(it->first)) && (detectorOfInterest == Process->detectorType(it->first))) {
	  if((it->second->totEnergyDep) > highEnergyDeposit) {
		highestDetector = it->first;
		highEnergyDeposit = it->second->totEnergyDep;
	  }
	}
  }
  
  std::map<std::string,NpolDetectorEvent *>::const_iterator itt;
  int highAVNum = PProcess->GetAVNumber(highestDetector);
  int highImprNum = PProcess->GetImprNumber(highestDetector);
  int highPVNum = PProcess->GetPlacementNumber(highestDetector);
  for(itt = detEvents->begin(); itt != detEvents->end(); itt++) {
	if(detectorOfInterest == analyzer || detectorOfInterest == topdEArray || detectorOfInterest == botdEArray){
	  if((SOI ==  Process->sectionNumber(itt->first)) && (detectorOfInterest == Process->detectorType(itt->first))) {
		if(PProcess->GetAVNumber(itt->first) == highAVNum){
		  if(PProcess->GetImprNumber(itt->first) == highImprNum){
			int PVNum = PProcess->GetPlacementNumber(itt->first);
			if((PVNum == highPVNum) || (PVNum == (highPVNum + 1)) || (PVNum == (highPVNum - 1))){
			  totEnergyDeposit += itt->second->totEnergyDep;
			}
		  }
		}
	  }
	} else if(detectorOfInterest == topEArray || detectorOfInterest == botEArray){
	  if(((SOI ==  Process->sectionNumber(itt->first)) || ((SOI + 1) ==  Process->sectionNumber(itt->first))) && (detectorOfInterest == Process->detectorType(itt->first))) {
		if(PProcess->GetAVNumber(itt->first) == highAVNum){
		  if(PProcess->GetImprNumber(itt->first) == highImprNum){
			int PVNum = PProcess->GetPlacementNumber(itt->first);
			if((PVNum == highPVNum) || (PVNum == (highPVNum + 2)) || (PVNum == (highPVNum - 2))){
			  totEnergyDeposit += itt->second->totEnergyDep;
			}
		  }
		}
	  } 
	}
  }
  
  return totEnergyDeposit;
}
  
double NpolPhysicsVariables::getTotalEnergyDep(const std::map<std::string,NpolDetectorEvent *> *detEvents) {

  double totEnergyDeposit = -1.0;
  std::map<std::string,NpolDetectorEvent *>::const_iterator it;
  for(it = detEvents->begin(); it != detEvents->end(); it++) {
    if((it->second->totEnergyDep) > 0.0)  {
      totEnergyDeposit += it->second->totEnergyDep;
    }
  }

  return totEnergyDeposit;
}

double NpolPhysicsVariables::highestEDepPV(const std::map<std::string,NpolDetectorEvent *> *detEvents, int sectionOfInterest, PolarimeterDetector detectorOfInterest) {
  NpolEventProcessing *Process = NpolEventProcessing::GetInstance();
  
  double highestE_Dep = -1.0;

  std::map<std::string,NpolDetectorEvent *>::const_iterator it;

  for(it = detEvents->begin(); it != detEvents->end(); it++) {
    if((sectionOfInterest == Process->sectionNumber(it->first)) && (detectorOfInterest == Process->detectorType(it->first))) { 
      if((it->second->totEnergyDep) > highestE_Dep) 
		highestE_Dep = it->second->totEnergyDep;

    }
  }
  return highestE_Dep;
}


double NpolPhysicsVariables::computeBetheBloch(double KE, double Mass, int z, double rho, double A, int Z, double I){

  // This function computes the stopping power of a charged  particle through Matter using Bethe formula. 
  // Returns the stopping power in units of MeV/cm. Detailed explanation goes here 
  
  /* Inputs
	 KE = kinetic energy of incident particle (MeV)
	 M = mass of incident particle (MeV/c^2)
	 z = atomic number of incident particle
	 rho = density of absorber
	 A = 12.929;  // (g/mol) Atomic Mass of Absorber (plastic Scintillator)
	 Z = 7; // Atomic Number of Absorber (plastic Scintillator)
	 I = 64.7 * 10 ^(-6); // (MeV) (plastic scintillator)
  */
  // **** Constants **** //
  //double c = 2.998e8;  // speed-of-light
  //double MolarMassConstant = 1; // (g/mol)
  //double alpha = 1/137.035; // fine structure constant
  //double Ne = (NA * Z * rho)/(A * MolarMassConstant);
  // Tc = 00.000350; // (MeV) // no clue; Critical thingy // not used currently
  double M = Mass; // * c^2;
  double mE = 0.510998; // (MeV) mass-energy of electron
  double NA = 6.0221409e23;// Avogadro's Number
  double re = 2.817940e-15 * 100.; // (cm) Classical electron Radius

  // **** Computed values **** //
  double E = KE; //+ M; // Energy means KE? apparently
  double gamma = 1.+E/M;  // kinematic gamma factor
  double beta2 = 1. - 1/(pow(gamma,2));    // kinematic beta^2 factor
  //double beta = sqrt(beta2);         // beta factor
  double K = (4. * TMath::Pi() * NA * pow(re,2) * mE); // (MeV*cm^2/g)
  
  double Tmax = (2*mE*beta2*pow(gamma,2))/(1+2*gamma*(mE/M)+ pow((mE/M),2));

  // Correction factor that isn't working yet.  needed for higher energies only?
  //hw = sqrt(4*pi()*Ne*re^3)*mE/alpha;
  //densityCorr = log(hw/I) + log(beta*gamma) - 0.5;
  //densityCorr = log(beta.*gamma);
  
  double stoppingPower = ((rho*K*z*z*Z)/(A*beta2))*(0.5*log((2*mE*beta2*pow(gamma,2)*Tmax)/(I*I)) - beta2);
  // - densityCorr*beta*gamma);
  return stoppingPower;
}

double NpolPhysicsVariables::computeEnergyLoss(double protonEnergy, double thetaP, double scintThick){

  // **** This energy loss computation is designed for proton on
  //plastic scintillator at the moment **** //

  double A = 12.929;  // atomic mass number for plastic scintillator
  int z = 1; // atomic number of projectile (proton currently)
  int Z = 7; // atomic number of detector material (plastic scintillator)
  double I = 64.7e-6; // Ionization energy of plastic scintillator
  double rho = 1.032; // (g/cm^3) density of plastsic scintillator
  double mP = 938.27205; // (MeV/c^2) mass of proton
  
  //Compute the Energy Deposition variables
  int N = 100;// number of slices of scintillator
  double deltaD = (scintThick)/sin(thetaP);   // (cm)
  double dT = deltaD/N;  // transport thickness
  double stopPower = 0.0;
  double Eloss = 0.0;
  double pEnergy = 0.0;  // present energy value
  
  for(int i = 0; i < N; i++){
	pEnergy = protonEnergy - Eloss; 
	if(pEnergy < 0) break;
	stopPower = computeBetheBloch(pEnergy,mP,z,rho,A,Z,I);
	Eloss += stopPower*dT;
	if(Eloss >= protonEnergy){ Eloss = protonEnergy; break;}
  }
  
  return Eloss;
}


double NpolPhysicsVariables::computeMomentum(double xMom, double yMom, double zMom){

  // Computes the magnitude of the momemtum from the three momentum components.
  double momentumMagnitude = 0.;

  return momentumMagnitude = sqrt(xMom*xMom + yMom*yMom + zMom*zMom);
  
}

double NpolPhysicsVariables::computeScatTheta(double xMom, double yMom){

  // Computes the Theta angle in spherical coordinates.
  double Theta = 0.;

  return Theta = atan(yMom/xMom);
  
}

double NpolPhysicsVariables::computeScatPhi(double zMom, double totalMom){

  // Computes the Phi angle in spherical coordinates.
  double Phi = 0.;

  return Phi = acos(zMom/totalMom);

}

double NpolPhysicsVariables::computeElasticMomentum(const std::pair<double,std::vector<double>> &projNeutron4Vec, double thetaP){

  // **** This method computes the momentum of a recoil proton struck
  // **** by a neutron given the momentum(energy) of the neutron and
  // **** the angle of the recoil proton from the initial neutron direction.
  double momentum = 0.0;
  double mN = 939.56538; // (MeV/c^2)
  double mP = 938.27205; // (MeV/c^2)
  double Pnc = returnParticleMomentum(projNeutron4Vec);
  double En = sqrt(pow(Pnc,2) + pow(mN,2));
 
  double alpha = En + mP;
  double beta = Pnc * cos(thetaP);
  double gamma = pow(mP,2) + En * mP;
  double A = pow(alpha,2) - pow(beta,2);
  double B = -2 * alpha * gamma;
  double C = pow(gamma,2) + pow(beta,2) * pow(mP,2);

  double recoilEnergy = (-B + sqrt(pow(B,2) - 4 * A * C))/(2 * A);
  momentum = sqrt(pow(recoilEnergy,2) - pow(mP,2));
  
  return momentum;
}


bool NpolPhysicsVariables::checkQuasiElasticScattering(const std::map<int,NpolVertex *> &theVertexMap, const std::pair<double,std::vector<double>> &projNeutron4Vec){

  NpolEventProcessing *Process = NpolEventProcessing::GetInstance();
  bool QEflag = false;
  double highNeutronMomentum = 0.0; 
  std::pair<double,std::vector<double>> currentParticle4Vec;
  std::pair<double,std::vector<double>> highestParticle4Vec;

  std::vector<double> a3Vector = {0.0, 0.0,0.0};
  highestParticle4Vec.first = 0.0;
  highestParticle4Vec.second = a3Vector;
  
  std::map<int,NpolVertex *>::const_iterator mapIt;
  for(mapIt = theVertexMap.begin(); mapIt != theVertexMap.end(); mapIt++){
	NpolVertex *aVertex = mapIt->second;
	if(aVertex == NULL) continue;
	Process->fillFourVector(aVertex,currentParticle4Vec);

	double currentMomentum = returnParticleMomentum(currentParticle4Vec);
	int currentPType = mapIt->second->particleId;

	if(mapIt->first > 1 && currentPType == 2112 && currentMomentum > highNeutronMomentum){
	  highNeutronMomentum = currentMomentum;
	  Process->fillFourVector(aVertex,highestParticle4Vec);
	} else {
	  continue;
	}
  }
  
  double initial4VecMag = sqrt(abs(compute4VecSquared(projNeutron4Vec)));
  double final4VecMag = sqrt(abs(compute4VecSquared(highestParticle4Vec)));
  currentParticle4Vec.second.clear();
  highestParticle4Vec.second.clear();
  
  if(final4VecMag >= 0.95*initial4VecMag){
	return QEflag = true;
  } else {
	return QEflag = false;
  }
}

double NpolPhysicsVariables::compute4VecSquared(const std::pair<double,std::vector<double>> &aParticle4Vec){

  double Squared4Vec = 0.0;
  double particleMomentum = returnParticleMomentum(aParticle4Vec);
  double particleEnergy = returnParticleEnergy(aParticle4Vec);
   
  Squared4Vec = pow(particleMomentum,2) - pow(particleEnergy,2); 
  return Squared4Vec;
}

double NpolPhysicsVariables::computeLeadingParticleMomentum(const std::map<int,NpolVertex *> &theVertexMap,int selectedTID){
  double maximalMomentum = 0.0;
  
  std::map<int,NpolVertex *>::const_iterator mapIt;
  mapIt = theVertexMap.find(selectedTID);
  maximalMomentum =
	computeMomentum(mapIt->second->momX,mapIt->second->momY,mapIt->second->momZ);
  return maximalMomentum;
}

double NpolPhysicsVariables::computeRecoilParticleAngle(const std::pair<double,std::vector<double>> &projNeutron4Vec, const std::map<int,NpolVertex *> &theVertexMap, int selectedTID){
  
  // Extract Initial Initial Neutron Momentum 
  std::vector<double> initNeutron3Mom = projNeutron4Vec.second;
  double initNxMom = initNeutron3Mom[0];
  double initNyMom = initNeutron3Mom[1];
  double initNzMom = initNeutron3Mom[2];
  double initNeutronMomMag = returnParticleMomentum(projNeutron4Vec);

  // Extract Recoil Momentum components from vertex vector
  std::map<int,NpolVertex *>::const_iterator mapIt;
  mapIt = theVertexMap.find(selectedTID);
  double RecoilxMom = mapIt->second->momX;
  double RecoilyMom = mapIt->second->momY;
  double RecoilzMom = mapIt->second->momZ;
  double recoilMomMag = computeMomentum(RecoilxMom,RecoilyMom,RecoilzMom);
  
  // Now compute the recoil angle (returned from getAzimuthAngle in degrees)
  double recoilAngle =
	TMath::ACos((initNxMom*RecoilxMom + initNyMom*RecoilyMom + initNzMom*RecoilzMom)/(initNeutronMomMag*recoilMomMag));

  initNeutron3Mom.clear();
  return TMath::RadToDeg()*recoilAngle; 
}


int NpolPhysicsVariables::findLeadingParticle(const std::map<int,NpolVertex *> &theVertexMap, std::string eventType){

  double maximalMomentum = 0.0; double maxNeutronMomentum = 0.0;
  int maximalTID = -1; int maxNeutronTID = -1;
  
  std::map<int,NpolVertex *>::const_iterator mapIt;

  for(mapIt = theVertexMap.begin(); mapIt != theVertexMap.end(); mapIt++){
	double currentMomentum = computeMomentum(mapIt->second->momX,mapIt->second->momY,mapIt->second->momZ);
	int currentPType = mapIt->second->particleId;
	// 10/6/2018:  Added in proton to this if statement
	if(eventType == "neutronInelastic"){
	  if(mapIt->first > 1  && (currentPType == 2112 || currentPType == 2212) && currentMomentum > maxNeutronMomentum){
		maxNeutronMomentum = currentMomentum;
		maxNeutronTID = mapIt->first;
	  }
	}
  }
  
  for(mapIt = theVertexMap.begin(); mapIt != theVertexMap.end(); mapIt++){
	double currentMomentum =
	  computeMomentum(mapIt->second->momX,mapIt->second->momY,mapIt->second->momZ);
	int currentPType = mapIt->second->particleId;
	if(mapIt->first > 1  && mapIt->first != maxNeutronTID){
	  if(currentPType == 2112 || currentPType == 2212){
		if(currentMomentum > maximalMomentum){
		  maximalMomentum = currentMomentum;
		  maximalTID = mapIt->first;
		}
	  }
	}
  }
  return maximalTID;
}


double NpolPhysicsVariables::returnParticleMomentum(const std::pair<double,std::vector<double> > &aParticle4Vec){

  double particleMomentum = 0.0;
  
  std::vector<double> p;
  if(aParticle4Vec.second.size() != 0) p = aParticle4Vec.second;
  particleMomentum = computeMomentum(p[0],p[1],p[2]);
  p.clear();
  
  return particleMomentum;
}

double NpolPhysicsVariables::returnParticleEnergy(const std::pair<double,std::vector<double> > &aParticle4Vec){

  double particleEnergy = 0.0;
  particleEnergy = aParticle4Vec.first;
  return particleEnergy;
  
}

double NpolPhysicsVariables::returnParticleEnergy(const std::map<int,NpolVertex *> &theVertexMap, int TID){

  double neutronEnergy = 0.0;
  std::map<int,NpolVertex *>::const_iterator mapIt;
  mapIt = theVertexMap.find(TID);
  neutronEnergy = mapIt->second->energy;

  return neutronEnergy;
}

int NpolPhysicsVariables::findBestProtonTrackID(const std::map<int,NpolVertex *> &theVertexMap, const std::vector<NpolStep *> *steps, int npSOI){

  // This will return the track ID of the "good" proton track from the
  // vertexMap of particles generated from the initial collision. 
  NpolEventPreProcessing *PProcess = NpolEventPreProcessing::GetInstance();
  NpolEventProcessing *Process = NpolEventProcessing::GetInstance();
  
  double maxEnergy = 0.0;
  int bestProtonTID = 0;
  std::map<int,NpolVertex *>::const_iterator mapIt;
  for(mapIt = theVertexMap.begin(); mapIt != theVertexMap.end(); mapIt++){
	bool TopdEdetFlag = false; bool TopEdetFlag = false;
	bool BotdEdetFlag = false; bool BotEdetFlag = false;
	int pType = mapIt->second->particleId;
	int PID = mapIt->second->parentId;
	int TID = mapIt->first;  // this is equal to trackId in mapIt->second->trackId
	double particleEnergy = mapIt->second->energy;
	if(TID == 1) continue;
	if(pType == 2212 && particleEnergy >= 25 /*MeV*/){
	  std::vector<NpolStep *>::const_iterator ss_it;
	  for(ss_it = steps->begin(); ss_it != steps->end(); ss_it++) {
		NpolStep *aStep = *ss_it;
		if(aStep == NULL) continue;
		
		int stepPID = aStep->parentId;
		int stepTID = aStep->trackId;
		if((stepPID == PID) && (stepTID == TID)){
		  std::string stepVolName = aStep->volume;
		  int stepAVNum = PProcess->GetAVNumber(stepVolName);
		  int stepSection = Process->sectionNumber(stepVolName);
		  if(stepSection == npSOI){
			if(stepAVNum == 1 || stepAVNum == 2) TopEdetFlag = true;
			if(stepAVNum == 3 || stepAVNum == 4) TopdEdetFlag = true;
			if(stepAVNum == 5 || stepAVNum == 6) BotEdetFlag = true;
			if(stepAVNum == 7 || stepAVNum == 8) BotdEdetFlag = true;
		  }
		}
	  }
	}
	
	bool TopFlag = false; bool BotFlag = false;
	if(TopEdetFlag && TopdEdetFlag) TopFlag = true;
	if(BotEdetFlag && BotdEdetFlag) BotFlag = true;
	if((TopFlag  && !BotFlag) || (!TopFlag && BotFlag)){// ExOR
	  if(particleEnergy > maxEnergy){
		bestProtonTID = TID;
		maxEnergy = particleEnergy;
	  }
	}
  }
  
  return bestProtonTID;
}

